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
		/** File Menu */
		void fileNew();
		void fileOpen();
		void fileSave();
		void fileSaveAs();
		void fileSaveACopyAs();
		void fileSaveAll();
		bool fileClose();
		bool fileCloseAll();
		void fileCloseAllExceptCurrentDocument();
		void fileReload();
		void filePrint();	
		/** Edit Menu */
		void editCut();
		void editCopy();
		void editPaste();
		void editUndo();
		void editRedo();
		void editSelectAll();
		void increaseIndentation();
		void decreaseIndentation();
		void setAutoIndentation(bool b_);
		void showIndentationGuides(bool b_);
		void doTrimTrailing();
		
		void convertSelectedTextToUpperCase();
		void convertSelectedTextToLowerCase();
		void duplicateCurrentLine();
		void copyCurrentLine();
		void cutCurrentLine();
		void deleteCurrentLine();
		void moveCurrentLineUp();
		void moveCurrentLineDown();
		void deleteCurrentWord();
		
		void setReadOnly(bool b_);
		
		void aboutToShowEditMenu();
		void aboutToShowEditIndentationMenu();
		
		/** Wiew Menu */
		void zoomIn();
		void zoomOut();
		void zoomRestore();
		void showWhiteSpaceAndTab(bool b_);
		void showEndOfLine(bool b_);
		void showAll(bool b);
		void foldUnfoldAll();
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
		
		/** Diff Menu */
		void diff();
		
		/** Macro Menu */
		void startRecordingMacro();
		void stopRecordingMacro();
		void runMacro();
		void runMacroMultipleTimes();
		void loadMacro();
		void saveMacro();
		
		/** Bookmarks Menu */
	    void toggleBookmark();
		void removeAllBookmarks();
		void nextBookmark();
		void prevBookmark();
		
		/** Settings */
		void settings();
		
		void about();

		/** update all */
		void update(DocumentEditor*);
		/** update the main window information */
		void updateMainWindowTitle(DocumentEditor*);
		/** update the status bar information */
		void updateStatusBar(DocumentEditor*);
		/** update menu actions */
		void updateActions(DocumentEditor*);
		
		void updateRecentFile(const QStringList &fileList_);
		void updateRecentFileActions();
		void clearRecentFile();
		void openRecentFile();
		void openAllRecentFile();
	
	private:
		/** create a search dock
		 * @return the associated toggle view action */
		QAction* createSearchDock();
		
		void readSettings();
		void writeSettings();

		/** create the documentManager */
		void createDocumentManager();
		/** create the status bar */
		void createStatusBar();
		
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
		
		/** search dock */
		QDockWidget* _searchDock;
		
		/** status bar widget */
		MenuLabel* _encodingLabel;
		MenuLabel* _formatLabel;
		QLabel* _editInfoLabel;
		QLabel* _fileInfoLabel;
		QLabel* _fileTypeLabel;
		
		/** recent files */
		enum { MaxRecentFiles = 5 };
		QAction *_recentFileActions[MaxRecentFiles];
		QAction *_recentFileSeparator;
		
		friend class Settings;
};

#endif // __MAINWINDOW_H__
