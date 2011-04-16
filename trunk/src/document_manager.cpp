/**
 * This file is part of the raptor project.
 *
 * Copyright (C) 2011 Christophe Duvernois <christophe.duvernois@gmail.com>
 *
 * Raptor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Raptor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <QtGui>
#include <Qsci/qscimacro.h>

#include "settings/settings.h"
#include "Diff.h"
#include "document_editor.h"
#include "document_view.h"
#include "document_manager.h"

DocumentManager::DocumentManager(QWidget * parent_) : Splitter(Qt::Horizontal, parent_) {		
	_macro.clear();
	//create file watcher
	_watcher = new QFileSystemWatcher(this);
	connect(_watcher, SIGNAL(fileChanged(const QString&)), this, SLOT(watchedFileChanged(const QString&)));
	
	//create view
	_activeView = createDocumentView();
}
DocumentManager::~DocumentManager(){

}

QFileSystemWatcher& DocumentManager::getFileSystemWatcher(){
	return *_watcher;
}

DocumentView* DocumentManager::createDocumentView(DocumentEditor* document_){
	//create and connect the new view
	DocumentView* view = new DocumentView(*_watcher, document_, this);
	//connect(view, SIGNAL(destroyViewRequested()), this, SLOT(destroyView()));
	connect(view, SIGNAL(empty()), this, SLOT(updateView()));
	connect(view, SIGNAL(activeStatusChanged(bool)), this, SLOT(activeViewChanged(bool)));
	connect(view, SIGNAL(documentChanged(DocumentEditor*)), this, SLOT(activeViewChanged()));
	connect(view, SIGNAL(documentChanged(DocumentEditor*)), this, SIGNAL(documentChanged(DocumentEditor*)));
	connect(view, SIGNAL(selectionChanged(DocumentEditor*)), this, SIGNAL(selectionChanged(DocumentEditor*)));
	connect(view, SIGNAL(cursorPositionChanged(DocumentEditor*, int, int)), this, SIGNAL(cursorPositionChanged(DocumentEditor*, int, int)));
	connect(view, SIGNAL(documentMoveRequested(QWidget*, int, QWidget*, int)), this, SLOT(moveDocument(QWidget*, int, QWidget*, int)));
	//add context menu
	QMenu *menu = new QMenu(view);
	menu->setObjectName("TabContextMenu");
	QAction* close = menu->addAction(QIcon(":/images/closeallpage.png"), tr("Close"));
	connect(close, SIGNAL(triggered()), view, SLOT(closeCurrentDocument()));
	QAction* closeAllExceptCurrent = menu->addAction(QIcon(":/images/forbiddenpage.png"), tr("Close All Except Me"));
	connect(closeAllExceptCurrent, SIGNAL(triggered()), view, SLOT(closeAllDocumentsExceptCurrent()));
	QAction* save = menu->addAction(QIcon(":/images/save.png"), tr("&Save"));
	connect(save, SIGNAL(triggered()), view, SLOT(save()));
	QAction* saveAs = menu->addAction(QIcon(":/images/saveas.png"), tr("&Save As..."));
	connect(saveAs, SIGNAL(triggered()), view, SLOT(saveAs()));
	menu->addSeparator();
	QAction* moveTo = menu->addAction(QIcon(":/images/moveto.png"), tr("&Move to other view"));
	connect(moveTo, SIGNAL(triggered()), this, SLOT(moveDocument()));
	QAction* cloneTo = menu->addAction(QIcon(":/images/cloneto.png"), tr("&Clone to other view"));
	connect(cloneTo, SIGNAL(triggered()), this, SLOT(cloneDocument()));
	view->setContextMenu(menu);

	//add view to splitter
	addWidget(view);
	
	//add view to the list
	_viewList.push_back(view);
	return view;
}

DocumentView* DocumentManager::getActiveView() const{
	return _activeView;
}
DocumentEditor* DocumentManager::getActiveDocument() const{
	return _activeView->currentDocument();
}
DocumentEditor* DocumentManager::getDocument(const QString& name_){
	std::vector<DocumentView*>::iterator it;
	for(it = _viewList.begin(); it < _viewList.end(); ++it){
		DocumentEditor* doc = (*it)->getDocument(name_);
		if(doc != 0)
			return doc;
	}
	return 0;
}
QList<DocumentEditor*> DocumentManager::getDocuments(){
	QList<DocumentEditor*> list;
	//for each view
	std::vector<DocumentView*>::iterator it;
	for(it = _viewList.begin(); it < _viewList.end(); ++it){
		//fill the list
		list.append((*it)->getDocuments());
	}
	return list;
}

void DocumentManager::setActiveDocument(DocumentEditor* document_){
	std::vector<DocumentView*>::iterator it;
	for(it = _viewList.begin(); it < _viewList.end(); ++it){
		if((*it)->setActiveDocument(document_))
			return;
	}
}
void DocumentManager::setActiveDocument(const QString& name_){
	std::vector<DocumentView*>::iterator it;
	for(it = _viewList.begin(); it < _viewList.end(); ++it){
		if((*it)->setActiveDocument(name_))
			return;
	}
}

void DocumentManager::activeViewChanged(bool active_){
	if(active_ == true){
		DocumentView* view = qobject_cast<DocumentView*>(sender());
		_activeView = view;
	}
}
void DocumentManager::updateView(){
	if(_viewList.size() == 1)
		return;
	
	DocumentView* view = qobject_cast<DocumentView*>(sender());
	std::vector<DocumentView*>::iterator it;
	for(it = _viewList.begin(); it < _viewList.end(); ++it){
		if((*it) != view){
			if(!((*it)->isVisible()))
				return;	//the other view is not visible keep this one
			else
				_activeView = (*it);
		}
	}
	view->setVisible(false);
	view->setParent(0);
	//add view to splitter
	addWidget(view);
}

void DocumentManager::destroyView(DocumentView* view_){
	//we need at least one view
	if(_viewList.size() == 1)
		return;

	if(view_ == 0){
		//get pointer on the view which send the signal
		view_ = qobject_cast<DocumentView*>(sender());
		if(view_ == 0){
			qCritical() << "destroyView NULL pointer";
			return;
		}
	}

	std::vector<DocumentView*>::iterator it;
	for(it = _viewList.begin(); it < _viewList.end(); ++it){
		if(*it == view_){
			//update active view
			if(view_ == _activeView)
				_activeView = 0;
			//remove view from the list	
			_viewList.erase(it);
			//delete view
			view_->deleteLater(); //delete provoques a crash
		}
	}
	
	_activeView = _viewList[0];
	//_activeView->currentDocument()->setFocus();
}

bool DocumentManager::documentExists(const QString& name_){
	std::vector<DocumentView*>::iterator it;
	for(it = _viewList.begin(); it < _viewList.end(); ++it){
		if((*it)->documentExists(name_))
			return true;
	}
	return false;
}

void DocumentManager::openDocument(const QString& file_){
	if(documentExists(file_)){
		setActiveDocument(file_);
		return;
	}
	getActiveView()->openDocument(file_);
}
void DocumentManager::openDocument(QStringList& files_){
	int i = 0;
	while(i != files_.size()){
		QString fileName = files_[i];
		
		bool fileRemoved = false;
		if(documentExists(fileName)){
			setActiveDocument(fileName);
			files_.removeAt(i);
			fileRemoved = true;
		}
		if(!fileRemoved)
			i++;
	}
	getActiveView()->openDocument(files_);
}

bool DocumentManager::saveAll(){
	std::vector<DocumentView*>::iterator it;
	for(it = _viewList.begin(); it < _viewList.end(); ++it){
		(*it)->saveAll();
	}
	return true;
}
bool DocumentManager::closeAll(){
	std::vector<DocumentView*>::iterator it;
	for(it = _viewList.begin(); it < _viewList.end(); ++it){
		if((*it)->closeAll() == false)
			return false;
	}
	return true;
}
bool DocumentManager::closeAllExceptActiveDocument(){
	std::vector<DocumentView*>::iterator it;
	for(it = _viewList.begin(); it < _viewList.end(); ++it){
		if((*it) == _activeView)
			continue;
		if((*it)->closeAll() == false)
			return false;
	}
	
	_activeView->closeAllDocumentsExceptCurrent();
	return true;
}

void DocumentManager::moveDocument(){
	int nbView = _viewList.size();
	for(int i = 0; i < nbView; i++){
		if(_activeView == _viewList[i]){
			DocumentEditor* document = _activeView->removeCurrentDocument();
			if(nbView == 1)
				createDocumentView(document);
			else{
				int nb = (i+1)%nbView;
				_viewList[nb]->addDocument(document);
				_viewList[nb]->setVisible(true);
			}
			return;
		}
	}
}

void DocumentManager::moveDocument(QWidget* src_, int srcIndex_, QWidget* dest_, int destIndex_){
	DocumentView* srcView = (DocumentView*)src_->parent();
	DocumentView* destView = (DocumentView*)dest_->parent();
	
	DocumentEditor* doc = srcView->removeDocumentAt(srcIndex_);
	if(destIndex_ < 0){
		destView->addDocument(doc);
	}else{
		destView->insertDocument(destIndex_, doc);
	}
}

void DocumentManager::cloneDocument(){
	int nbView = _viewList.size();
	for(int i = 0; i < nbView; i++){
		if(_activeView == _viewList[i]){
			DocumentEditor* document = _activeView->currentDocument();
			if(nbView == 1){
				DocumentView* docView = createDocumentView();
				docView->cloneDocument(document);
				//remove document created with the view
				docView->closeDocument(0);
			}else{
				int nb = (i+1)%nbView;
				_viewList[nb]->cloneDocument(document);
			}
			return;
		}
	}
}

void DocumentManager::saveSession(){
	QMessageBox::information(this, PACKAGE_NAME, tr("Not implemented yet !!"));
}
void DocumentManager::restoreSession(){
	QMessageBox::information(this, PACKAGE_NAME, tr("Not implemented yet !!"));
}

void DocumentManager::saveCurrentSession(QSettings& settings_){
	settings_.setValue("View/number", (int)_viewList.size());
	for(unsigned int i = 0; i < _viewList.size(); i++){
		settings_.setValue(QString("View/View%1/files").arg(i), _viewList[i]->getDocumentNameList());
	}
}

void DocumentManager::restoreLastSession(QSettings& settings_){
	int nbView = settings_.value("View/number", 0).toInt();
	for(int i = 0; i < nbView; i++){
		QStringList list  = settings_.value(QString("View/View%1/files").arg(i)).toStringList();
		foreach(QString fileName, list){
			if(fileName.isEmpty())
				continue;
			openDocument(fileName);
			if(i == 1){
				moveDocument();
			}
		}
	}
}

void DocumentManager::reindentDocument(){
	getActiveDocument()->reindent();
}

void DocumentManager::reindentOpenDocuments(){
	QMessageBox::information(this, PACKAGE_NAME, tr("Not implemented yet !!"));
}

void DocumentManager::diff(){
	DocumentView* view1 = _viewList[0];
	DocumentView* view2 = _viewList[1];
	DocumentEditor* doc1 = view1->currentDocument();
	DocumentEditor* doc2 = view2->currentDocument();
	
	Compare c(doc1, doc2);
	c.diff();
}

QString DocumentManager::getCurrentMacro() const{
	return _macro;
}

void DocumentManager::startRecordingMacro(){
	//clear the current macro
	_macro.clear();
	//start recording
	getActiveDocument()->getMacro()->startRecording();
}

void DocumentManager::stopRecordingMacro(){
	QsciMacro* m = getActiveDocument()->getMacro();
	//stop recording
	m->endRecording();
	//get the current macro
	_macro = m->save();
}

void DocumentManager::runMacro(int times_){
	if(!_macro.isEmpty()){
		QsciMacro* m = getActiveDocument()->getMacro();
		//load the macro
		if(m->load(_macro)){
			//run it
			for(int i = 0; i < times_; i++)
				m->play();
			return;
		}
	}
	QMessageBox::warning(this, PACKAGE_NAME, tr("Cannot run the macro! Maybe Record one\n"));
}

void DocumentManager::runMacroUntilEOF(){
	if(!_macro.isEmpty()){
		QsciMacro* m = getActiveDocument()->getMacro();
		//load the macro
		if(m->load(_macro)){
			//run it
			DocumentEditor* document = getActiveDocument();
			int nbLines = document->lines();
			int line, column;
			document->getCursorPosition(&line, &column);
			while(line < nbLines){
				m->play();
				int oldLine = line;
				document->getCursorPosition(&line, &column);
				if(oldLine == line)
					break;
			}
			return;
		}
	}
	QMessageBox::warning(this, PACKAGE_NAME, tr("Cannot run the macro! Maybe Record one\n"));	
}

void DocumentManager::watchedFileChanged(const QString& path_){
	//remove path_ of the filewatcher
	_watcher->removePath(path_);
	//add path to the notification list
	_watcherNotification << path_;
}

void DocumentManager::notify(){
	///@todo update tab save color icon when file is not reloaded

	//FileSystemWatcher notification
	_watcherNotification.removeDuplicates();
	
	QStringList list = _watcherNotification;
	//clear notification list
	_watcherNotification.clear();
	
	foreach(QString file, list){
		int ret;
		if (QFile(file).exists()) {
			DocumentEditor* document = 0;
			for(std::vector<DocumentView*>::iterator it = _viewList.begin(); it < _viewList.end(); ++it){
				document = (*it)->getDocument(file);
				if(document == 0)
					continue;

				if(document->isModified() == false){
					ret = QMessageBox::question(this ,
							tr("Reload ?"),
							tr("%1\nThis file has been modified by another program.\nDo you want to reload it?").arg(file),
							QMessageBox::Yes | QMessageBox::No);
				}
				else{
					ret = QMessageBox::question(this,
							tr("Reload ?"),
							tr("%1\nThis file has been modified by another program.\nDo you want to reload it and lose all the changes made?").arg(file),
							QMessageBox::Yes | QMessageBox::No);
				}

				if (ret == QMessageBox::Yes){
					document->load(file);
				}
			}
		}
		else {
			ret = QMessageBox::question(this,
						tr("Keep non existing file ?"),
						tr("%1\ndoesn't exist anymore.\nKeep this file in editor?").arg(file),
						QMessageBox::Yes | QMessageBox::No);
			if (ret == QMessageBox::No){
				for(std::vector<DocumentView*>::iterator it = _viewList.begin(); it < _viewList.end(); ++it){
					(*it)->closeDocument(file);
				}
			}
		}		
	}

	//update all views
	for(std::vector<DocumentView*>::iterator it = _viewList.begin(); it < _viewList.end(); ++it){
		(*it)->updateAllDocuments();
	}
}
