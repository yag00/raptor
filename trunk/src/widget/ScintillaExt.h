#ifndef __SCINTILLA_EXT_H__
#define __SCINTILLA_EXT_H__

#include <Qsci/qsciscintilla.h>

class ScintillaExt : public QsciScintilla {
    Q_OBJECT
	public:
	    //! This enum defines the different styles for the folding margin. overload QsciScintilla::FoldStyle 
		enum FoldStyleExt {
			//! Folding is disabled.
			NoFoldStyle,
			//! Plain folding style using plus and minus symbols.
			PlainFoldStyle,
			//! Circled folding style using circled plus and minus symbols.
			CircledFoldStyle,
			//! Boxed folding style using boxed plus and minus symbols.
			BoxedFoldStyle,
			//! Circled tree style using a flattened tree with circled plus and
			//! minus symbols and rounded corners.
			CircledTreeFoldStyle,
			//! Boxed tree style using a flattened tree with boxed plus and minus
			//! symbols and right-angled corners.
			BoxedTreeFoldStyle,
			//! Arrow folding style using arrow symbols.
			ArrowFoldStyle,
			//! Arrow tree style using a flattened tree with arrow symbols and right-angled corners.
			ArrowTreeFoldStyle
		};
		
		/** default constructor
		 * @param parent_ */
		ScintillaExt(QWidget *parent_ = 0);
		/** destructor */
		virtual ~ScintillaExt();

		/** @return eol string */
		QString getEol() const;
		/** @return format string */
		QString getFormat() const;
		/** @return format pixmap */
		QPixmap getFormatPixmap() const;

		/** common line/position functions */
		inline int linesVisible() const { return SendScintilla(SCI_LINESONSCREEN); }
		/** return current position */
		int getCurrentPosition() const;
		
	public slots:
		/** Set no selection foreground color */
		void setNoSelectionForegroundColor();
		/** enable or disable the display of line numbers in margin
		 * @param enable_ */
		void setLineNumbersMarginEnabled(bool enable_);

		/** indent the text */
		void increaseIndentation();
		/** unindent the text */
		void decreaseIndentation();

		/** remove all whitespaces at the end of each line */
		void doTrimTrailing();
		/** remove all whitespaces at the start and the end of each line
		 * and each sequence of internal whitespace are replaced with a single space*/
		void doSimplified();

		/** convert selected text to upper case */
		void convertSelectedTextToUpperCase();
		/** convert selected text to lower case */
		void convertSelectedTextToLowerCase();
		/** duplicate current line */
		void duplicateCurrentLine();
		/** duplicate current line */
		void duplicateSelection();
		/** duplicate selection or current line */
		void duplicateSelectionOrCurrentLine();
		/** copy current line */
		void copyCurrentLine();
		/** cut current line */
		void cutCurrentLine();
		/** delete current line */
		void deleteCurrentLine();
		/** move current line up */
		void moveCurrentLineUp();
		/** move current line down */
		void moveCurrentLineDown();
		/** delete current word */
		void deleteCurrentWord();
		
		/** define folding style 
		 * @param folding_ folding style
		 * @param foreground_ foreground color
		 * @param background_ background color 
		 * @param margin_ margin */
		void setFolding(FoldStyleExt folding_, const QColor& foreground_, const QColor& background_, int margin_ = 2);
	
		/** fold all block for the given level
		 * @param level_ the fold level */
		void foldLevel(int level_);
		/** unfold all block for the given level
		 * @param level_ the unfold level */
		void unfoldLevel(int level_);
		
	protected slots:
		/** update margin width */
		void updateLineNumberWidth();
		/** update margin width */
		void setLineNumbersMarginWidth(int i);
		
	protected:
		/** reimplement wheelEvent */
		virtual void wheelEvent(QWheelEvent *event_);
		/** reimplement paintEvent */
		virtual void paintEvent(QPaintEvent *event_);
		
		void setFoldMarker(int marknr, int mark, const QColor& foreground_, const QColor& background_);

		/** @return true if the string is a qualified word (letter or number sequence of char) */
		bool isQualifiedWord(const QString& str_) const;
		bool isWordChar(char ch) const;
};

#endif /* __SCINTILLA_EXT_H__ */
