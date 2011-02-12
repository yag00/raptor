#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <vector>
#include <QMainWindow>
#include "ui_mainwindow.h"

class QAction;
class QLabel;
class QsciScintilla;

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
		
		/** Format Menu */
		void convertToWindowFormat();
		void convertToUnixFormat();
		void convertToMacFormat();
	
		/** Language Menu */
		void changeLanguage(QAction*);

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

		/** document manager */
		DocumentManager* _documentManager;
		
		/** search dock */
		QDockWidget* _searchDock;
		
		/** status bar widget */
		QLabel* _insLabel;
		QLabel* _majLabel;
		QLabel* _encodingLabel;
		QLabel* _formatLabel;
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
