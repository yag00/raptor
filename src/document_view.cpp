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
#include <QtDebug>

#include "widget/tabbar.h"
#include "document_editor.h"
#include "document_view.h"

DocumentView::DocumentView(QFileSystemWatcher& watcher_, DocumentEditor * document_, QWidget * parent_) : QTabWidget(parent_), _watcher(watcher_){
	setAcceptDrops(true);
	//create tabBar
	TabBar* tabbar = new TabBar(this);
	setTabBar(tabbar);
	tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(tabBar(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTabBarContextMenu(QPoint)));
	connect(tabBar(), SIGNAL(tabMoveRequested(QWidget*, int, QWidget*, int)), this, SIGNAL(documentMoveRequested(QWidget*, int, QWidget*, int)));
	//currentChanged signal is not needed anymore since tabClicked handle it
	connect(tabBar(), SIGNAL(tabClicked(int)), this, SIGNAL(currentChanged(int)));
	connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
	connect(tabBar(), SIGNAL(tabMiddleClicked(int)), this, SLOT(closeDocument(int)));
	connect(tabBar(), SIGNAL(tabBarDoubleClicked()), this, SLOT(newDocument()));

	//create tabBar button
	QToolButton *newTabButton = new QToolButton(this);
	newTabButton->setAutoRaise(true);
	newTabButton->setToolTip(tr("Add new page"));
	newTabButton->setIcon(QIcon(":/images/addtab.png"));
	setCornerWidget(newTabButton, Qt::TopLeftCorner);
	connect(newTabButton, SIGNAL(clicked()), this, SLOT(newDocument()));

	QToolButton *closeTabButton = new QToolButton(this);
	closeTabButton->setAutoRaise(true);
	closeTabButton->setEnabled(true);
	closeTabButton->setToolTip(tr("Close current page"));
	closeTabButton->setIcon(QIcon(":/images/closetab.png"));
	setCornerWidget(closeTabButton, Qt::TopRightCorner);
	connect(closeTabButton, SIGNAL(clicked()), this, SLOT(closeCurrentDocument()));


	//initialize data
	_isActive = false;
	_docCounter = 0;
	_tabContextMenu = 0;
	
	QMenu *menuBar = new QMenu(this);
	menuBar->setObjectName("TabBarContextMenu");
	QAction* newTab = menuBar->addAction(QIcon(":/images/new.png"), tr("New"));
	connect(newTab, SIGNAL(triggered()), this, SLOT(newDocument()));
	_reopen = menuBar->addAction(QIcon(":/images/revert.png"), tr("Reopen last file closed"));
	_reopen->setEnabled(!_lastClosed.isEmpty());
	connect(_reopen, SIGNAL(triggered()), this, SLOT(reopen()));
	_tabbarContextMenu = menuBar;
	
	setDocumentMode(true);
	setMovable(true);
	setIconSize(QSize(12,12));
	//setTabsClosable(false);

	//create the first document
	if(document_ == 0)
		newDocument();
	else
		addDocument(document_);
}
DocumentView::~DocumentView(){

}

void DocumentView::connectDocument(DocumentEditor* document_){
	connect(document_, SIGNAL(modificationChanged(bool)), this, SLOT(modificationChanged(bool)));
	connect(document_, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
	connect(document_, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(cursorPositionChanged(int, int)));
	connect(document_, SIGNAL(focusChanged(bool)), this, SLOT(documentfocusChanged(bool)));
	connect(document_, SIGNAL(fileDropped(QStringList)), this, SLOT(openDocument(QStringList)));
}

DocumentEditor* DocumentView::currentDocument(){
	return static_cast<DocumentEditor*>(currentWidget());
}

void DocumentView::setCurrentDocument(DocumentEditor* document_){
	setCurrentWidget(document_);
	emit activeDocumentChanged(document_);
}

DocumentEditor* DocumentView::getDocument(int index_){
	return static_cast<DocumentEditor*>(widget(index_));
}
DocumentEditor* DocumentView::getDocument(const QString& name_){
	for(int i = 0; i < count(); i++){
		DocumentEditor* doc = getDocument(i);
		if(doc->getFullPath() == name_)
			return doc;
	}
	return 0;
}

int DocumentView::getDocumentIndex(const QString& name_){
	for(int i = 0; i < count(); i++){
		DocumentEditor* doc = getDocument(i);
		if(doc->getFullPath() == name_)
			return i;
	}
	return -1;
}
int DocumentView::getDocumentIndex(const DocumentEditor* document_){
	for(int i = 0; i < count(); i++){
		DocumentEditor* doc = getDocument(i);
		if(doc == document_)
			return i;
	}
	return -1;
}

QList<DocumentEditor*> DocumentView::getDocuments(){
	QList<DocumentEditor*> list;
	for(int i = 0; i < count(); i++)
		list.push_back(getDocument(i));
	return list;
}
QStringList DocumentView::getDocumentNameList(){
	QStringList list;
	for(int i = 0; i < count(); i++){
		QString path = getDocument(i)->getFullPath();
		if(!path.isEmpty())
			list.push_back(path);
	}
	return list;
}

bool DocumentView::setActiveDocument(DocumentEditor* document_){
	for(int i = 0; i < count(); i++){
		DocumentEditor* doc = getDocument(i);
		if(doc == document_){
			setCurrentDocument(doc);
			return true;
		}
	}
	return false;
}
bool DocumentView::setActiveDocument(const QString& name_){
	for(int i = 0; i < count(); i++){
		DocumentEditor* doc = getDocument(i);
		if(doc->getFullPath() == name_){
			setCurrentDocument(doc);
			return true;
		}
	}
	return false;
}


void DocumentView::documentfocusChanged(bool active_){
	///FIXME @todo there is a bug :
	///when the second view is clicked, there is focusInEvent for each view instead of the second one
	///when the first view is clicked, there is focusOutEvent for each view instead of the second one
	///???? Don't know why ????
	///so to go around we just check if the current document has the focus;
	(void)active_;
	//_isActive = active;
	if(currentDocument()->hasFocus()){
		_isActive = true;
		activeDocumentChanged(qobject_cast<DocumentEditor*>(sender()));
	}else{
		_isActive = false;
	}

	updateAllDocuments();
	emit activeStatusChanged(_isActive);
}

void DocumentView::currentTabChanged(int tab_){
	DocumentEditor* document = getDocument(tab_);
	setCurrentIndex(tab_);
	document->setFocus();
	emit activeDocumentChanged(document);
}

void DocumentView::modificationChanged(bool modified_){
	DocumentEditor* document = qobject_cast<DocumentEditor*>(sender());
	updateDocumentTab(document, modified_);
	emit activeDocumentChanged(document);
}

void DocumentView::updateDocumentTab(DocumentEditor* document_, bool modified_){
	//update tab icon
	int index = getDocumentIndex(document_);
	if(modified_ || (document_->stillExist() == false))
		setTabIcon(index, QIcon(":/images/unsaved.png"));
	else if(document_->isReadOnly())
		setTabIcon(index, QIcon(":/images/readonly.png"));
	else
		setTabIcon(index, QIcon(":/images/saved.png"));

	//update tab text
	if(!document_->getName().isEmpty()){
		setTabText(index, document_->getName());
	}
}

void DocumentView::selectionChanged(){
	emit selectionChanged(currentDocument());
}

void DocumentView::cursorPositionChanged(int line, int col){
	emit cursorPositionChanged(currentDocument(), line, col);
}

void DocumentView::updateAllDocuments(){
	for(int i = 0; i < count(); i++){
		DocumentEditor* document = getDocument(i);
		if(i == currentIndex()){
			if(document->isModified() || (document->stillExist() == false))
				setTabIcon(i, QIcon(":/images/unsaved.png"));
			else if(document->isReadOnly())
				setTabIcon(i, QIcon(":/images/readonly.png"));
			else
				setTabIcon(i, QIcon(":/images/saved.png"));

			if(!document->getName().isEmpty())
				setTabText(i, document->getName());

			if(_isActive == true){
				//set selected active font
				tabBar()->setTabTextColor(i, QColor(0,0,170));
			}
			else{
				//set selected unactive font
				tabBar()->setTabTextColor(i, QColor(128,128,128));
			}
		}
		else{
			//set normal tab font
			tabBar()->setTabTextColor(i, QColor(80,80,80));
		}
	}
}

void DocumentView::newDocument(){
	DocumentEditor* document = new DocumentEditor(_watcher, this);
	connectDocument(document);
	addTab(document, QIcon(":/images/saved.png"), tr("new %1").arg(_docCounter++));
	setCurrentDocument(document);
}

void DocumentView::addDocument(DocumentEditor* document_){
	disconnect(document_);
	document_->setParent(this);

	QString docName = document_->getName();
	if(docName.isEmpty())
		docName = tr("new %1").arg(_docCounter++);

	if(document_->isModified())
		addTab(document_, QIcon(":/images/unsaved.png"), docName);
	else
		addTab(document_, QIcon(":/images/saved.png"), docName);

	connectDocument(document_);
	removeFirstNewDocument();
	setCurrentDocument(document_);
}

void DocumentView::insertDocument(int index_, DocumentEditor* document_){
	disconnect(document_);
	document_->setParent(this);

	QString docName = document_->getName();
	if(docName.isEmpty())
		docName = tr("new %1").arg(_docCounter++);

	if(document_->isModified())
		insertTab(index_, document_, QIcon(":/images/unsaved.png"), docName);
	else
		insertTab(index_, document_, QIcon(":/images/saved.png"), docName);
	connectDocument(document_);
	removeFirstNewDocument();
	setCurrentDocument(document_);
}

void DocumentView::cloneDocument(DocumentEditor* document_){
	DocumentEditor* doc = new DocumentEditor(document_, this);
	QString docName = doc->getName();
	if(docName.isEmpty())
		docName = tr("new %1").arg(_docCounter++);

	if(doc->isModified())
		addTab(doc, QIcon(":/images/unsaved.png"), docName);
	else
		addTab(doc, QIcon(":/images/saved.png"), docName);

	connectDocument(doc);

	setCurrentDocument(doc);
}

void DocumentView::openDocument(const QString& file_){
	DocumentEditor* document = new DocumentEditor(_watcher, this);
	connectDocument(document);

	if(document->load(file_)){
		addTab(document, QIcon(":/images/saved.png"), document->getName());
		removeFirstNewDocument();
		setCurrentDocument(document);
		QStringList openedFiles;
		openedFiles << file_;
		emit opened(openedFiles);
	}else{
		delete document;
	}
}
void DocumentView::openDocument(const QStringList& files_){
	QStringList openedFiles;
	for(int i = 0; i < files_.size(); i++){
		DocumentEditor* document = new DocumentEditor(_watcher, this);
		connectDocument(document);

		if(document->load(files_[i])){
			openedFiles << files_[i];
			addTab(document, QIcon(":/images/saved.png"), document->getName());
			removeFirstNewDocument();
			setCurrentDocument(document);
		}else{
			delete document;
		}
	}
	emit opened(openedFiles);
}

void DocumentView::reopen(){
	QString file = _lastClosed.back();
	_lastClosed.pop_back();
	_reopen->setEnabled(!_lastClosed.isEmpty());
	openDocument(file);
}

bool DocumentView::closeCurrentDocument(){
	return closeDocument(currentIndex());
}
bool DocumentView::closeDocument(int index_){
	DocumentEditor* document = getDocument(index_);
    if(count() == 1){
		if(document->maybeSave()){
			newDocument();
			//update closed history
			if(!document->getFullPath().isEmpty()){
				_lastClosed.push_back(document->getFullPath());
				if(_lastClosed.size() > _lifoSize)
					_lastClosed.pop_front();
				_reopen->setEnabled(!_lastClosed.isEmpty());
			}
			//close the file
			delete document;
			emit empty();
			return true;
		}
		else
			return false;
	}
	else{
		if(document->maybeSave()){
			//update closed history
			if(!document->getFullPath().isEmpty()){
				_lastClosed.push_back(document->getFullPath());
				if(_lastClosed.size() > _lifoSize)
					_lastClosed.pop_front();
				_reopen->setEnabled(!_lastClosed.isEmpty());
			}
			//close the file			
			delete document;
			return true;
		}
		else
			return false;
	}
}

bool DocumentView::closeDocument(const QString& file_){
	int index = getDocumentIndex(file_);
	if(index < 0)
		return false;
	return closeDocument(index);
}

void DocumentView::closeAllDocumentsExceptCurrent(){
	int i = 0;
	while (count() != 1){
		if(i != currentIndex())
			closeDocument(i);
		else
			i++;
	}
}
bool DocumentView::closeAll(){
	bool ret = true;
	int nbTab = count() - 1;
	for(int i = nbTab; i >= 0; i--){
		if(closeDocument(i) == false)
			ret = false;
	}
	return ret;
}

DocumentEditor* DocumentView::removeDocumentAt(int index_){
	bool emitEmptySignal = false;
	if(count() == 1){
		newDocument();
		emitEmptySignal = true;
	}
	DocumentEditor* document = getDocument(index_);
	disconnect(document);
	removeTab(index_);
	//note: this do a remove tab but i'm not sure it's clean
	//and removeTab problaly setParent to 0 (verify this ...)
	document->setParent(0);
	if(emitEmptySignal == true)
		emit empty();
	return document;
}

DocumentEditor* DocumentView::removeCurrentDocument(){
	return removeDocumentAt(currentIndex());
}

void DocumentView::save(){
	currentDocument()->save();
	emit saved((QStringList() << currentDocument()->getFullPath()));
}
void DocumentView::saveAs(){
	currentDocument()->saveAs();
	emit saved((QStringList() << currentDocument()->getFullPath()));
}
void DocumentView::saveACopyAs(){
	currentDocument()->saveACopyAs();
}
void DocumentView::saveAll(){
	QStringList list;
	for(int i = 0; i < count(); i++){
		DocumentEditor* document = getDocument(i);
		document->save();
		list << document->getFullPath();
	}
	emit saved(list);
	updateAllDocuments();
}

void DocumentView::print(){
	currentDocument()->print();
}

void DocumentView::removeFirstNewDocument(){
	if(count() == 2){
		DocumentEditor* currentDoc = currentDocument();
		if(currentDoc->isNew() && !currentDoc->isModified()){
			delete currentDoc;
		}
	}
}

bool DocumentView::documentExists(const QString& file_){
	for(int i = 0; i < count(); i++){
		if(file_ == getDocument(i)->getFullPath())
			return true;
	}
	return false;
}

void DocumentView::setContextMenu(QMenu* tabContextMenu_){
	_tabContextMenu = tabContextMenu_;
}

void DocumentView::showTabBarContextMenu(const QPoint &point_){
	QTabBar *tabBar = this->tabBar();
	for (int i = 0; i < tabBar->count(); ++i) {
		if (tabBar->tabRect(i).contains(point_)){
			tabBar->setCurrentIndex(i);
			if(_tabContextMenu != 0)
				_tabContextMenu->exec(QCursor::pos());
			return;
		}
	}
	//not in tab, display the tabbar context menu
	_tabbarContextMenu->exec(QCursor::pos());
}

void DocumentView::dragEnterEvent(QDragEnterEvent *event_){
	event_->acceptProposedAction();
}

void DocumentView::dropEvent(QDropEvent *event_){
	const QMimeData* mimeData = event_->mimeData();
	// check for our needed mime type, here a file or a list of files
	if (mimeData->hasUrls()) {
		QStringList pathList;
		QList<QUrl> urlList = mimeData->urls();
 
		// extract the local paths of the files
		for (int i = 0; i < urlList.size() && i < 32; ++i) {
			pathList.append(urlList.at(i).toLocalFile());
		}
 
		openDocument(pathList);
		event_->acceptProposedAction();  
	}
}

#if WAF
#include "document_view.moc"
#endif

