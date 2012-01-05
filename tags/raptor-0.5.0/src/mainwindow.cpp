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
#include <Qsci/qsciscintilla.h>

#include "ui_macro.h"
#include "about/About.h"
#include "about/Version.h"
#include "about/HelpBrowser.h"
#include "settings/Settings.h"
#include "settings/SettingsDialog.h"
#include "settings/ShortcutEditor.h"
#include "settings/ShortcutSettings.h"
#include "settings/SessionManager.h"
#include "search.h"
#include "explorer.h"
#include "document_editor.h"
#include "document_view.h"
#include "document_manager.h"
#include "lexer/lexer_manager.h"
#include "ctags/SymbolManager.h"
#include "widget/MenuLabel.h"
#include "export/Exporter.h"
#include "export/ExporterHTML.h"

#include "mainwindow.h"

MainWindow::MainWindow() {
	//init ui
	setupUi(this);

	//create manager
	createManager();

	//initialize menu
	initMenu();

	//create status bar
	createStatusBar();
	//create tool bar
	createToolBar();
	//create docks
	createDocks();
	//create action
	createActions();

	//read Settings
	readSettings();
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
	initMenuDiff();
	initMenuMacro();
	initMenuSettings();
	initMenuHelp();
}

void MainWindow::initMenuFile() {
	//connect menu "File" Action
	connect(actionNew, SIGNAL(triggered()), _documentManager, SLOT(newDocument()));
	connect(actionOpen, SIGNAL(triggered()), _documentManager, SLOT(open()));
	connect(actionSave, SIGNAL(triggered()), _documentManager, SLOT(save()));
	connect(actionSaveAs, SIGNAL(triggered()), _documentManager, SLOT(saveAs()));
	connect(actionSaveACopyAs, SIGNAL(triggered()), _documentManager, SLOT(saveACopyAs()));
	connect(actionSaveAll, SIGNAL(triggered()), _documentManager, SLOT(saveAll()));
	connect(actionClose, SIGNAL(triggered()), _documentManager, SLOT(close()));
	connect(actionCloseAll, SIGNAL(triggered()), _documentManager, SLOT(closeAll()));
	connect(actionCloseAllExceptCurrentDocument, SIGNAL(triggered()), _documentManager, SLOT(closeAllExceptCurrentDocument()));
	connect(actionReload, SIGNAL(triggered()), _documentManager, SLOT(reload()));
	connect(actionPrint, SIGNAL(triggered()), _documentManager, SLOT(print()));
	connect(actionExit, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

	connect(actionExportAsHTML, SIGNAL(triggered()), this, SLOT(exportDocument()));

	connect(actionNewSession, SIGNAL(triggered()), _sessionManager, SLOT(newSession()));
	connect(actionOpenSession, SIGNAL(triggered()), _sessionManager, SLOT(openSession()));
	connect(actionSwitchSession, SIGNAL(triggered()), _sessionManager, SLOT(switchSession()));
	connect(actionSaveSession, SIGNAL(triggered()), _sessionManager, SLOT(saveSession()));
	connect(actionSaveSessionAs, SIGNAL(triggered()), _sessionManager, SLOT(saveSessionAs()));
	connect(actionManageSessions, SIGNAL(triggered()), _sessionManager, SLOT(manageSessions()));

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
	connect(actionCut, SIGNAL(triggered()), _documentManager, SLOT(cut()));
	connect(actionCopy, SIGNAL(triggered()), _documentManager, SLOT(copy()));
	connect(actionPaste, SIGNAL(triggered()), _documentManager, SLOT(paste()));
	connect(actionUndo, SIGNAL(triggered()), _documentManager, SLOT(undo()));
	connect(actionRedo, SIGNAL(triggered()), _documentManager, SLOT(redo()));
	connect(actionSelectAll, SIGNAL(triggered()), _documentManager, SLOT(selectAll()));

	connect(actionIndent, SIGNAL(triggered()), this, SLOT(increaseIndentation()));
	connect(actionUnindent, SIGNAL(triggered()), this, SLOT(decreaseIndentation()));
	connect(actionAutoIndentation, SIGNAL(toggled(bool)), _documentManager, SLOT(setAutoIndentation(bool)));
	connect(actionIndentationGuides, SIGNAL(toggled(bool)), _documentManager, SLOT(showIndentationGuides(bool)));

	connect(actionTabsToSpaces, SIGNAL(triggered()), _documentManager, SLOT(tabsToSpaces()));
	connect(actionSpacesToTabs, SIGNAL(triggered()), _documentManager, SLOT(spacesToTabs()));
	connect(actionTrimTrailingSpaces, SIGNAL(triggered()), _documentManager, SLOT(trimTrailingSpaces()));
	connect(actionCompressSpaces, SIGNAL(triggered()), _documentManager, SLOT(compressSpaces()));

	connect(actionUpperCase, SIGNAL(triggered()), _documentManager, SLOT(convertSelectedTextToUpperCase()));
	connect(actionLowerCase, SIGNAL(triggered()), _documentManager, SLOT(convertSelectedTextToLowerCase()));
	connect(actionDuplicateLine, SIGNAL(triggered()), _documentManager, SLOT(duplicateCurrentLine()));
	connect(actionCopyLine, SIGNAL(triggered()), _documentManager, SLOT(copyCurrentLine()));
	connect(actionCutLine, SIGNAL(triggered()), _documentManager, SLOT(cutCurrentLine()));
	connect(actionDeleteLine, SIGNAL(triggered()), _documentManager, SLOT(deleteCurrentLine()));
	connect(actionMoveLineUp, SIGNAL(triggered()), _documentManager, SLOT(moveCurrentLineUp()));
	connect(actionMoveLineDown, SIGNAL(triggered()), _documentManager, SLOT(moveCurrentLineDown()));
	connect(actionDeleteWord, SIGNAL(triggered()), _documentManager, SLOT(deleteCurrentWord()));

	connect(actionGotoLine, SIGNAL(triggered()), this, SLOT(gotoLine()));

	connect(actionReadOnly, SIGNAL(triggered(bool)), _documentManager, SLOT(setReadOnly(bool)));
	connect(actionReindentFile, SIGNAL(triggered()), _documentManager, SLOT(reindentDocument()));
	connect(actionReindentOpenFiles, SIGNAL(triggered()), _documentManager, SLOT(reindentOpenDocuments()));

	connect(actionToggleComment, SIGNAL(triggered()), _documentManager, SLOT(toggleComment()));
	connect(actionToggleBlockComment, SIGNAL(triggered()), _documentManager, SLOT(toggleBlockComment()));

	connect(actionSwitchSrc, SIGNAL(triggered()), _documentManager, SLOT(switchDocumentSrc()));
	connect(actionSwitchFile, SIGNAL(triggered()), _documentManager, SLOT(switchFile()));

	connect(menuEdit, SIGNAL(aboutToShow()), this, SLOT(aboutToShowEditMenu()));
	connect(menuIndentation, SIGNAL(aboutToShow()), this, SLOT(aboutToShowEditIndentationMenu()));
}

void MainWindow::initMenuView() {
	//connect menu "View" Action
	connect(actionZoomIn, SIGNAL(triggered()), _documentManager, SLOT(zoomIn()));
	connect(actionZoomOut, SIGNAL(triggered()), _documentManager, SLOT(zoomOut()));
	connect(actionRestoreDefaultZoom, SIGNAL(triggered()), _documentManager, SLOT(zoomRestore()));
	connect(actionShowWhitespaceAndTab, SIGNAL(triggered(bool)), this, SLOT(showWhiteSpaceAndTab(bool)));
	connect(actionShowEndOfLine, SIGNAL(triggered(bool)), this, SLOT(showEndOfLine(bool)));
	connect(actionShowAll, SIGNAL(triggered(bool)), this, SLOT(showAll(bool)));
	connect(actionFoldAll, SIGNAL(triggered()), _documentManager, SLOT(foldUnfoldAll()));

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
	//nothing to do here
	//connection is done during the dock creation
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
	QActionGroup* forceSaveWithCharsetAsActionGroup = new QActionGroup(menuForceSaveWithCharsetAs);
	foreach(QTextCodec* codec, codecMap.values()){
		//menu use charset
		QAction* action = menuUseCharset->addAction(codec->name());
		action->setObjectName("codec_menuUseCharset_" + codec->name());
		action->setCheckable(true);
		useCharsetActionGroup->addAction(action);
		//menu menuSaveWithCharset
		action = menuSaveWithCharset->addAction(codec->name());
		action->setObjectName("codec_menuSaveWithCharset_" + codec->name());
		saveWithCharsetActionGroup->addAction(action);
		//menu menuSaveWithCharsetAs
		action = menuSaveWithCharsetAs->addAction(codec->name());
		action->setObjectName("codec_menuSaveWithCharsetAs_" + codec->name());
		saveWithCharsetAsActionGroup->addAction(action);
		//menu menuForceSaveWithCharsetAs
		action = menuForceSaveWithCharsetAs->addAction(codec->name());
		action->setObjectName("codec_menuForceSaveWithCharsetAs_" + codec->name());
		forceSaveWithCharsetAsActionGroup->addAction(action);
	}
	connect(useCharsetActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(changeCharset(QAction*)));
	connect(saveWithCharsetActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(saveWithCharset(QAction*)));
	connect(saveWithCharsetAsActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(saveWithCharsetAs(QAction*)));
	connect(forceSaveWithCharsetAsActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(saveWithCharsetAs(QAction*)));

	connect(menuEncoding, SIGNAL(aboutToShow()), this, SLOT(aboutToShowEncodingMenu()));
	connect(menuUseCharset, SIGNAL(aboutToShow()), this, SLOT(aboutToShowChangeCharsetMenu()));
	connect(menuSaveWithCharset, SIGNAL(aboutToShow()), this, SLOT(aboutToShowSaveWithCharsetMenu()));
	connect(menuSaveWithCharsetAs, SIGNAL(aboutToShow()), this, SLOT(aboutToShowSaveWithCharsetAsMenu()));
}

void MainWindow::initMenuBookmarks() {
	//connect menu "Bookmarks" Action
	connect(actionToggleBookmark, SIGNAL(triggered()), _documentManager, SLOT(toggleBookmark()));
	connect(actionRemoveAllBookmarks, SIGNAL(triggered()), _documentManager, SLOT(removeAllBookmarks()));
	connect(actionNextBookmark, SIGNAL(triggered()), _documentManager, SLOT(nextBookmark()));
	connect(actionPreviousBookmark, SIGNAL(triggered()), _documentManager, SLOT(prevBookmark()));
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
	connect(actionQuickUnidiff, SIGNAL(triggered()), _documentManager, SLOT(quickUnidiff()));
	connect(actionUnidiff, SIGNAL(triggered()), _documentManager, SLOT(unidiff()));
}

void MainWindow::initMenuMacro() {
	//connect menu "Macro" Action
	connect(actionStartRecording, SIGNAL(triggered()), _documentManager, SLOT(startRecordingMacro()));
	connect(actionStopRecording, SIGNAL(triggered()), _documentManager, SLOT(stopRecordingMacro()));
	connect(actionRunMacro, SIGNAL(triggered()), _documentManager, SLOT(runMacro()));
	connect(actionRunMacroMultipleTimes, SIGNAL(triggered()), this, SLOT(runMacroMultipleTimes()));
	//connect(actionSaveMacro, SIGNAL(triggered()), this, SLOT(saveMacro()));
	//connect(actionLoadMacro, SIGNAL(triggered()), this, SLOT(loadMacro()));
}

void MainWindow::initMenuSettings() {
	//connect menu "Settings" Action
	connect(actionSettings, SIGNAL(triggered()), this, SLOT(settings()));
	connect(actionShortcuts, SIGNAL(triggered()), this, SLOT(shortcuts()));
}

void MainWindow::initMenuHelp() {
	//connect menu "Help" Action
	connect(actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(actionHelp,  SIGNAL(triggered()), this, SLOT(showDocumentation()));
}

void MainWindow::createManager() {
	_documentManager = new DocumentManager(this);
	_sessionManager = new SessionManager(*_documentManager, this);
	_symbolManager = new SymbolManager(this);
	_helpBrowser = new HelpBrowser(this);

	setCentralWidget(_documentManager);

	//connection
	connect(_documentManager, SIGNAL(statusMessage(QString)), this, SLOT(updateStatusBarMessage(QString)));
	connect(_documentManager, SIGNAL(activeDocumentChanged(DocumentEditor*)), this, SLOT(activeDocumentChanged(DocumentEditor*)));
	connect(_documentManager, SIGNAL(selectionChanged(DocumentEditor*)), this, SLOT(updateStatusBar(DocumentEditor*)));
	connect(_documentManager, SIGNAL(cursorPositionChanged(DocumentEditor*, int, int)), this, SLOT(updateStatusBar(DocumentEditor*, int, int)));
	connect(_documentManager, SIGNAL(opened(QStringList)), this, SLOT(updateRecentFile(QStringList)));
	connect(_documentManager, SIGNAL(saved(QStringList)), this, SLOT(updateRecentFile(QStringList)));

	connect(_symbolManager, SIGNAL(symbolActivated(int)), _documentManager, SLOT(gotoLine(int)));

	connect(_sessionManager, SIGNAL(statusMessage(QString)), this, SLOT(updateStatusBarMessage(QString)));
}

void MainWindow::createStatusBar() {
	_encodingLabel = new MenuLabel(this);
	_encodingLabel->setObjectName("EncodingLabel");
	_encodingLabel->setText("??");
	_encodingLabel->getMenu()->setObjectName("EncodingLabelMenu");
	_encodingLabel->getMenu()->addMenu(menuUseCharset);
	_encodingLabel->getMenu()->addMenu(menuSaveWithCharset);
	_encodingLabel->getMenu()->addMenu(menuSaveWithCharsetAs);
	_encodingLabel->getMenu()->addMenu(menuForceSaveWithCharsetAs);

	_formatLabel = new MenuLabel(this);
	_formatLabel->setObjectName("FormatLabel");
	_formatLabel->getMenu()->setObjectName("FormatLabelMenu");
	_formatLabel->getMenu()->addAction(actionConvertToWindowsFormat);
	_formatLabel->getMenu()->addAction(actionConvertToUnixFormat);
	_formatLabel->getMenu()->addAction(actionConvertToMacFormat);

	_editInfoLabel = new QLabel(this);
	_fileInfoLabel = new QLabel(this);
	_fileTypeLabel = new QLabel(this);

	statusBar()->addWidget(_fileTypeLabel, 7);
	statusBar()->addWidget(_fileInfoLabel, 2);
	statusBar()->addWidget(_editInfoLabel, 3);
	statusBar()->addPermanentWidget(_encodingLabel);
	statusBar()->addPermanentWidget(_formatLabel);
}

void MainWindow::createToolBar(){
	QToolBar* toolbar = addToolBar("Symbol");
	toolbar->setObjectName(QString::fromUtf8("toolBarSymbol"));
	toolbar->addAction(_symbolManager->getSymbolBrowerAction());
	//default is not visible
	toolbar->setVisible(false);
	connect(toolbar, SIGNAL(visibilityChanged(bool)), this, SLOT(visibilityChanged(bool)));
}

void MainWindow::createDocks() {
	//create the Search Dock
	_searchDock = new QDockWidget(tr("Search/Replace"), this);
	Search* search = new Search(_documentManager, _searchDock);
	_searchDock->setWidget(search);
	addDockWidget(Qt::BottomDockWidgetArea, _searchDock);
	//connect the Search Dock
	connect(_searchDock, SIGNAL(visibilityChanged(bool)), search, SLOT(setVisible(bool)));
	connect(actionSearch, SIGNAL(triggered()), _searchDock->toggleViewAction(), SLOT(trigger()));
	connect(actionFindNext, SIGNAL(triggered()), search, SLOT(next()));
	connect(actionFindPrevious, SIGNAL(triggered()), search, SLOT(prev()));
	_searchDock->hide();

	//create the Symbol Dock
	_symbolDock = new QDockWidget(tr("Symbol Browser"), this);
	_symbolDock->setWidget(_symbolManager->getSymbolBrowerTreeView());
	_symbolDock->setObjectName(QString::fromUtf8("dockSymbol"));
	addDockWidget(Qt::RightDockWidgetArea, _symbolDock);
	//connect the Symbol Dock
	connect(_symbolDock, SIGNAL(visibilityChanged(bool)), this, SLOT(visibilityChanged(bool)));
	_symbolDock->hide();

	//create the Explorer Dock
	_explorerDock = new QDockWidget(tr("File Explorer"), this);
	Explorer* explorer = new Explorer(_explorerDock);
	_explorerDock->setWidget(explorer);
	_explorerDock->setObjectName(QString::fromUtf8("dockExplorer"));
	addDockWidget(Qt::LeftDockWidgetArea, _explorerDock);
	connect(explorer, SIGNAL(fileClicked(const QString&)), _documentManager, SLOT(open(const QString&)));
	connect(explorer, SIGNAL(synchFileRequest()), this, SLOT(synchronizeExplorerWithCurrentDocument()));
	_explorerDock->hide();
}

void MainWindow::createActions(){
	//set the corresponding qscicommand to corresponding actions
	actionUndo->setData(QVariant(QsciCommand::Undo));
	actionRedo->setData(QVariant(QsciCommand::Redo));

	//@todo other delete word
	actionDeleteWord->setData(QVariant(QsciCommand::DeleteWordRight));

	actionCutLine->setData(QVariant(QsciCommand::LineCut));
	actionCopyLine->setData(QVariant(QsciCommand::LineCopy));
	actionDeleteLine->setData(QVariant(QsciCommand::LineDelete));
	//@todo maybe use MoveSelectedLinesUp - MoveSelectedLinesDown instead of doing it with LineTranspose
	actionTransposeLine->setData(QVariant(QsciCommand::LineTranspose));

	//QsciCommand = LineDuplicate and SelectionDuplicate -> use SelectionDuplicate
	actionDuplicateLine->setData(QVariant(QsciCommand::SelectionDuplicate));

	actionSelectAll->setData(QVariant(QsciCommand::SelectAll));
	actionLowerCase->setData(QVariant(QsciCommand::SelectionLowerCase));
	actionUpperCase->setData(QVariant(QsciCommand::SelectionUpperCase));
	actionCut->setData(QVariant(QsciCommand::SelectionCut));
	actionCopy->setData(QVariant(QsciCommand::SelectionCopy));
	actionPaste->setData(QVariant(QsciCommand::Paste));
	actionIndent->setData(QVariant(QsciCommand::Tab));
	actionUnindent->setData(QVariant(QsciCommand::Backtab));

	actionZoomIn->setData(QVariant(QsciCommand::ZoomIn));
	actionZoomOut->setData(QVariant(QsciCommand::ZoomOut));
}

DocumentManager& MainWindow::getDocumentManager() {
	return *_documentManager;
}

void MainWindow::handleMessage(const QString& message_) {
	QStringList files = message_.split(";");
	files.removeAll("");
	if(!files.empty()) {
		_documentManager->open(files);
		updateRecentFile(files);
	}
}

void MainWindow::activeDocumentChanged(DocumentEditor* document_){
	//update mainwindow
	update(document_);
	//display symbol
	QToolBar* toolbar = (findChildren<QToolBar*>("toolBarSymbol"))[0];
	if(_symbolDock->isVisible() || toolbar->isVisible()){
		_symbolManager->tagFile(document_->getFullPath());
	}
	//file explorer synchronization
	if(_explorerDock->isVisible()){
		static_cast<Explorer*>(_explorerDock->widget())->synchronize(document_->getFullPath());
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
	int line,column;
	document_->getCursorPosition(&line, &column);
	updateStatusBar(document_, line, column);
}

void MainWindow::updateStatusBar(DocumentEditor* document_, int line_, int col_) {
	int nbChars = document_->text().length();
	int nbLines = document_->lines();
	int select = document_->selectedText().length();
	_editInfoLabel->setText(tr("Line : %1  Col : %2  Sel : %3     ").arg(line_).arg(col_).arg(select));
	_fileInfoLabel->setText(tr("%1 chars  %2 lines     ").arg(nbChars).arg(nbLines));
	_encodingLabel->setText(document_->getCodec());
	//_formatLabel->setText(document->getFormat() + "     ");
	_formatLabel->setPixmap(document_->getFormatPixmap().scaled(16,16));
	_fileTypeLabel->setText(document_->getType());
}

void MainWindow::updateStatusBarMessage(QString message_){
	statusBar()->showMessage(message_, 1000);
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

void MainWindow::visibilityChanged(bool visible_){
	if(visible_){
		//display symbol
		QToolBar* toolbar = (findChildren<QToolBar*>("toolBarSymbol"))[0];
		if(_symbolDock->isVisible() || toolbar->isVisible()){
			_symbolManager->tagFile(_documentManager->getActiveDocument()->getFullPath());
		}
	}
}

void MainWindow::synchronizeExplorerWithCurrentDocument(){
	static_cast<Explorer*>(_explorerDock->widget())->forceSynchronize(_documentManager->getActiveDocument()->getFullPath());
}

void MainWindow::closeEvent(QCloseEvent *event_) {
	writeSettings();
	if(_documentManager->closeAll() == true) {
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
		_documentManager->open(fileName);
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
	_documentManager->open(files);
}

void MainWindow::exportDocument(){
	DocumentEditor* doc = _documentManager->getActiveDocument();
	QString docName = doc->getBaseName();

	Exporter* exporter;
	QString type;
	QAction* action = qobject_cast<QAction*>(sender());
	if(action->objectName() == "actionExportAsHTML"){
		exporter = new ExporterHTML();
		type = "HTML";
	}else{
		return;
	}
	QString file = QFileDialog::getSaveFileName(this, tr("Export File as ") + type, doc->getFullPath() + "." + type.toLower());
	if (!file.isEmpty()){
		exporter->write(*doc, file);
		updateStatusBarMessage(tr("%1 exported to %2").arg(doc->getName()).arg(file));
	}
	delete exporter;
}

void MainWindow::increaseIndentation() {
	if(!_searchDock->focusWidget()->hasFocus()) {
		_documentManager->increaseIndentation();
	}
}

void MainWindow::decreaseIndentation() {
	if(!_searchDock->focusWidget()->hasFocus()) {
		_documentManager->decreaseIndentation();
	}
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

void MainWindow::gotoLine(){
	bool ok = false;
	int line = QInputDialog::getInt(this, tr("Goto line ..."), tr("Enter the line you want to go :"), 1, 1, 2147483647, 1, &ok);
	if(ok)
		_documentManager->gotoLine(line);
}

void MainWindow::showWhiteSpaceAndTab(bool b_) {
	actionShowEndOfLine->setChecked(false);
	actionShowAll->setChecked(false);
	_documentManager->showWhiteSpaceAndTab(b_);
}

void MainWindow::showEndOfLine(bool b_) {
	actionShowWhitespaceAndTab->setChecked(false);
	actionShowAll->setChecked(false);
	_documentManager->showEndOfLine(b_);
}

void MainWindow::showAll(bool b_) {
	actionShowWhitespaceAndTab->setChecked(false);
	actionShowEndOfLine->setChecked(false);
	_documentManager->showAll(b_);
}

void MainWindow::foldLevel() {
	QAction* action = qobject_cast<QAction*>(sender());
	QString actionNb = action->objectName().split("_")[1];
	int level = actionNb.toInt();
	_documentManager->foldLevel(level);
}

void MainWindow::unfoldLevel() {
	QAction* action = qobject_cast<QAction*>(sender());
	QString actionNb = action->objectName().split("_")[1];
	int level = actionNb.toInt();
	_documentManager->unfoldLevel(level);
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
	_documentManager->convertToWindowFormat();
	actionConvertToWindowsFormat->setEnabled(false);
	actionConvertToUnixFormat->setEnabled(true);
	actionConvertToMacFormat->setEnabled(true);
	//updateStatusBar(document);
	QApplication::restoreOverrideCursor();
}

void MainWindow::convertToUnixFormat() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	_documentManager->convertToUnixFormat();
	actionConvertToWindowsFormat->setEnabled(true);
	actionConvertToUnixFormat->setEnabled(false);
	actionConvertToMacFormat->setEnabled(true);
	//updateStatusBar(document);
	QApplication::restoreOverrideCursor();
}

void MainWindow::convertToMacFormat() {
	QApplication::setOverrideCursor(Qt::WaitCursor);
	_documentManager->convertToMacFormat();
	actionConvertToWindowsFormat->setEnabled(true);
	actionConvertToUnixFormat->setEnabled(true);
	actionConvertToMacFormat->setEnabled(false);
	//updateStatusBar(document);
	QApplication::restoreOverrideCursor();
}

void MainWindow::changeCharset(QAction* action_){
	_documentManager->changeCharset(action_->text());
}

void MainWindow::saveWithCharset(QAction* action_){
	_documentManager->saveWithCharset(action_->text());
}

void MainWindow::saveWithCharsetAs(QAction* action_){
	_documentManager->saveWithCharsetAs(action_->text());
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
	_documentManager->changeLanguage(action_->text());
}

void MainWindow::aboutToShowLanguageMenu(){
	//langage actions
	DocumentEditor* document = _documentManager->getActiveDocument();
	LexerManager::getInstance().update(document->lexer());
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


void MainWindow::settings() {
	SettingsDialog dlg(*this);
	dlg.exec();
}
void MainWindow::shortcuts() {
	QList<QMenu*> menus = findChildren<QMenu*>();
	ShortcutEditor dlg(menus);
	dlg.exec();
	//update open document
	ShortcutSettings settings;
	QList<DocumentEditor*>list = _documentManager->getDocuments();
	foreach(DocumentEditor* d, list){
		settings.update(d);
	}
}

void MainWindow::showDocumentation() {
	QMessageBox::information(this, PACKAGE_NAME, tr("Not implemented yet !! Will be available in the next release"));
	//_helpBrowser->showDocumentation("index.html");
}

void MainWindow::about() {
	AboutDlg about;
	about.exec();
}

void MainWindow::writeSettings() {
	Settings settings;
	settings.setVersion(RaptorVersion);
	_sessionManager->saveStartSession();
}

void MainWindow::readSettings() {
	Settings settings;
	ShortcutSettings shortcuts;

	//check version
	Version settingsVersion = settings.getVersion();
	if(settingsVersion != RaptorVersion){
		QMessageBox msgBox;
		msgBox.setWindowTitle(tr("Old Settings!"));
		msgBox.setText(tr("Found settings file for raptor version ") + settingsVersion.getVersion() + tr(". Current version is ") + RaptorVersion.getVersion());
		msgBox.setInformativeText(tr("What do you want to do?"));

		QString detailedTxt;
		detailedTxt += tr("Reset : Remove old settings file and create a new one\n");
		detailedTxt += tr("Upgrade : keep and upgrade your old settings\n");
		msgBox.setDetailedText(detailedTxt);
		QPushButton* resetSettings = msgBox.addButton(tr("Reset"), QMessageBox::AcceptRole);
		resetSettings->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogResetButton));
		QPushButton* upgradeSettings = msgBox.addButton(tr("Upgrade"), QMessageBox::AcceptRole);
		upgradeSettings->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
		msgBox.setDefaultButton(upgradeSettings);
		int ret = msgBox.exec();
		if (ret == 0){
			//remove old settings!!
			settings.clear();
			shortcuts.clear();
		}else{
			QMessageBox::information(this, PACKAGE_NAME, tr("Settings upgrade is not implemented yet!\nCurrent settings file is kept but some settings can now be inconsistent! A settings reset is recommanded!"));
		}
	}

	//read shortcut settings
	bool hasDefaultShortcut = shortcuts.hasDefaultShortcut();
	QList<QMenu*> menus = findChildren<QMenu*>();
	foreach(QMenu* menu, menus){
		//discard unwanted menu
		if(menu->title().isEmpty())
			continue;
		if(menu->objectName() == "menuLanguage")
			continue;
		if(menu->objectName() == "menuUseCharset")
			continue;
		if(menu->objectName() == "menuSaveWithCharset")
			continue;
		if(menu->objectName() == "menuSaveWithCharsetAs")
			continue;
		if(menu->objectName() == "menuForceSaveWithCharsetAs")
			continue;

		QList<QAction*> actions = menu->actions();
		foreach(QAction *action, actions) {
			//discard unwanted action
			if (action->menu())
				continue;
			if (action->isSeparator())
				continue;
			if (action->objectName().isEmpty())
				continue;

			//if there is no default shortcuts save them
			if(!hasDefaultShortcut){
				shortcuts.saveDefaultShortcut(action);
			}
			//load user custom shortcut
			shortcuts.updateActionWithUserShortcut(action);
		}
	}


	//Restore last session
	_sessionManager->openStartSession();
	settings.apply(*this);
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

