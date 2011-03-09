#include <QtGui>
#include <QtDebug>

#include "widget/tabbar.h"
#include "document_editor.h"
#include "document_view.h"

DocumentView::DocumentView(QFileSystemWatcher& watcher_, DocumentEditor * document_, QWidget * parent_) : QTabWidget(parent_), _watcher(watcher_){
	//create tabBar
	TabBar* tabbar = new TabBar(this);
	setTabBar(tabbar);
	tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(tabBar(), SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showTabBarContextMenu(QPoint)));
	connect(tabBar(), SIGNAL(tabMoveRequested(QWidget*, int, QWidget*, int)), this, SIGNAL(documentMoveRequested(QWidget*, int, QWidget*, int)));
	//currentChanged signal is not needed anymore since tabClicked handle it
	connect(tabBar(), SIGNAL(tabClicked(int)), this, SLOT(currentTabChanged(int)));
	//connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentTabChanged(int)));
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
	_documentContextMenu = 0;

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
	connect(document_, SIGNAL(textChanged()), this, SLOT(documentChanged()));
	connect(document_, SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
	connect(document_, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(cursorPositionChanged(int, int)));
	connect(document_, SIGNAL(focusChanged(bool)), this, SLOT(setActive(bool)));
}

DocumentEditor* DocumentView::currentDocument(){
	return static_cast<DocumentEditor*>(currentWidget());
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

QList<DocumentEditor*> DocumentView::getDocuments(){
	QList<DocumentEditor*> list;
	for(int i = 0; i < count(); i++)
		list.push_back(getDocument(i));
	return list;
}
QStringList DocumentView::getDocumentNameList(){
	QStringList list;
	for(int i = 0; i < count(); i++)
		list.push_back(getDocument(i)->getFullPath());
	return list;
}

bool DocumentView::setActiveDocument(DocumentEditor* document_){
	for(int i = 0; i < count(); i++){
		DocumentEditor* doc = getDocument(i);
		if(doc == document_){
			setCurrentWidget(doc);
			return true;
		}
	}
	return false;
}
bool DocumentView::setActiveDocument(const QString& name_){
	for(int i = 0; i < count(); i++){
		DocumentEditor* doc = getDocument(i);
		if(doc->getFullPath() == name_){
			setCurrentWidget(doc);
			return true;
		}
	}
	return false;
}

bool DocumentView::isActive() const{
	return _isActive;
}

void DocumentView::setActive(bool active_){
	///NOTE: there is a bug :
	///when the second view is clicked, there is focusInEvent for each view instead of the second one
	///when the first view is clicked, there is focusOutEvent for each view instead of the second one
	///???? Don't know why ????
	///so to go around we just check if the current document has the focus;
	(void)active_;
	//_isActive = active;
	if(currentDocument()->hasFocus()){
		_isActive = true;
	}else{
		_isActive = false;
	}	
	updateAllDocuments();
	emit activeStatusChanged(_isActive);
}

void DocumentView::currentTabChanged(int tab_){
	for(int i = 0; i < count(); i++){
		DocumentEditor* document = getDocument(i);
		if(i == tab_){
			if(!document->getName().isEmpty())
				setTabText(i, document->getName());
			//set the focus on the new selected document	
			document->setFocus();
			//set selected active font
			tabBar()->setTabTextColor(i, QColor(0,0,170));
			//call to documentChanged() may not be necessary
			documentChanged();
		}
		else{
			//set normal tab font
			tabBar()->setTabTextColor(i, QColor(80,80,80));
		}
	}
}

void DocumentView::documentChanged(){
	DocumentEditor* document = currentDocument();
	int index = currentIndex();
	//update tab icon
	if(document->isModified() || (document->stillExist() == false))
		setTabIcon(index, QIcon(":/images/unsaved.png"));
	else
		setTabIcon(index, QIcon(":/images/saved.png"));	
	//update tab text
	if(!document->getName().isEmpty())
		setTabText(index, document->getName());
	emit documentChanged(currentDocument());
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
	DocumentEditor* document = new DocumentEditor(this);
	connectDocument(document);
	addTab(document, QIcon(":/images/saved.png"), tr("new %1").arg(_docCounter++));
	setCurrentWidget(document);
	documentChanged();
}

void DocumentView::addDocument(DocumentEditor* document_){
	disconnect(document_);
	document_->setParent(this);

	QString docName = document_->getName();
	if(docName.isEmpty())
		docName = tr("new %1").arg(_docCounter++);
	else
		_watcher.addPath(document_->getFullPath());
	
	if(document_->isModified())
		addTab(document_, QIcon(":/images/unsaved.png"), docName);
	else
		addTab(document_, QIcon(":/images/saved.png"), docName);

	connectDocument(document_);
	removeFirstNewDocument();
	setCurrentWidget(document_);
	documentChanged();
}

void DocumentView::insertDocument(int index_, DocumentEditor* document_){
	disconnect(document_);
	document_->setParent(this);

	QString docName = document_->getName();
	if(docName.isEmpty())
		docName = tr("new %1").arg(_docCounter++);
	else
		_watcher.addPath(document_->getFullPath());

	if(document_->isModified())
		insertTab(index_, document_, QIcon(":/images/unsaved.png"), docName);
	else
		insertTab(index_, document_, QIcon(":/images/saved.png"), docName);
	connectDocument(document_);
	removeFirstNewDocument();
	setCurrentWidget(document_);
	documentChanged();
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

	setCurrentWidget(doc);
	documentChanged();
}

void DocumentView::openDocument(const QString& file_){
	DocumentEditor* document = new DocumentEditor(this);
	connectDocument(document);

	if(document->load(file_)){		
		_watcher.addPath(document->getFullPath());
		addTab(document, QIcon(":/images/saved.png"), document->getName());
		removeFirstNewDocument();
		setCurrentWidget(document);
		documentChanged();
	}else{
		delete document;
	}
}
void DocumentView::openDocument(const QStringList& files_){
	for(int i = 0; i < files_.size(); i++){
		DocumentEditor* document = new DocumentEditor(this);
		connectDocument(document);

		if(document->load(files_[i])){
			_watcher.addPath(document->getFullPath());
			addTab(document, QIcon(":/images/saved.png"), document->getName());
			removeFirstNewDocument();
			setCurrentWidget(document);
		}else{
			delete document;
		}
	}
	documentChanged();
}

bool DocumentView::closeCurrentDocument(){
	return closeDocument(currentIndex());
}
bool DocumentView::closeDocument(int index_){
	DocumentEditor* document = getDocument(index_);
    if(count() == 1){
		if(document->maybeSave()){
			newDocument();
			_watcher.removePath(document->getFullPath());
			delete document;
			emit empty();
			return true;
		}
		else
			return false;
	}
	else{
		if(document->maybeSave()){
			_watcher.removePath(document->getFullPath());
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
	_watcher.removePath(document->getFullPath());
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
	DocumentEditor* document = currentDocument();
	_watcher.removePath(document->getFullPath());
	document->save();
	documentChanged();
	_watcher.addPath(document->getFullPath());
}
void DocumentView::saveAs(){
	DocumentEditor* document = currentDocument();
	_watcher.removePath(document->getFullPath());
	currentDocument()->saveAs();
	documentChanged();
	_watcher.addPath(document->getFullPath());
}
void DocumentView::saveACopyAs(){
	currentDocument()->saveACopyAs();
}
void DocumentView::saveAll(){
	for(int i = 0; i < count(); i++){
		DocumentEditor* document = getDocument(i);
		_watcher.removePath(document->getFullPath());
		document->save();
		_watcher.addPath(document->getFullPath());
	}
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

void DocumentView::setContextMenu(QMenu* menu_){
	_documentContextMenu = menu_;
}

void DocumentView::showTabBarContextMenu(const QPoint &point_){
	QTabBar *tabBar = this->tabBar();
	for (int i = 0; i < tabBar->count(); ++i) {
		if (tabBar->tabRect(i).contains(point_)){
			tabBar->setCurrentIndex(i);
			if(_documentContextMenu != 0)
				_documentContextMenu->exec(QCursor::pos());
		}
	}
}
