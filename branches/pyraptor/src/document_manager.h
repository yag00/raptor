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

#ifndef __DOCUMENT_MANAGER_H__
#define __DOCUMENT_MANAGER_H__

#include <vector>
#include "widget/splitter.h"

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

		/** @return the file watcher */
		QFileSystemWatcher& getFileSystemWatcher();

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
		/** @return list of document by view */
		QList<DocumentEditor*> getDocuments(int view_);
		/** @return list of document (fullpath)*/
		QStringList getDocumentsNameList();
		/** @return list of document (fullpath) by view */
		QStringList getDocumentsNameList(int view_);
		/** @return number of active view */
		int getViewNumber() const;

	public slots:
		/** @{ */

		/** create a new document */
		void newDocument();
		/** open a document */
		void open();
		/** open a document
		 * @param file_ document to open */
		void open(const QString& file_);
		/** open many documents
		 * @param files_ list of documents to open */
		void open(QStringList& files_);
		/** open many documents in the specified view
		 * @param files_ list of documents to open
		 * @param view_ the view */
		void open(QStringList& files_, int view_);
		/** save the current document */
		void save();
		/** save the current document as */
		void saveAs();
		/** save a copy of the current document as */
		void saveACopyAs();
		/** save all documents */
		void saveAll();
		/** close current document */
		bool close();
		/** close all documents */
		bool closeAll();
		/** close all except active document */
		bool closeAllExceptCurrentDocument();
		/** close all documents if it is possible else nothing is done */
		bool closeAllIfPossible();
		/** reload current document */
		void reload();
		/** rename current document */
		void rename();

		/** print current document */
		void print();

		/** @} */


		/** @{ */

		/** cut the current document selection */
		void cut();
		/** copy the current document selection */
		void copy();
		/** pase into the current document */
		void paste();
		/** undo the last action for the current document */
		void undo();
		/** redo the last action for the current document */
		void redo();
		/** select all the current document */
		void selectAll();
		/** increase indentation for the current document selection */
		void increaseIndentation();
		/** decrease indentation for the current document selection */
		void decreaseIndentation();
		/** set autoindentation for current document */
		void setAutoIndentation(bool b_);
		/** show autoindentation guide for current document */
		void showIndentationGuides(bool b_);

		/** convert tabs to spaces using the current tab witdh value */
		void tabsToSpaces();
		/** convert spaces to tabs using the current tab witdh value */
		void spacesToTabs();
		/** remove extra space at the end of lines on the current document */
		void trimTrailingSpaces();
		/** Remove all extra spaces between words */
		void compressSpaces();

		/** convert selection to upper case for the current document */
		void convertSelectedTextToUpperCase();
		/** convert selection to lower case for the current document */
		void convertSelectedTextToLowerCase();
		/** duplicate the current line for the current document */
		void duplicateCurrentLine();
		/** copy the current line for the current document */
		void copyCurrentLine();
		/** cut the current line for the current document */
		void cutCurrentLine();
		/** delete the current line for the current document */
		void deleteCurrentLine();
		/** move up the current selection for the current document */
		void moveCurrentLineUp();
		/** move down the current selection for the current document */
		void moveCurrentLineDown();
		/** transpose current and previous lines */
		void transposeLine();
		/** delete the current word for the current document */
		void deleteCurrentWord();
		/** set current document in read only mode */
		void setReadOnly(bool b_);
		/** go to \a line_ */
		void gotoLine(int line_);

		/** astyle reindent current document */
		void reindentDocument();
		/** astyle reindent all open documents */
		void reindentOpenDocuments();

		/** toggle comment */
		void toggleComment();
		/** toggle block comment */
		void toggleBlockComment();

		/** switch to the source file associated to the current document
		 * select or open the file if exists, do nothing if no association */
		void switchDocumentSrc();

		/** @} */

		/** @{ */

		/** zoom in on current document */
		void zoomIn();
		/** zoom out on current document */
		void zoomOut();
		/** restore zoom on current document */
		void zoomRestore();
		/** show white space and tab for the current document */
		void showWhiteSpaceAndTab(bool b_);
		/** show end of line for the current document */
		void showEndOfLine(bool b_);
		/** show all(white space,tab and end of line) for the current document */
		void showAll(bool b_);

		/** fold/unfold all */
		void foldUnfoldAll();
		/** fold level x */
		void foldLevel(int level_);
		/** unfold level x */
		void unfoldLevel(int level_);

		/** @} */

		/** @{ */

		/** convert current document to to window eol */
		void convertToWindowFormat();
		/** convert current document to to unix eol */
		void convertToUnixFormat();
		/** convert current document to to mac eol */
		void convertToMacFormat();
		/** change the charset and reload the current document */
		void changeCharset(const QString& codec_);
		/** save current document with charset */
		void saveWithCharset(const QString& codec_);
		/** save current document as with charset */
		void saveWithCharsetAs(const QString& codec_);

		/** @} */

		/** @{ */

		/** set bookmark on current line */
		void toggleBookmark();
		/** remove all bookmarks */
		void removeAllBookmarks();
		/** go to next bookmarks */
		void nextBookmark();
		/** go to previous bookmarks */
		void prevBookmark();

		/** @} */

		/** @{ */

		/** set/change the language for the current document */
		void changeLanguage(const QString& language_);

		/** @} */

		/** @{ */

		/** start recording a macro */
		void startRecordingMacro();
		/** stop recording a macro */
		void stopRecordingMacro();
		/** run the current macro \a times_ */
		void runMacro(int times_ = 1);
		/** run the current macro until the end of file */
		void runMacroUntilEOF();
		/** @return the current macro */
		QString getCurrentMacro() const;
		//void loadMacro();
		//void saveMacro();

		/** @} */

		void unidiff();
		void quickUnidiff();
		void showUnidiff(const QString& diff_);

		/** send notification */
		void notify();

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

		/** use when watched file are externally modified */
		void watchedFileChanged(const QString& path_);

	signals:
		/**emit when documents have been successfully opened */
		void opened(QStringList);
		/**emit when documents have been successfully saved */
		void saved(QStringList);
		/** use when the active document changed */
		void activeDocumentChanged(DocumentEditor*);
		/**emit when a document selection changed */
		void selectionChanged(DocumentEditor*);
		/**emit when a document cursor position changed */
		void cursorPositionChanged(DocumentEditor*, int, int);
		/** emit on a new a status message */
		void statusMessage(QString);

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

		/** file watcher */
		QFileSystemWatcher* _watcher;
		/** file watcher notification list */
		QStringList _watcherNotification;
};

#endif // __DOCUMENT_MANAGER_H__
