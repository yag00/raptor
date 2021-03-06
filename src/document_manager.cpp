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

#include <QtWidgets>
#include <Qsci/qscimacro.h>

#include "settings/Settings.h"
#include "diff/Unidiff.h"
#include "diff/UnidiffDialog.h"
#include "diff/CompareDialog.h"
#include "switcher/Switcher.h"
#include "switcher/VerticalFileSwitcher.h"
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
	connect(view, SIGNAL(opened(QStringList)), this, SIGNAL(opened(QStringList)));
	connect(view, SIGNAL(saved(QStringList)), this, SIGNAL(saved(QStringList)));
	connect(view, SIGNAL(activeDocumentChanged(DocumentEditor*)), this, SIGNAL(activeDocumentChanged(DocumentEditor*)));
	connect(view, SIGNAL(activeDocumentChanged(DocumentEditor*)), this, SLOT(activeViewChanged()));
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
	QAction* copyNameToClipboard = menu->addAction(QIcon(":/images/copy.png"), tr("Copy Filename to clipboard"));
	connect(copyNameToClipboard, SIGNAL(triggered()), this, SLOT(copyFileNameToClipboard()));
	QAction* copyPathToClipboard = menu->addAction(QIcon(":/images/copy.png"), tr("Copy File path to clipboard"));
	connect(copyPathToClipboard, SIGNAL(triggered()), this, SLOT(copyFilePathToClipboard()));
	menu->addSeparator();
	QAction* rename = menu->addAction(QIcon(":/listeditor/edit.png"), tr("Rename"));
	connect(rename, SIGNAL(triggered()), this, SLOT(rename()));
	QAction* reload = menu->addAction(QIcon(":/images/restore.png"), tr("Reload"));
	connect(reload, SIGNAL(triggered()), this, SLOT(reload()));
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
		if((*it)->isVisible()){
			//fill the list
			list.append((*it)->getDocuments());
		}
	}
	return list;
}
QList<DocumentEditor*> DocumentManager::getDocuments(int view_){
	if(view_ >= (int)_viewList.size()){
		QList<DocumentEditor*> empty;
		return empty;
	}
	return _viewList[view_]->getDocuments();
}
QStringList DocumentManager::getDocumentsNameList(){
	QStringList list;
	//for each view
	std::vector<DocumentView*>::iterator it;
	for(it = _viewList.begin(); it < _viewList.end(); ++it){
		if((*it)->isVisible()){
			//fill the list
			list.append((*it)->getDocumentNameList());
		}
	}
	return list;
}
QStringList DocumentManager::getDocumentsNameList(int view_){
	if(view_ >= (int)_viewList.size()){
		QStringList empty;
		return empty;
	}
	return _viewList[view_]->getDocumentNameList();
}
int DocumentManager::getViewNumber() const{
	int i = 0;
	for(std::vector<DocumentView*>::const_iterator it = _viewList.begin(); it != _viewList.end(); ++it){
		if((*it)->isVisible())
			i++;
	}
	return i;
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

bool DocumentManager::documentExists(const QString& name_){
	std::vector<DocumentView*>::iterator it;
	for(it = _viewList.begin(); it < _viewList.end(); ++it){
		if((*it)->documentExists(name_))
			return true;
	}
	return false;
}

//==================== files slots ====================//

void DocumentManager::newDocument(){
	getActiveView()->newDocument();
}

void DocumentManager::open(){
	///@todo add filter
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select one or more files to open"), getActiveDocument()->getPath());
	open(files);
}

void DocumentManager::open(const QString& file_){
	if(documentExists(file_)){
		setActiveDocument(file_);
		return;
	}
	getActiveView()->openDocument(file_);
}

void DocumentManager::open(QStringList& files_){
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

void DocumentManager::open(QStringList& files_, int view_){
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

	if(view_ >= (int)_viewList.size()){
		DocumentView* view = createDocumentView();
		view->openDocument(files_);
		view->setVisible(true);
	}else{
		_viewList[view_]->openDocument(files_);
		_viewList[view_]->setVisible(true);
	}
}

void DocumentManager::save(){
	getActiveView()->save();
}

void DocumentManager::saveAs(){
	getActiveView()->saveAs();
}

void DocumentManager::saveACopyAs(){
	getActiveView()->saveACopyAs();
}

void DocumentManager::saveAll(){
	std::vector<DocumentView*>::iterator it;
	for(it = _viewList.begin(); it < _viewList.end(); ++it){
		(*it)->saveAll();
	}
}

bool DocumentManager::close(){
	return getActiveView()->closeCurrentDocument();
}

bool DocumentManager::closeAll(){
	std::vector<DocumentView*>::iterator it;
	for(it = _viewList.begin(); it < _viewList.end(); ++it){
		if((*it)->closeAll() == false)
			return false;
	}
	return true;
}

bool DocumentManager::closeAllExceptCurrentDocument(){
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

bool DocumentManager::closeAllIfPossible(){
	QList<DocumentEditor*> list = getDocuments();
	foreach(DocumentEditor* doc, list){
		if(doc->isModified())
			return false;
	}
	return closeAll();
}

void DocumentManager::reload(){
	if(getActiveDocument()->reload()) {
		getActiveView()->updateAllDocuments();
		emit statusMessage(tr("File reloaded"));
	}
}

void DocumentManager::rename(){
	if(getActiveDocument()->rename()) {
		emit statusMessage(tr("File renamed"));
	}
}

void DocumentManager::copyFileNameToClipboard(){
	QClipboard *clipboard = QApplication::clipboard();
	QString txt = getActiveDocument()->getName();
	clipboard->setText(txt);
	emit statusMessage(tr("Copy %1 to clipboard").arg(txt));
}

void DocumentManager::copyFilePathToClipboard(){
	QClipboard *clipboard = QApplication::clipboard();
	QString txt = getActiveDocument()->getFullPath();
	clipboard->setText(txt);
	emit statusMessage(tr("Copy %1 to clipboard").arg(txt));
}

void DocumentManager::print(){
	getActiveView()->print();
}
//==================== edition slots ====================//

void DocumentManager::cut(){
	getActiveDocument()->cut();
}

void DocumentManager::copy(){
	getActiveDocument()->copy();
}

void DocumentManager::paste(){
	getActiveDocument()->paste();
}

void DocumentManager::undo(){
	getActiveDocument()->undo();
}

void DocumentManager::redo(){
	getActiveDocument()->redo();
}

void DocumentManager::selectAll(){
	getActiveDocument()->selectAll();
}

void DocumentManager::increaseIndentation(){
	getActiveDocument()->increaseIndentation();
}

void DocumentManager::decreaseIndentation(){
	getActiveDocument()->decreaseIndentation();
}

void DocumentManager::setAutoIndentation(bool b_){
	getActiveDocument()->setAutoIndent(b_);
}

void DocumentManager::showIndentationGuides(bool b_){
	getActiveDocument()->setIndentationGuides(b_);
}

void DocumentManager::tabsToSpaces(){
	getActiveDocument()->tabsToSpaces();
}

void DocumentManager::spacesToTabs(){
	getActiveDocument()->spacesToTabs();
}

void DocumentManager::trimTrailingSpaces(){
	getActiveDocument()->trimTrailingSpaces();
}

void DocumentManager::compressSpaces(){
	getActiveDocument()->compressSpaces();
}

void DocumentManager::convertSelectedTextToUpperCase(){
	getActiveDocument()->convertSelectedTextToUpperCase();
}

void DocumentManager::convertSelectedTextToLowerCase(){
	getActiveDocument()->convertSelectedTextToLowerCase();
}

void DocumentManager::duplicateCurrentLine(){
	getActiveDocument()->duplicateSelectionOrCurrentLine();
}

void DocumentManager::copyCurrentLine(){
	getActiveDocument()->copyCurrentLine();
}

void DocumentManager::cutCurrentLine(){
	getActiveDocument()->cutCurrentLine();
}

void DocumentManager::deleteCurrentLine(){
	getActiveDocument()->deleteCurrentLine();
}

void DocumentManager::moveCurrentLineUp(){
	getActiveDocument()->moveCurrentLineUp();
}

void DocumentManager::moveCurrentLineDown(){
	getActiveDocument()->moveCurrentLineDown();
}

void DocumentManager::transposeLine(){
	getActiveDocument()->transposeLine();
}

void DocumentManager::deleteCurrentWord(){
	getActiveDocument()->deleteCurrentWord();
}

void DocumentManager::setReadOnly(bool b_){
	getActiveDocument()->setReadOnly(b_);
	getActiveView()->updateAllDocuments();
}

void DocumentManager::reindentDocument(){
	getActiveDocument()->reindent();
}

void DocumentManager::reindentOpenDocuments(){
	QMessageBox::information(this, PACKAGE_NAME, tr("Not implemented yet !!"));
}
void DocumentManager::toggleComment(){
	//prefer single line comment when possible
	getActiveDocument()->toggleComment(true);
}
void DocumentManager::toggleBlockComment(){
	//do not use single line comment on selection
	getActiveDocument()->toggleComment(false);
}
void DocumentManager::gotoLine(int line_){
	getActiveDocument()->gotoLine(line_);
}
void DocumentManager::switchDocumentSrc(){
	Settings settings;
	QMap<QString, QStringList> switcher = settings.getSwitcherValues();

	DocumentEditor* doc = getActiveDocument();
	QString name = doc->getName();

	QList<QString> keys = switcher.keys();
	foreach(QString k, keys){
		if(QDir::match(k, name)){
			QString path = doc->getPath();
			QString basename = doc->getBaseName();

			QStringList tryList;
			foreach(QString suffix, switcher[k]){
				QString switchPath = path + QDir::separator() + suffix.replace(0,1, basename);
				tryList << switchPath;
				//if file exists, open it
				if(QFileInfo(switchPath).exists()){
					open(switchPath);
					return;
				}
			}
			//no file matches, notify and return
			if(settings.getSwitcherNotification()){
				int ret = QMessageBox::question(this, tr("Switcher : Fail !"), tr("Can't find files according to switching rules!\nDo you want to create the file?"), QMessageBox::Yes | QMessageBox::No);
				if (ret == QMessageBox::Yes){
					bool ok;
					QString	fileToCreate = QInputDialog::getItem(this, tr("Switcher : Create new File"), tr("Select or type the new file to create :"), tryList, 0, true, &ok);
					if(ok){
						QFile file(fileToCreate);
						file.open(QIODevice::WriteOnly);
						file.close();
						open(fileToCreate);
					}
				}
			}
			return;
		}
	}
}

void DocumentManager::switchFile(){
	Switcher* sw = this->parent()->findChild<Switcher*>("Switcher");
	if(!sw){
		QList<DocumentEditor*> list = getDocuments();
		//QSplitter parent can't be parent, otherwise switcher will be integrate to splitter
		VerticalFileSwitcher* switcher = new VerticalFileSwitcher(list, (QWidget*)this->parent());
		connect(switcher, SIGNAL(selected(QString)), this, SLOT(setActiveDocument(QString)));
		switcher->show();
	}else{
		sw->close();
	}
}

//==================== view slots ====================//

void DocumentManager::zoomIn(){
	getActiveDocument()->zoomIn();
}

void DocumentManager::zoomOut(){
	getActiveDocument()->zoomOut();
}

void DocumentManager::zoomRestore(){
	getActiveDocument()->zoomTo(0);
}

void DocumentManager::showWhiteSpaceAndTab(bool b_){
	getActiveDocument()->setEolVisibility(false);
	if(b_)
		getActiveDocument()->setWhitespaceVisibility(QsciScintilla::WsVisible);
	else
		getActiveDocument()->setWhitespaceVisibility(QsciScintilla::WsInvisible);
}

void DocumentManager::showEndOfLine(bool b_){
	getActiveDocument()->setEolVisibility(b_);
	getActiveDocument()->setWhitespaceVisibility(QsciScintilla::WsInvisible);
}

void DocumentManager::showAll(bool b_){
	getActiveDocument()->setEolVisibility(b_);
	if(b_)
		getActiveDocument()->setWhitespaceVisibility(QsciScintilla::WsVisible);
	else
		getActiveDocument()->setWhitespaceVisibility(QsciScintilla::WsInvisible);
}

void DocumentManager::foldUnfoldAll(){
	getActiveDocument()->foldAll(true);
}

void DocumentManager::foldLevel(int level_){
	getActiveDocument()->foldLevel(level_);
}

void DocumentManager::unfoldLevel(int level_){
	getActiveDocument()->unfoldLevel(level_);
}

//==================== encoding slots ====================//

void DocumentManager::convertToWindowFormat(){
	DocumentEditor* document = getActiveDocument();
	document->convertEols(QsciScintilla::EolWindows);
	document->setEolMode(QsciScintilla::EolWindows);
}

void DocumentManager::convertToUnixFormat(){
	DocumentEditor* document = getActiveDocument();
	document->convertEols(QsciScintilla::EolUnix);
	document->setEolMode(QsciScintilla::EolUnix);
}

void DocumentManager::convertToMacFormat(){
	DocumentEditor* document = getActiveDocument();
	document->convertEols(QsciScintilla::EolMac);
	document->setEolMode(QsciScintilla::EolMac);
}

void DocumentManager::changeCharset(const QString& codec_){
	if(getActiveDocument()->setCodec(codec_)) {
		getActiveView()->updateAllDocuments();
		emit statusMessage(tr("File reloaded with charset %1").arg(codec_));
	}
}
void DocumentManager::saveWithCharset(const QString& codec_){
	if(getActiveDocument()->saveWithCharset(codec_)){
		getActiveView()->updateAllDocuments();
		emit statusMessage(tr("File save with charset %1").arg(codec_));
	}
}

void DocumentManager::saveWithCharsetAs(const QString& codec_){
	if(getActiveDocument()->saveWithCharsetAs(codec_)){
		getActiveView()->updateAllDocuments();
		emit statusMessage(tr("File save with charset %1").arg(codec_));
	}
}

//==================== bookmarks slots ====================//

void DocumentManager::toggleBookmark(){
	getActiveDocument()->toggleBookmark();
}

void DocumentManager::removeAllBookmarks(){
	getActiveDocument()->removeAllBookmarks();
}

void DocumentManager::nextBookmark(){
	getActiveDocument()->nextBookmark();
}

void DocumentManager::prevBookmark(){
	getActiveDocument()->prevBookmark();
}

//==================== language slots ====================//

void DocumentManager::changeLanguage(const QString& language_){
	getActiveDocument()->setLanguage(language_);
}

//==================== macro slots ====================//

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

QString DocumentManager::getCurrentMacro() const{
	return _macro;
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

				if(_viewList[nb]->count() == 2){
					DocumentEditor* d = _viewList[nb]->getDocument(0);
					if(d->isNew() && !d->isModified())
						_viewList[nb]->closeDocument(0);
				}

				_viewList[nb]->setVisible(true);
			}
			return;
		}
	}
}

void DocumentManager::quickUnidiff(){
	if(getViewNumber() == 2){
		DocumentView* view1 = _viewList[0];
		DocumentView* view2 = _viewList[1];
		DocumentEditor* doc1 = view1->currentDocument();
		DocumentEditor* doc2 = view2->currentDocument();
		Unidiff udiff;
		QString diff = udiff.diff(*doc1, *doc2);
		if(diff.isEmpty()){
			QMessageBox::information(this, PACKAGE_NAME, tr("Files match !\n"));
		}else{
			showUnidiff(diff);
		}
	}else{
		unidiff();
	}
}

void DocumentManager::unidiff(){
	CompareDialog dlg(this);
	if(dlg.exec() == QDialog::Accepted) {
		Unidiff udiff;
		QString diff = udiff.diff(dlg.getSrcFile1(), dlg.getSrcFile2());
		if(diff.isEmpty()){
			QMessageBox::information(this, PACKAGE_NAME, tr("Files match !\n"));
		}else{
			showUnidiff(diff);
		}
	}
}

void DocumentManager::showUnidiff(const QString& diff_){
	UnidiffDialog dlg(diff_, this);
	int ret = dlg.exec();
	if(ret == UnidiffDialog::Open){
		newDocument();
		DocumentEditor* doc = getActiveDocument();
		doc->setText(diff_);
		doc->setLanguage("Diff");
	}else if(ret == UnidiffDialog::Save){
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save Diff"), getActiveDocument()->getPath());
		if (!fileName.isEmpty()){
			QFile file(fileName);
			if (!file.open(QFile::WriteOnly)) {
				QMessageBox::warning(this, PACKAGE_NAME, tr("Cannot save file %1:\n%2.").arg(fileName).arg(file.errorString()));
			}
			QApplication::setOverrideCursor(Qt::WaitCursor);
			QTextStream out(&file);
			out << diff_;
			out.flush();	
			QApplication::restoreOverrideCursor();
		}
	}
}

void DocumentManager::watchedFileChanged(const QString& path_){
	//remove path_ of the filewatcher
	_watcher->removePath(path_);
	//add path to the notification list
	_watcherNotification << path_;
}

void DocumentManager::notify(){
	//FileSystemWatcher notification
	_watcherNotification.removeDuplicates();

	QStringList list = _watcherNotification;
	//clear notification list
	_watcherNotification.clear();

	foreach(QString file, list){
		int ret;
		if (QFile(file).exists()) {
			for(std::vector<DocumentView*>::iterator it = _viewList.begin(); it < _viewList.end(); ++it){
				DocumentEditor* document = (*it)->getDocument(file);
				if(document == 0)
					continue;
				if (document->isNotified() == false) {
					document->setNotified(true);
					_watcher->addPath(file);
					continue;
				}
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
				} else {
					_watcher->addPath(file);
				}
			}
		}
		else {
			ret = QMessageBox::question(this,
						tr("Keep non existing file ?"),
						tr("%1\ndoesn't exist anymore.\nKeep this file in editor?").arg(file),
						QMessageBox::Yes | QMessageBox::No);
			for(std::vector<DocumentView*>::iterator it = _viewList.begin(); it < _viewList.end(); ++it){
				if (ret == QMessageBox::No){
					(*it)->closeDocument(file);
				}else{
					DocumentEditor* document = (*it)->getDocument(file);
					if(document == 0)
						continue;
					(*it)->updateDocumentTab(document, true);
				}
			}
		}
	}
}

#if WAF
#include "document_manager.moc"
#endif

