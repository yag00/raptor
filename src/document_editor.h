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

#ifndef __DOCUMENT_EDITOR_H__
#define __DOCUMENT_EDITOR_H__

#include "ScintillaExt.h"

class QFileSystemWatcher;
class QString;
class QsciMacro;

class DocumentEditor : public ScintillaExt {
    Q_OBJECT
	public:
		/** default constructor
		 * @param parent_ */
		DocumentEditor(QFileSystemWatcher& watcher_, QWidget *parent_ = 0);
		/** copy constructor
		 * @param document_
		 * @param parent_ */
		DocumentEditor(DocumentEditor* document_, QWidget *parent_ = 0);

		/** destructor */
		virtual ~DocumentEditor();

		/** @return filename */
		QString getName() const;
		/** @return basename */
		QString getBaseName() const;
		/** @return filename extension */
		QString getSuffix() const;
		/** @return path with filename */
		QString getFullPath() const;
		/** @return path without filename */
		QString getPath() const;
		/** @return type */
		QString getType() const;

		/** check if the document path still exist
		 * @return false if path doesn't exist anymore and set the path to an empty string
		 * @return false if path is an empty string
		 * @return true if path exists */
		bool stillExist() const;

		/** @return true if the document is new and has never been saved yet */
		bool isNew() const;
		/** @return true if the document is cloned */
		bool isCloned() const;

		bool isNotified() const;
		void setNotified(bool notified_);

		/** enable/disable auto detection of eol
		 * @param enable_ enable/disable*/
		void setAutoDetectEol(bool enable_);
		/** enable/disable auto detection of indentation
		 * @param enable_ enable/disable*/
		void setAutoDetectIndent(bool enable_);

		const QList<int>& bookmarks() const { return _bookmarks; }
		QStringList bookmarkedLinesToStringList();
		void setBookmarks(const QStringList& lines);

		QsciMacro* getMacro() const;

		/** detach the clone */
		void detachClone();


		/** @return the codec used by the document */
		QString getCodec() const;
		/** set the codec to used for the document
		 * NOTE : changing the codec will relaod the document and reset the undo list
		 * @param codec_ codec name
		 * @return true if document was reloaded */
		bool setCodec(const QString& codec_);
		/** set the codec to used by default for the document
		 * do nothing if a codec as already be set
		 * @param codec_ codec name */
		void setDefaultCodec(const QString& codec_);

		//! This enum defines the different bom behaviour in case of unicode charset.
		enum UnicodeBomUseMode {
			//! leave the bom as is (ie do not add or remove the bom on the file)
			//! do not add a bom on new file
			BomLeaveAsIs,
			//! leave the bom as is (ie do not add or remove the bom on the file)
			//! add a bom on new file
			BomLeaveAsIsAddToNewFile,
			//! always add a bom on the file
			BomAlwaysAdd,
			//! always remove the bom on the file
			BomAlwaysRemove
		};
		/** set the unicode bom use mode
		 * @param bomMode_ bom mode */
		void setUnicodeBomUseMode(UnicodeBomUseMode bomMode_);
		/** enable/disable the charset auto detection
		 * @param autoDetect_ if true enable charset auto detection */
		void setCharsetAutoDetection(bool autoDetect_);

		/** enable/disable add new line on save
		 * @param addNewLine_ if true a new line will be added on save */
		void setAddNewLineOnSave(bool addNewLine_);
		/** enable/disable trim extra spaces on save
		 * @param trim_ if true trim extra spaces on save */
		void setTrimOnSave(bool trim_);

	protected:
		/** reimplement focusInEvent */
		virtual void focusInEvent(QFocusEvent *event_);
		/** reimplement focusOutEvent */
		virtual void focusOutEvent(QFocusEvent *event_);

	signals:
		/** emitted when focus changed with true if the document has the focus
		 * or false if the document lost it*/
		void focusChanged(bool);

	public slots:
		/** overload undo */
		virtual void undo();
		/** overload redo */
		virtual void redo();

		/** set language */
		void setLanguage(const QString &language_);

		/** toggle bookmark */
	    void toggleBookmark();
		/** remove all bookmarks */
		void removeAllBookmarks();
		/** navigate bookmark
		 * @param id_ bookmark id */
		bool navigateBookmark(int id_);
		/** go to the next bookmark */
		void nextBookmark();
		/** go to the previous bookmark */
		void prevBookmark();



		/** load a file
		 * @param fileName file name
		 * @return true if ok */
		bool load(const QString &fileName_);
		/** reload a file from disk
		 * @return true if ok */
		bool reload();
		/** rename file on the disk
		 * @return true if ok */
		bool rename();

		/** save file
		 * @return true if saved */
		bool save();
		/** save file as ...
		 * @return true if saved */
		bool saveAs();
		/** save a copy of the file as ...
		 * @return true if saved */
		bool saveACopyAs();
		/** save&convert the file with the given charset
		 * @return true if saved */
		bool saveWithCharset(const QString& codec_);
		/** save&convert the file as ...  with the given charset
		 * @return true if saved */
		bool saveWithCharsetAs(const QString& codec_);

		/** check if document is modified and ask if we need to save it
		 * @return true if we need to save it */
		bool maybeSave();

		/** print */
		void print(bool quick_ = false);
		/** quick print */
		void quickPrint();

		/** toggle comment
		 * @param lineCommentPrefered_ if true will try to comment a block using single comment line */
		void toggleComment(bool lineCommentPrefered_ = true);

	protected slots:

		void toggleBookmark(int margin_, int line_,	Qt::KeyboardModifiers state_);

	private:
		bool saveFile(const QString &fileName_);
		bool saveCopy(const QString &fileName_);

		/** @return true if document need a bom */
		bool needBOM();
		/** bom detection, update codec if successfull detection
		 *@param bom_ char array containing the first 4 bytes of the file
		 *@return true if successfull detection */
		bool detectBOM(const char* bom_);

		/** auto detect eol */
		void autoDetectEol();
		/** auto detect indentation */
		void autoDetectIndent();

		/** document path + fileName */
		QString _fullPath;

		/** hold if document is new */
		bool _isNew;


		/** bookmarks list */
		QList<int> _bookmarks;


		/** hold auto detect eol property */
		bool _autoDetectEol;
		/** hold auto detect indent property */
		bool _autoDetectIndent;

		/** Macro */
		QsciMacro* _macro;

		/** hold if document is cloned */
		bool _isCloned;
		/** clone */
		DocumentEditor* _clone;

		/** codec */
		QString _codec;
		UnicodeBomUseMode _bomMode;
		bool _hasBom;
		bool _charsetAutoDetect;

		/** storing options */
		bool _addNewLineOnSave;
		bool _trimOnSave;

		/** Reference on FileWatcher */
		QFileSystemWatcher& _watcher;
		bool _notified;
};

#endif /* __DOCUMENT_EDITOR_H__ */
