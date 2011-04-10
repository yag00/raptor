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

#ifndef __DOCUMENT_VIEW_H__
#define __DOCUMENT_VIEW_H__

#include <QTabWidget>

class QFileSystemWatcher;
class QString;
class QMenu;

class DocumentEditor;

class DocumentView : public QTabWidget {
    Q_OBJECT
	public:
		/** default constructor
		 * @param document_ initialize the view with an existing document
		 * @param parent_ */
		DocumentView(QFileSystemWatcher& watcher_, DocumentEditor * document_ = 0, QWidget * parent_ = 0);
		/** destructor */
		~DocumentView();

		/** @return the current document */
		DocumentEditor* currentDocument();
		/** return a document
		 * @param index_ document index
		 * @return the document */
		DocumentEditor* getDocument(int index_);
		/** get a document
		 * @param name_ path + filename of the document
		 * @return the document or null if doesn't exist */
		DocumentEditor* getDocument(const QString& name_);
		/** get a document index
		 * @param name_ path + filename of the document
		 * @return the index of the document or -1 if doesn't exist */
		int getDocumentIndex(const QString& name_);

		/** set the active document (do nothing if document doesn't exist
		 * @param document_ the document to be active
		 * @return true if the document_ exists and has been set to active */
		bool setActiveDocument(DocumentEditor* document_);
		/** set the active document (do nothing if document doesn't exist
		 * @param name_ of the document to be active
		 * @return true if the document_ exists and has been set to active */
		bool setActiveDocument(const QString& name_);
		
		/** set a context menu to each tab of the view
		 * @param menu_ context menu */
		void setContextMenu(QMenu* menu_);

		/** @return true if the view is active */
		bool isActive() const;

		/** @return number of new created document */
		int getNbNewCreatedDocument() const;

		/** check if the document physically exists
		 * @param file_ file to check
		 * @return true if exists */
		bool documentExists(const QString& file_);

		/** @return list of document */
		QList<DocumentEditor*> getDocuments();
		QStringList getDocumentNameList();

	signals:
		/**emit when a document changed */
		void documentChanged(DocumentEditor*);
		/**emit when a document selection changed */
		void selectionChanged(DocumentEditor*);
		/**emit when a document cursor position changed */
		void cursorPositionChanged(DocumentEditor*, int, int);	
		/** emit when the active status of the view changed */
		void activeStatusChanged(bool);
		/** emit when the view has no more document
		 * NOTE: view has to have at least one tab so when the last document is closed/moved the view 
		 * will automatically create a new document and emit the empty signal */
		void empty();
		/** emit when the last document is closed */
		//void destroyViewRequested();
		/** emit when a drag&drop drop event occurs */
		void documentMoveRequested(QWidget* src_, int srcIndex_, QWidget* dest_, int destIndex_);

	public slots:
		/** use to update all documents infos */
		void updateAllDocuments();

		/** create a new document */
		void newDocument();
		/** open a document
		 * @param file_ document to open */
		void openDocument(const QString& file_);
		/** open many documents
		 * @param files_ list of documents to open */
		void openDocument(const QStringList& files_);

		/** add a document to this view
		 * @param document_ */
		void addDocument(DocumentEditor* document_);
		/** insert a document to this view
		 * @param index_
		 * @param document_ */
		void insertDocument(int index_, DocumentEditor* document_);
		
		/** clone a document to this view
		 * @param document_ */
		void cloneDocument(DocumentEditor* document_);

		/** close the current document
		 * @return true if has been closed */
		bool closeCurrentDocument();
		/** close a document
		 * @param index_ of the document
		 * @return true if has been closed */
		bool closeDocument(int index_);
		/** close a document
		 * @param file_ document to close
		 * @return true if has been closed */
		bool closeDocument(const QString& file_);
		/** close all documents except the current one */
		void closeAllDocumentsExceptCurrent();
		/** remove and disconnect the document but do not destroy it
		 * @return removed document */
		DocumentEditor* removeDocumentAt(int index_);
		/** remove and disconnect the current document but do not destroy it
		 * @return removed document */
		DocumentEditor* removeCurrentDocument();
		
		/** close all document
		 * @return true if all documents have been closed */
		bool closeAll();
		/** save current document */
		void save();
		/** save current document as ...*/
		void saveAs();
		/** save a copy of current document as ...*/
		void saveACopyAs();
		/** save all document */
		void saveAll();
		/** print current document */
		void print();

	private slots:
		/** show a context menu for a tab */
		void showTabBarContextMenu(const QPoint &point_);
		
		/** use when the current tab(document) changed */
		void currentTabChanged(int);

		/** use when a document changed */
		void documentChanged();
		/** use when a document selection changed */
		void selectionChanged();
		/** use when a document cursor position changed */
		void cursorPositionChanged(int, int);
		
		/** active/desactive the view */
		void setActive(bool active_);
		
	private:
		/** connect document's signal to the view */
		void connectDocument(DocumentEditor* document_);
		/** remove the first new document of the view if there is one
		 * (the view must always have at least one tab, remove it if this tab is not wanted anymore)*/
		void removeFirstNewDocument();
		
	private:
		/** Reference on FileWatcher */
		QFileSystemWatcher& _watcher;
		
		/** context menu */
		QMenu* _documentContextMenu;
		/** count the new document created */
		int _docCounter;
		/** manager status */
		bool _isActive;

};

#endif /* __DOCUMENT_VIEW_H__ */
