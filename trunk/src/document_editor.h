#ifndef __DOCUMENT_EDITOR_H__
#define __DOCUMENT_EDITOR_H__

#include "widget/ScintillaExt.h"

class QString;
class QsciMacro;

class DocumentEditor : public ScintillaExt {
    Q_OBJECT
	public:
		/** default constructor
		 * @param parent_ */
		DocumentEditor(QWidget *parent_ = 0);
		/** copy constructor
		 * @param document_
		 * @param parent_ */
		DocumentEditor(DocumentEditor* document_, QWidget *parent_ = 0);

		/** destructor */
		virtual ~DocumentEditor();

		/** @return filename */
		QString getName() const;
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
		
	protected slots:
	
		void toggleBookmark(int margin_, int line_,	Qt::KeyboardModifiers state_);
				
	private:
		bool saveFile(const QString &fileName_);
		bool saveCopy(const QString &fileName_);
		
		/** @return true if document need a bom */
		bool needBOM();
		
		/** auto detect eol */
		void autoDetectEol();
		/** auto detect indentation */
		void autoDetectIndent();
		
		/** document path + fileName */
		QString _fullPath;
		/** document type */
		QString _type;
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
		
	//friend class DocumentView;
};

#endif /* __DOCUMENT_EDITOR_H__ */
