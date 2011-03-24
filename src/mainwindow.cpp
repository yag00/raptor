#include <QtGui>
#include <Qsci/qsciscintilla.h>

#include <assert.h>

#include "about/About.h"
#include "ui_macro.h"
#include "settings/settings_dialog.h"
#include "search.h"
#include "document_editor.h"
#include "document_view.h"
#include "document_manager.h"
#include "lexer/lexer_manager.h"

#include "mainwindow.h"

MainWindow::MainWindow() {
	//init ui
	setupUi(this);

	//create manager
	createDocumentManager();

	//initialize menu
	initMenu();

	//create status bar
	createStatusBar();

	//read Settings
	readSettings();

	//if launch from console open arg document ...
	///@todo dislay usage if arg == --help
	///@todo dislay version if arg == --version || -v
	QStringList args = QCoreApplication::arguments();
	args.pop_front();
	if(args.count() > 0) {
		_documentManager->openDocument(args);
		updateRecentFile(args);
	}
}

MainWindow::~MainWindow() {
	LexerManager::getInstance().destroy();
}

void MainWindow::initMenu() {
	initMenuFile();
	initMenuEdit();
	initMenuView();
	initMenuSearch();
	initMenuEncoding();
	initMenuBookmarks();
	initMenuLanguage();
	initMenuMacro();
	initMenuSettings();
	initMenuHelp();
}

void MainWindow::initMenuFile() {
	//connect menu "File" Action
	connect(actionNew, SIGNAL(triggered()), this, SLOT(fileNew()));
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));
	connect(actionSaveAs, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
	connect(actionSaveACopyAs, SIGNAL(triggered()), this, SLOT(fileSaveACopyAs()));
	connect(actionSaveAll, SIGNAL(triggered()), this, SLOT(fileSaveAll()));
	connect(actionClose, SIGNAL(triggered()), this, SLOT(fileClose()));
	connect(actionCloseAll, SIGNAL(triggered()), this, SLOT(fileCloseAll()));
	connect(actionCloseAllExceptCurrentDocument, SIGNAL(triggered()), this, SLOT(fileCloseAllExceptCurrentDocument()));
	connect(actionSessionSave, SIGNAL(triggered()), _documentManager, SLOT(saveSession()));
	connect(actionSessionRestore, SIGNAL(triggered()), _documentManager, SLOT(restoreSession()));
	connect(actionReload, SIGNAL(triggered()), this, SLOT(fileReload()));
	connect(actionPrint, SIGNAL(triggered()), this, SLOT(filePrint()));
	connect(actionExit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
	//recent file actions
	connect(actionEmptyRecentFilesList, SIGNAL(triggered()), this, SLOT(clearRecentFile()));
	connect(actionOpenAllRecentFiles, SIGNAL(triggered()), this, SLOT(openAllRecentFile()));
	_recentFileSeparator = menuRecentFiles->addSeparator();
	for (int i = 0; i < MaxRecentFiles; ++i) {
		_recentFileActions[i] = new QAction(this);
		_recentFileActions[i]->setVisible(false);
		connect(_recentFileActions[i], SIGNAL(triggered()),this, SLOT(openRecentFile()));
		menuRecentFiles->addAction(_recentFileActions[i]);
	}
	updateRecentFileActions();
}

void MainWindow::initMenuEdit() {
	//connect menu "Edit" Action
	connect(actionCut, SIGNAL(triggered()), this, SLOT(editCut()));
	connect(actionCopy, SIGNAL(triggered()), this, SLOT(editCopy()));
	connect(actionPaste, SIGNAL(triggered()), this, SLOT(editPaste()));
	connect(actionUndo, SIGNAL(triggered()), this, SLOT(editUndo()));
	connect(actionRedo, SIGNAL(triggered()), this, SLOT(editRedo()));
	connect(actionSelectAll, SIGNAL(triggered()), this, SLOT(editSelectAll()));

	connect(actionIndent, SIGNAL(triggered()), this, SLOT(increaseIndentation()));
	connect(actionUnindent, SIGNAL(triggered()), this, SLOT(decreaseIndentation()));
	connect(actionAutoIndentation, SIGNAL(toggled(bool)), this, SLOT(setAutoIndentation(bool)));
	connect(actionIndentationGuides, SIGNAL(toggled(bool)), this, SLOT(showIndentationGuides(bool)));

	connect(actionTrimTrailingSpaces, SIGNAL(triggered()), this, SLOT(doTrimTrailing()));
	connect(actionUpperCase, SIGNAL(triggered()), this, SLOT(convertSelectedTextToUpperCase()));
	connect(actionLowerCase, SIGNAL(triggered()), this, SLOT(convertSelectedTextToLowerCase()));
	connect(actionDuplicateLine, SIGNAL(triggered()), this, SLOT(duplicateCurrentLine()));
	connect(actionCopyLine, SIGNAL(triggered()), this, SLOT(copyCurrentLine()));
	connect(actionCutLine, SIGNAL(triggered()), this, SLOT(cutCurrentLine()));
	connect(actionDeleteLine, SIGNAL(triggered()), this, SLOT(deleteCurrentLine()));
	connect(actionMoveLineUp, SIGNAL(triggered()), this, SLOT(moveCurrentLineUp()));
	connect(actionMoveLineDown, SIGNAL(triggered()), this, SLOT(moveCurrentLineDown()));
	connect(actionDeleteWord, SIGNAL(triggered()), this, SLOT(deleteCurrentWord()));

	connect(actionReadOnly, SIGNAL(triggered(bool)), this, SLOT(setReadOnly(bool)));
	connect(actionReindentFile, SIGNAL(triggered()), _documentManager, SLOT(reindentDocument()));
	connect(actionReindentOpenFiles, SIGNAL(triggered()), _documentManager, SLOT(reindentOpenDocuments()));
	
	connect(menuEdit, SIGNAL(aboutToShow()), this, SLOT(aboutToShowEditMenu()));
	connect(menuIndentation, SIGNAL(aboutToShow()), this, SLOT(aboutToShowEditIndentationMenu()));
}

void MainWindow::initMenuView() {
	//connect menu "View" Action
	connect(actionZoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));
	connect(actionZoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));
	connect(actionRestoreDefaultZoom, SIGNAL(triggered()), this, SLOT(zoomRestore()));
	connect(actionShowWhitespaceAndTab, SIGNAL(triggered(bool)), this, SLOT(showWhiteSpaceAndTab(bool)));
	connect(actionShowEndOfLine, SIGNAL(triggered(bool)), this, SLOT(showEndOfLine(bool)));
	connect(actionShowAll, SIGNAL(triggered(bool)), this, SLOT(showAll(bool)));
	connect(actionFoldAll, SIGNAL(triggered()), this, SLOT(foldUnfoldAll()));

	connect(actionFoldLevel_1, SIGNAL(triggered()), this, SLOT(foldLevel()));
	connect(actionFoldLevel_2, SIGNAL(triggered()), this, SLOT(foldLevel()));
	connect(actionFoldLevel_3, SIGNAL(triggered()), this, SLOT(foldLevel()));
	connect(actionFoldLevel_4, SIGNAL(triggered()), this, SLOT(foldLevel()));
	connect(actionFoldLevel_5, SIGNAL(triggered()), this, SLOT(foldLevel()));
	connect(actionFoldLevel_6, SIGNAL(triggered()), this, SLOT(foldLevel()));
	connect(actionFoldLevel_7, SIGNAL(triggered()), this, SLOT(foldLevel()));
	connect(actionFoldLevel_8, SIGNAL(triggered()), this, SLOT(foldLevel()));
	connect(actionUnfoldLevel_1, SIGNAL(triggered()), this, SLOT(unfoldLevel()));
	connect(actionUnfoldLevel_2, SIGNAL(triggered()), this, SLOT(unfoldLevel()));
	connect(actionUnfoldLevel_3, SIGNAL(triggered()), this, SLOT(unfoldLevel()));
	connect(actionUnfoldLevel_4, SIGNAL(triggered()), this, SLOT(unfoldLevel()));
	connect(actionUnfoldLevel_5, SIGNAL(triggered()), this, SLOT(unfoldLevel()));
	connect(actionUnfoldLevel_6, SIGNAL(triggered()), this, SLOT(unfoldLevel()));
	connect(actionUnfoldLevel_7, SIGNAL(triggered()), this, SLOT(unfoldLevel()));
	connect(actionUnfoldLevel_8, SIGNAL(triggered()), this, SLOT(unfoldLevel()));
	
	connect(menuInvisibleSymbol, SIGNAL(aboutToShow()), this, SLOT(aboutToShowViewInvisibleSymbolMenu()));
}

void MainWindow::initMenuSearch() {
	//connect memu "Search" Action
	connect(actionSearch, SIGNAL(triggered()), createSearchDock(), SLOT(trigger()));
}

void MainWindow::initMenuEncoding() {
	//connect menu "Encoding" Action
	connect(actionConvertToWindowsFormat, SIGNAL(triggered()), this, SLOT(convertToWindowFormat()));
	connect(actionConvertToUnixFormat, SIGNAL(triggered()), this, SLOT(convertToUnixFormat()));
	connect(actionConvertToMacFormat, SIGNAL(triggered()), this, SLOT(convertToMacFormat()));

	//codec list
	QMap<QString, QTextCodec *> codecMap;
	QRegExp iso8859RegExp("ISO[- ]8859-([0-9]+).*");

	foreach (int mib, QTextCodec::availableMibs()) {
		QTextCodec *codec = QTextCodec::codecForMib(mib);

		QString sortKey = codec->name().toUpper();
		int rank;

		if (sortKey.startsWith("UTF-8")) {
			rank = 1;
		} else if (sortKey.startsWith("UTF-16")) {
			rank = 2;
		} else if (iso8859RegExp.exactMatch(sortKey)) {
			if (iso8859RegExp.cap(1).size() == 1)
				rank = 3;
			else
				rank = 4;
		} else {
			rank = 5;
		}
		sortKey.prepend(QChar('0' + rank));

		codecMap.insert(sortKey, codec);
	}

	//qDebug() << Settings::availableTextCodecs();
	
	//initialize menu
	QActionGroup* useCharsetActionGroup = new QActionGroup(menuUseCharset);
	QActionGroup* saveWithCharsetActionGroup = new QActionGroup(menuSaveWithCharset);
	QActionGroup* saveWithCharsetAsActionGroup = new QActionGroup(menuSaveWithCharsetAs);
	foreach(QTextCodec* codec, codecMap.values()){
		//menu use charset
		QAction* action = menuUseCharset->addAction(codec->name());
		action->setCheckable(true);
		useCharsetActionGroup->addAction(action);
		//menu saveWithCharset
		action = menuSaveWithCharset->addAction(codec->name());
		saveWithCharsetActionGroup->addAction(action);
		//menu saveWithCharset
		action = menuSaveWithCharsetAs->addAction(codec->name());
		saveWithCharsetAsActionGroup->addAction(action);		
	}
	connect(useCharsetActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeCharset(QAction*)));
	connect(saveWithCharsetActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(saveWithCharset(QAction*)));
	connect(saveWithCharsetAsActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(saveWithCharsetAs(QAction*)));
	
	connect(menuEncoding, SIGNAL(aboutToShow()), this, SLOT(aboutToShowEncodingMenu()));
	connect(menuUseCharset, SIGNAL(aboutToShow()), this, SLOT(aboutToShowChangeCharsetMenu()));
	connect(menuSaveWithCharset, SIGNAL(aboutToShow()), this, SLOT(aboutToShowSaveWithCharsetMenu()));
	connect(menuSaveWithCharsetAs, SIGNAL(aboutToShow()), this, SLOT(aboutToShowSaveWithCharsetAsMenu()));
}

void MainWindow::initMenuBookmarks() {
	//connect menu "Bookmarks" Action
	connect(actionToggleBookmark, SIGNAL(triggered()), this, SLOT(toggleBookmark()));
	connect(actionRemoveAllBookmarks, SIGNAL(triggered()), this, SLOT(removeAllBookmarks()));
	connect(actionNextBookmark, SIGNAL(triggered()), this, SLOT(nextBookmark()));
	connect(actionPreviousBookmark, SIGNAL(triggered()), this, SLOT(prevBookmark()));
}

void MainWindow::initMenuLanguage() {
	//create lexer manager and init menuLanguage
	LexerManager::getInstance().initialize(menuLanguage);
	//connect menu "Language" Action
	LexerManager::getInstance().connectTo(this, SLOT(changeLanguage(QAction*)));
	
	connect(menuLanguage, SIGNAL(aboutToShow()), this, SLOT(aboutToShowLanguageMenu()));
}

void MainWindow::initMenuDiff() {
	//connect menu "Diff" Action
	connect(actionDiff, SIGNAL(triggered()), this, SLOT(diff()));
}

void MainWindow::initMenuMacro() {
	//connect menu "Macro" Action
	connect(actionStartRecording, SIGNAL(triggered()), this, SLOT(startRecordingMacro()));
	connect(actionStopRecording, SIGNAL(triggered()), this, SLOT(stopRecordingMacro()));
	connect(actionRunMacro, SIGNAL(triggered()), this, SLOT(runMacro()));
	connect(actionRunMacroMultipleTimes, SIGNAL(triggered()), this, SLOT(runMacroMultipleTimes()));
	//connect(actionSaveMacro, SIGNAL(triggered()), this, SLOT(saveMacro()));
	//connect(actionLoadMacro, SIGNAL(triggered()), this, SLOT(loadMacro()));
}

void MainWindow::initMenuSettings() {
	//connect menu "Settings" Action
	connect(actionSettings, SIGNAL(triggered()), this, SLOT(settings()));
}

void MainWindow::initMenuHelp() {
	//connect menu "Help" Action
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createDocumentManager() {
	_documentManager = new DocumentManager(this);
	setCentralWidget(_documentManager);

	connect(_documentManager, SIGNAL(documentChanged(DocumentEditor*)), this, SLOT(update(DocumentEditor*)));
	connect(_documentManager, SIGNAL(selectionChanged(DocumentEditor*)), this, SLOT(update(DocumentEditor*)));
	connect(_documentManager, SIGNAL(cursorPositionChanged(DocumentEditor*, int, int)), this, SLOT(update(DocumentEditor*)));
}

void MainWindow::createStatusBar() {
	/*_insLabel = new QLabel("INS", this);
	_insLabel->setFrameShape(QFrame::Panel);
	_insLabel->setFrameShadow(QFrame::Sunken);
	_majLabel = new QLabel("MAJ", this);
	_majLabel->setFrameShape(QFrame::Panel);
	_majLabel->setFrameShadow(QFrame::Sunken);*/
	_encodingLabel = new QLabel("??", this);
	_formatLabel = new QLabel(this);
	_editInfoLabel = new QLabel(this);
	_fileInfoLabel = new QLabel(this);
	_fileTypeLabel = new QLabel(this);

	statusBar()->addWidget(_fileTypeLabel, 7);
	statusBar()->addWidget(_fileInfoLabel, 2);
	statusBar()->addWidget(_editInfoLabel, 3);
	statusBar()->addPermanentWidget(_encodingLabel);
	statusBar()->addPermanentWidget(_formatLabel);
	/*statusBar()->addPermanentWidget(_majLabel);
	statusBar()->addPermanentWidget(_insLabel);*/
}

QAction* MainWindow::createSearchDock() {
	_searchDock = new QDockWidget(tr("Search/Replace"), this);
	Search* search = new Search(_documentManager, _searchDock);
	_searchDock->setWidget(search);
	addDockWidget(Qt::BottomDockWidgetArea, _searchDock);

	connect(_searchDock, SIGNAL(visibilityChanged(bool)), search, SLOT(setVisible(bool)));
	connect(actionFindNext, SIGNAL(triggered()), search, SLOT(next()));
	connect(actionFindPrevious, SIGNAL(triggered()), search, SLOT(prev()));

	_searchDock->hide();
	return _searchDock->toggleViewAction();
}

DocumentManager& MainWindow::getDocumentManager() {
	return *_documentManager;
}

void MainWindow::handleMessage(const QString& message_) {
	QStringList files = message_.split(";");
	files.removeAll("");
	if(!files.empty()) {
		_documentManager->openDocument(files);
		updateRecentFile(files);
	}
}

void MainWindow::update(DocumentEditor* document_) {
	//update main window title
	updateMainWindowTitle(document_);
	//update status bar
	updateStatusBar(document_);
	//update actions
	updateActions(document_);
}
void MainWindow::updateMainWindowTitle(DocumentEditor* document_) {
	QString docName = document_->getFullPath();
	if(docName.isEmpty())
		docName = _documentManager->getActiveView()->tabText(_documentManager->getActiveView()->currentIndex());

	setWindowTitle(tr("[*]%1 - %2").arg( PACKAGE_NAME ).arg(docName));
	setWindowModified(document_->isModified());
}
void MainWindow::updateStatusBar(DocumentEditor* document_) {
	int nbChars = document_->text().length();
	int nbLines = document_->lines();
	int line, column;
	document_->getCursorPosition(&line, &column);
	int select = document_->selectedText().length();

	_editInfoLabel->setText(tr("Line : %1  Col : %2  Sel : %3     ").arg(line+1).arg(column).arg(select));
	_fileInfoLabel->setText(tr("%1 chars  %2 lines     ").arg(nbChars).arg(nbLines));
	_encodingLabel->setText(document_->getCodec());
	//_formatLabel->setText(document->getFormat() + "     ");
	_formatLabel->setPixmap(document_->getFormatPixmap().scaled(16,16));
	_fileTypeLabel->setText(document_->getType());
}
void MainWindow::updateActions(DocumentEditor* document_) {
	//update all actions on the tool bar always visibe
	//other actions will be update by aboutToShow slots ....
	
	//undo/redo action
	actionUndo->setEnabled(document_->isUndoAvailable());
	actionRedo->setEnabled(document_->isRedoAvailable());

	//show all invisible symbol
	bool wsIsVisible = (document_->whitespaceVisibility() == QsciScintilla::WsVisible);
	bool eolIsVisible = document_->eolVisibility();
	actionShowAll->setChecked(wsIsVisible & eolIsVisible);	
}


void MainWindow::closeEvent(QCloseEvent *event_) {
	writeSettings();
	if(fileCloseAll() == true) {
		event_->accept();
	}
	else
		event_->ignore();
}

void MainWindow::updateRecentFileActions() {
	Settings settings;
	QStringList files = settings.getRecentFiles();
	int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		QString text = tr("&%1 %2").arg(i + 1).arg(files[i]);
		_recentFileActions[i]->setText(text);
		_recentFileActions[i]->setData(files[i]);
		_recentFileActions[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
		_recentFileActions[j]->setVisible(false);

	_recentFileSeparator->setVisible(numRecentFiles > 0);
}
void MainWindow::updateRecentFile(const QStringList &fileList_) {
	Settings settings;
	QStringList files = settings.getRecentFiles();
	for(int i = 0; i < fileList_.size(); i++)
		files.removeAll(fileList_[i]);

	files.append(fileList_);
	while (files.size() > MaxRecentFiles)
		files.removeFirst();
	settings.setRecentFiles(files);
	updateRecentFileActions();
}
void MainWindow::clearRecentFile() {
	Settings settings;
	QStringList files = settings.getRecentFiles();
	files.clear();
	settings.setRecentFiles(files);
	updateRecentFileActions();
}
void MainWindow::openRecentFile() {
	QAction *action = qobject_cast<QAction *>(sender());
	if(action == 0)
		return;

	QString fileName = action->data().toString();
	if(!_documentManager->documentExists(fileName))
		_documentManager->getActiveView()->openDocument(fileName);
}
void MainWindow::openAllRecentFile() {
	Settings settings;
	QStringList files = settings.getRecentFiles();
	//check if files already exists
	int i = 0;
	while(i != files.size()) {
		QString fileName = files[i];

		bool fileRemoved = false;
		if(_documentManager->documentExists(fileName)) {
			files.removeAt(i);
			fileRemoved = true;
		}
		if(!fileRemoved)
			i++;
	}
	_documentManager->getActiveView()->openDocument(files);
}

void MainWindow::fileNew() {
	_documentManager->getActiveView()->newDocument();
}
void MainWindow::fileOpen() {
	///@todo add filter
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Select one or more files to open"), _documentManager->getActiveDocument()->getPath());
	_documentManager->openDocument(files);
	updateRecentFile(files);
}
void MainWindow::fileSave() {
	_documentManager->getActiveView()->save();
}
void MainWindow::fileSaveAs() {
	_documentManager->getActiveView()->saveAs();
}
void MainWindow::fileSaveACopyAs() {
	_documentManager->getActiveView()->saveACopyAs();
}
void MainWindow::fileSaveAll() {
	_documentManager->saveAll();
}
bool MainWindow::fileClose() {
	return _documentManager->getActiveView()->closeCurrentDocument();
}
bool MainWindow::fileCloseAll() {
	return _documentManager->closeAll();
}
void MainWindow::fileCloseAllExceptCurrentDocument() {
	_documentManager->closeAllExceptActiveDocument();
}
void MainWindow::filePrint() {
	_documentManager->getActiveView()->print();
}
void MainWindow::fileReload() {
	if(_documentManager->getActiveDocument()->reload()) {
		_documentManager->getActiveView()->updateAllDocuments();
		statusBar()->showMessage(tr("File reloaded"), 1000);
	}
}

void MainWindow::editCut() {
	_documentManager->getActiveDocument()->cut();
}
void MainWindow::editCopy() {
	_documentManager->getActiveDocument()->copy();
}
void MainWindow::editPaste() {
	_documentManager->getActiveDocument()->paste();
}
void MainWindow::editUndo() {
	DocumentEditor* document = _documentManager->getActiveDocument();
	document->undo();
	actionUndo->setEnabled(document->isUndoAvailable());
	actionRedo->setEnabled(document->isRedoAvailable());
}
void MainWindow::editRedo() {
	DocumentEditor* document = _documentManager->getActiveDocument();
	document->redo();
	actionUndo->setEnabled(document->isUndoAvailable());
	actionRedo->setEnabled(document->isRedoAvailable());
}
void MainWindow::editSelectAll() {
	_documentManager->getActiveDocument()->selectAll();
}

void MainWindow::increaseIndentation() {
	if(!_searchDock->focusWidget()->hasFocus()) {
		_documentManager->getActiveDocument()->increaseIndentation();
	}
}
void MainWindow::decreaseIndentation() {
	if(!_searchDock->focusWidget()->hasFocus()) {
		_documentManager->getActiveDocument()->decreaseIndentation();
	}
}
void MainWindow::setAutoIndentation(bool b) {
	_documentManager->getActiveDocument()->setAutoIndent(b);
}
void MainWindow::showIndentationGuides(bool b) {
	_documentManager->getActiveDocument()->setIndentationGuides(b);
}
void MainWindow::doTrimTrailing() {
	_documentManager->getActiveDocument()->doTrimTrailing();
}

void MainWindow::convertSelectedTextToUpperCase() {
	_documentManager->getActiveDocument()->convertSelectedTextToUpperCase();
}
void MainWindow::convertSelectedTextToLowerCase() {
	_documentManager->getActiveDocument()->convertSelectedTextToLowerCase();
}
void MainWindow::duplicateCurrentLine() {
	_documentManager->getActiveDocument()->duplicateSelectionOrCurrentLine();
}
void MainWindow::copyCurrentLine() {
	_documentManager->getActiveDocument()->copyCurrentLine();
}
void MainWindow::cutCurrentLine() {
	_documentManager->getActiveDocument()->cutCurrentLine();
}
void MainWindow::deleteCurrentLine() {
	_documentManager->getActiveDocument()->deleteCurrentLine();
}
void MainWindow::moveCurrentLineUp() {
	_documentManager->getActiveDocument()->moveCurrentLineUp();
}
void MainWindow::moveCurrentLineDown() {
	_documentManager->getActiveDocument()->moveCurrentLineDown();
}
void MainWindow::deleteCurrentWord() {
	_documentManager->getActiveDocument()->deleteCurrentWord();
}

void MainWindow::setReadOnly(bool b) {
	_documentManager->getActiveDocument()->setReadOnly(b);
}

void MainWindow::aboutToShowEditMenu(){
	DocumentEditor* document = _documentManager->getActiveDocument();
	actionReadOnly->setChecked(document->isReadOnly());
}
void MainWindow::aboutToShowEditIndentationMenu(){
	DocumentEditor* document = _documentManager->getActiveDocument();
	actionAutoIndentation->setChecked(document->autoIndent());
	actionIndentationGuides->setChecked(document->indentationGuides());
}

void MainWindow::zoomIn() {
	_documentManager->getActiveDocument()->zoomIn();
}
void MainWindow::zoomOut() {
	_documentManager->getActiveDocument()->zoomOut();
}
void MainWindow::zoomRestore() {
	_documentManager->getActiveDocument()->zoomTo(0);
}

void MainWindow::showWhiteSpaceAndTab(bool b_) {
	actionShowEndOfLine->setChecked(false);
	actionShowAll->setChecked(false);

	_documentManager->getActiveDocument()->setEolVisibility(false);
	if(b_)
		_documentManager->getActiveDocument()->setWhitespaceVisibility(QsciScintilla::WsVisible);
	else
		_documentManager->getActiveDocument()->setWhitespaceVisibility(QsciScintilla::WsInvisible);

}
void MainWindow::showEndOfLine(bool b_) {
	actionShowWhitespaceAndTab->setChecked(false);
	actionShowAll->setChecked(false);
	_documentManager->getActiveDocument()->setEolVisibility(b_);
	_documentManager->getActiveDocument()->setWhitespaceVisibility(QsciScintilla::WsInvisible);
}
void MainWindow::showAll(bool b_) {
	actionShowWhitespaceAndTab->setChecked(false);
	actionShowEndOfLine->setChecked(false);

	_documentManager->getActiveDocument()->setEolVisibility(b_);
	if(b_)
		_documentManager->getActiveDocument()->setWhitespaceVisibility(QsciScintilla::WsVisible);
	else
		_documentManager->getActiveDocument()->setWhitespaceVisibility(QsciScintilla::WsInvisible);
}

void MainWindow::foldUnfoldAll() {
	_documentManager->getActiveDocument()->foldAll(true);
}
void MainWindow::foldLevel() {
	QAction* action = qobject_cast<QAction*>(sender());
	QString actionNb = action->objectName().split("_")[1];
	int level = actionNb.toInt();
	_documentManager->getActiveDocument()->foldLevel(level);
}
void MainWindow::unfoldLevel() {
	QAction* action = qobject_cast<QAction*>(sender());
	QString actionNb = action->objectName().split("_")[1];
	int level = actionNb.toInt();
	_documentManager->getActiveDocument()->unfoldLevel(level);
}

void MainWindow::aboutToShowViewInvisibleSymbolMenu(){
	DocumentEditor* document = _documentManager->getActiveDocument();
	
	bool wsIsVisible = false;
	if(document->whitespaceVisibility() == QsciScintilla::WsVisible)
		wsIsVisible = true;
	
	bool eolIsVisible = document->eolVisibility();

	if(wsIsVisible == true && eolIsVisible == true) {
		actionShowAll->setChecked(true);
		actionShowWhitespaceAndTab->setChecked(false);
		actionShowEndOfLine->setChecked(false);
	} else {
		actionShowAll->setChecked(false);
		actionShowWhitespaceAndTab->setChecked(wsIsVisible);
		actionShowEndOfLine->setChecked(eolIsVisible);
	}
	actionIndentationGuides->setChecked(document->indentationGuides());	
}

void MainWindow::convertToWindowFormat() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	DocumentEditor* document = _documentManager->getActiveDocument();
	document->convertEols(QsciScintilla::EolWindows);
	document->setEolMode(QsciScintilla::EolWindows);
	actionConvertToWindowsFormat->setEnabled(false);
	actionConvertToUnixFormat->setEnabled(true);
	actionConvertToMacFormat->setEnabled(true);
	updateStatusBar(document);
	QApplication::restoreOverrideCursor();
}
void MainWindow::convertToUnixFormat() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	DocumentEditor* document = _documentManager->getActiveDocument();
	document->convertEols(QsciScintilla::EolUnix);
	document->setEolMode(QsciScintilla::EolUnix);
	actionConvertToWindowsFormat->setEnabled(true);
	actionConvertToUnixFormat->setEnabled(false);
	actionConvertToMacFormat->setEnabled(true);
	updateStatusBar(document);
	QApplication::restoreOverrideCursor();
}
void MainWindow::convertToMacFormat() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	DocumentEditor* document = _documentManager->getActiveDocument();
	document->convertEols(QsciScintilla::EolMac);
	document->setEolMode(QsciScintilla::EolMac);
	actionConvertToWindowsFormat->setEnabled(true);
	actionConvertToUnixFormat->setEnabled(true);
	actionConvertToMacFormat->setEnabled(false);
	updateStatusBar(document);
	QApplication::restoreOverrideCursor();
}
void MainWindow::changeCharset(QAction* action_){
	if(_documentManager->getActiveDocument()->setCodec(action_->text())) {
		_documentManager->getActiveView()->updateAllDocuments();
		statusBar()->showMessage(tr("File reloaded with charset %1").arg(action_->text()), 1000);
	}
}
void MainWindow::saveWithCharset(QAction* action_){
	if(_documentManager->getActiveDocument()->saveWithCharset(action_->text())){
		_documentManager->getActiveView()->updateAllDocuments();
		statusBar()->showMessage(tr("File save with charset %1").arg(action_->text()), 1000);
	}
}
void MainWindow::saveWithCharsetAs(QAction* action_){
	if(_documentManager->getActiveDocument()->saveWithCharsetAs(action_->text())){
		_documentManager->getActiveView()->updateAllDocuments();
		statusBar()->showMessage(tr("File save with charset %1").arg(action_->text()), 1000);
	}
}

void MainWindow::aboutToShowEncodingMenu(){
	DocumentEditor* document = _documentManager->getActiveDocument();
	actionConvertToWindowsFormat->setEnabled(not (document->eolMode() == QsciScintilla::EolWindows));
	actionConvertToUnixFormat->setEnabled(not (document->eolMode() == QsciScintilla::EolUnix));
	actionConvertToMacFormat->setEnabled(not (document->eolMode() == QsciScintilla::EolMac));	
}
void MainWindow::aboutToShowChangeCharsetMenu(){
	QString codec = _documentManager->getActiveDocument()->getCodec();
	foreach(QAction* action, menuUseCharset->actions()){
		if(action->text() == codec){
			action->setChecked(true);
			break;
		}
	}
}
void MainWindow::aboutToShowSaveWithCharsetMenu(){
	QString currentText = _documentManager->getActiveDocument()->text();
	foreach (QAction *action, menuSaveWithCharset->actions()) {
		QTextCodec *codec = QTextCodec::codecForName(action->text().toUtf8());
		action->setVisible(codec && codec->canEncode(currentText));
	}
}
void MainWindow::aboutToShowSaveWithCharsetAsMenu(){
	QString currentText = _documentManager->getActiveDocument()->text();
	foreach (QAction *action, menuSaveWithCharsetAs->actions()) {
		QTextCodec *codec = QTextCodec::codecForName(action->text().toUtf8());
		action->setVisible(codec && codec->canEncode(currentText));
	}
}

void MainWindow::changeLanguage(QAction* action_) {
	_documentManager->getActiveDocument()->setLanguage(action_->text());
}
void MainWindow::aboutToShowLanguageMenu(){
	//langage actions
	DocumentEditor* document = _documentManager->getActiveDocument();
	LexerManager::getInstance().update(document->lexer());
}


void MainWindow::diff() {
	qDebug() << "Not implemented";
	//_documentManager->diff();
}

void MainWindow::startRecordingMacro() {
	_documentManager->startRecordingMacro();
}
void MainWindow::stopRecordingMacro() {
	_documentManager->stopRecordingMacro();
}
void MainWindow::runMacro() {
	_documentManager->runMacro();
}
void MainWindow::runMacroMultipleTimes() {
	Ui::RunMacroDialog ui;
	QDialog* dlg = new QDialog();
	ui.setupUi(dlg);
	if(dlg->exec() == QDialog::Accepted) {
		if(ui.rbNbTimes->isChecked()) {
			_documentManager->runMacro(ui.spbNbTimes->value());
		} else {
			_documentManager->runMacroUntilEOF();
		}
	}
	delete dlg;
}
void MainWindow::loadMacro() {
	qDebug() << "Not implemented";
}
void MainWindow::saveMacro() {
	qDebug() << "Not implemented";
}

void MainWindow::toggleBookmark() {
	_documentManager->getActiveDocument()->toggleBookmark();
}
void MainWindow::removeAllBookmarks() {
	_documentManager->getActiveDocument()->removeAllBookmarks();
}
void MainWindow::nextBookmark() {
	_documentManager->getActiveDocument()->nextBookmark();
}
void MainWindow::prevBookmark() {
	_documentManager->getActiveDocument()->prevBookmark();
}

void MainWindow::settings() {
	Settings settings;
	SettingsDialog* settingsDlg = new SettingsDialog(*this, &settings);
	settingsDlg->exec();
	delete settingsDlg;
}

void MainWindow::about() {
	AboutDlg about;
	about.exec();
}

void MainWindow::writeSettings() {
	Settings settings;
	_documentManager->saveCurrentSession(settings);
}

void MainWindow::readSettings() {
	Settings settings;
	//Restore last session
	_documentManager->restoreLastSession(settings);
}

bool MainWindow::eventFilter(QObject *obj_, QEvent *ev_) {
	if ( obj_ != qApp )
		return QMainWindow::eventFilter(obj_, ev_);

	if ( ev_->type() == QEvent::ApplicationActivate ) {
		//qDebug() << __FUNCTION__ << obj_ << ev_;
		_documentManager->notify();
		return true;
	}
	/*if ( ev_->type() == QEvent::ApplicationDeactivate ){
		qDebug() << __FUNCTION__ << obj_ << ev_;
	    return true;
	}*/
	return QMainWindow::eventFilter(obj_, ev_);
}

