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

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <vector>
#include <QMainWindow>
#include "ui_mainwindow.h"

class QAction;
class QLabel;
class QsciScintilla;

class MenuLabel;
class DocumentEditor;
class DocumentManager;
class SymbolManager;
class SessionManager;
class HelpBrowser;

class MainWindow : public QMainWindow, private Ui::MainWindow{

	Q_OBJECT

	public:
		MainWindow();
		~MainWindow();

		/** @return reference on documentManager */
		DocumentManager& getDocumentManager();

	protected:
		virtual void closeEvent(QCloseEvent *event_);
		virtual bool eventFilter(QObject *obj_, QEvent *ev_);

	public slots:
		void handleMessage(const QString& message_);

	private slots:
		/** Edit Menu */
		void increaseIndentation();
		void decreaseIndentation();
		void aboutToShowEditMenu();
		void aboutToShowEditIndentationMenu();
		void gotoLine();

		/** Wiew Menu */
		void showWhiteSpaceAndTab(bool b_);
		void showEndOfLine(bool b_);
		void showAll(bool b_);
		void foldLevel();
		void unfoldLevel();
		void aboutToShowViewInvisibleSymbolMenu();

		/** Encoding Menu */
		void convertToWindowFormat();
		void convertToUnixFormat();
		void convertToMacFormat();
		void changeCharset(QAction*);
		void saveWithCharset(QAction*);
		void saveWithCharsetAs(QAction*);
		void aboutToShowEncodingMenu();
		void aboutToShowChangeCharsetMenu();
		void aboutToShowSaveWithCharsetMenu();
		void aboutToShowSaveWithCharsetAsMenu();

		/** Language Menu */
		void changeLanguage(QAction*);
		void aboutToShowLanguageMenu();

		/** Macro Menu */
		void runMacroMultipleTimes();
		void loadMacro(); //@todo
		void saveMacro(); //@todo

		/** Bookmarks Menu */

		/** Settings */
		void settings();
		void shortcuts();

		void about();
		void showDocumentation();
		
		/** update all */
		void update(DocumentEditor*);

		void activeDocumentChanged(DocumentEditor*);
		
		/** update the main window information */
		void updateMainWindowTitle(DocumentEditor*);
		/** update the status bar information */
		void updateStatusBar(DocumentEditor*);
		void updateStatusBar(DocumentEditor*, int, int);
		/** update & display a status message */
		void updateStatusBarMessage(QString);
		/** update menu actions */
		void updateActions(DocumentEditor*);

		void updateRecentFile(const QStringList &fileList_);
		void updateRecentFileActions();
		void clearRecentFile();
		void openRecentFile();
		void openAllRecentFile();

		void visibilityChanged(bool visible_);
		void synchronizeExplorerWithCurrentDocument();

	private:
		/** create window docks */
		void createDocks();
		/** create the status bar */
		void createStatusBar();
		/** create the extra toolbar */
		void createToolBar();
		
		void readSettings();
		void writeSettings();

		/** create the all manager */
		void createManager();
		

		/** intialize and connect menu */
		void initMenu();
		void initMenuFile();
		void initMenuEdit();
		void initMenuView();
		void initMenuSearch();
		void initMenuEncoding();
		void initMenuBookmarks();
		void initMenuLanguage();
		void initMenuDiff();
		void initMenuMacro();
		void initMenuSettings();
		void initMenuHelp();

	private:
		/** document manager */
		DocumentManager* _documentManager;
		/** session manager */
		SessionManager* _sessionManager;
		/** symbol manager */
		SymbolManager* _symbolManager;

		/** help browser */
		HelpBrowser* _helpBrowser;
		
		/** search dock */
		QDockWidget* _searchDock;
		/** symbol dock */
		QDockWidget* _symbolDock;
		/** explorer */
		QDockWidget* _explorerDock;

		/** status bar widget */
		MenuLabel* _encodingLabel;
		MenuLabel* _formatLabel;
		QLabel* _editInfoLabel;
		QLabel* _fileInfoLabel;
		QLabel* _fileTypeLabel;

		/** recent files */
		static const int MaxRecentFiles = 10;
		QAction *_recentFileActions[MaxRecentFiles];
		QAction *_recentFileSeparator;

		friend class Settings;
};

#endif // __MAINWINDOW_H__
