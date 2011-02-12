#ifndef __DOCUMENT_MANAGER_H__
#define __DOCUMENT_MANAGER_H__

#include <vector>
#include "widget/splitter.h"

class QSettings;
class QFileSystemWatcher;

class DocumentEditor;
class DocumentView;

class DocumentManager : public Splitter{
	Q_OBJECT

	public:
		/** default constructor
		 * @param document_ initialize the view with an existing document
		 * @param parent_ */
		DocumentManager(QWidget * parent_ = 0);
		/** destructor */
		~DocumentManager();
		
		/** @return active view */
		DocumentView* getActiveView() const;
		/** @return active document */
		DocumentEditor* getActiveDocument() const;
		
		/** set the active document (do nothing if document doesn't exist
		 * @param document_ the document to be active */
		void setActiveDocument(DocumentEditor* document_);
		/** set the active document (do nothing if document doesn't exist
		 * @param name_ of the document to be active */
		void setActiveDocument(const QString& name_);
		
		/** get a document
		 * @param name_ path + filename of the document
		 * @return the document or null if doesn't exist */
		DocumentEditor* getDocument(const QString& name_);
		
		/** check if the document physically exists
		 * @param name_ file to check
		 * @return true if exists */
		bool documentExists(const QString& name_);
		
		/** @return list of document */
		QList<DocumentEditor*> getDocuments();
		
	public slots:
		/** open a document
		 * @param file_ document to open */
		void openDocument(const QString& file_);
		/** open many documents
		 * @param files_ list of documents to open */
		void openDocument(QStringList& files_);
		
		/** save all document */
		bool saveAll();
		/** close all document */
		bool closeAll();
		/** close all except active document */
		bool closeAllExceptActiveDocument();
	
		/** save session */
		void saveSession();
		/** restore session */
		void restoreSession();

		/** save current session */
		void saveCurrentSession(QSettings& settings_);
		/** restore last session */
		void restoreLastSession(QSettings& settings_);
		
		/** astyle reindent current document */
		void reindentDocument();
		/** astyle reindent all open documents */
		void reindentOpenDocuments();
		
		/** Macro Menu */
		void startRecordingMacro();
		void stopRecordingMacro();
		void runMacro(int times_ = 1);
		void runMacroUntilEOF();
		QString getCurrentMacro() const;
		//void loadMacro();
		//void saveMacro();
		
		void diff();
		
	private slots:
		/** move current document to the next view */
		void moveDocument();
		/** move a document to the next view 
		 * @param src_ source widget (QTabBar)
		 * @param srcIndex_ destination index in the src_ tab bar 
		 * @param dest_ destination widget (QTabBar)
		 * @param destIndex_ destination index in the dest_ tab bar */
		void moveDocument(QWidget* src_, int srcIndex_, QWidget* dest_, int destIndex_);
		/** clone current document to the next view */
		void cloneDocument();

		/** update view */
		void updateView();
		void activeViewChanged(bool active_= true);
		/** destroy a view */
		void destroyView(DocumentView* view_ = 0);
		
	signals:
		/**emit when a document changed */
		void documentChanged(DocumentEditor*);
		/**emit when a document selection changed */
		void selectionChanged(DocumentEditor*);
		/**emit when a document cursor position changed */
		void cursorPositionChanged(DocumentEditor*, int, int);
	
	protected:
		/** create a new view 
		 * @param document_ initialize the view with the document */
		DocumentView* createDocumentView(DocumentEditor* document_ = 0);

	private:
		/** document view list */
		std::vector<DocumentView*> _viewList;
		/** active view */
		DocumentView* _activeView;
		
		/** Current Macro */
		QString _macro;
};

#endif // __DOCUMENT_MANAGER_H__
