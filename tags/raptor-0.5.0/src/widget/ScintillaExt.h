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

#ifndef __SCINTILLA_EXT_H__
#define __SCINTILLA_EXT_H__

#include <Qsci/qsciscintilla.h>

#define INDICATOR_SEARCH          0
#define INDICATOR_QUICK_SEARCH1   1
#define INDICATOR_QUICK_SEARCH2   2

#define INDICATOR_HIGHLIGHT1      3
#define INDICATOR_HIGHLIGHT2      4
#define INDICATOR_HIGHLIGHT3      5
#define INDICATOR_HIGHLIGHT4      6
#define INDICATOR_HIGHLIGHT5      7
#define INDICATOR_HIGHLIGHT6      8

/** this class extends the QsciScintilla  widget
 * with a lots of usefull function for text manipulation
 * or text highlighting */
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
		/** @return current position */
		int getCurrentPosition() const;
		/** @return current line */
		int getCurrentLine() const;
		/** @return the size of the selected text */
		int getSelectedTextSize() const;
		/** This collects the text between the positions pos1_ and pos2_
		 * If pos2_ is -1, text is returned to the end of the document */
		QString getTextRange(int pos1_, int pos2_);

		//highlighting slots
		void applyIndicator(int start_, int end_, int id_);
		void applyIndicator(const QString &text_, int id1_, int id2_ = -1);
		void clearIndicators(int id1_, int id2_ = -1);

		/** @return the style corresponding to the position */
		int getStyleAt(int position_) const;
		/** @return the char at the position */
		char getCharAt(int position_) const;
		
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
		/** reindent the text (using astyle) */
		void reindent();

		/** convert tabs to spaces using the current tab witdh value */
		void tabsToSpaces();
		/** convert spaces to tabs using the current tab witdh value */
		void spacesToTabs();
		/** remove all whitespaces at the end of each line */
		void trimTrailingSpaces();
		/** remove all whitespaces at the start and the end of each line
		 * and each sequence of internal whitespace are replaced with a single space*/
		void compressSpaces();

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
		/** transpose current and previous lines */
		void transposeLine();
		/** delete current word */
		void deleteCurrentWord();
		/** go to the specified line */
		void gotoLine(int line_, int pos_ = 0);

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

		/** do the same as setText() but do not clear the cancel action list and
		 * set the editor view same as before */
		void replaceAllText(const QString& text_);

	protected slots:
		/** update margin width */
		void updateLineNumberWidth();
		/** update margin width */
		void setLineNumbersMarginWidth(int i);

		/** selected text changed */
		void selectedTextChanged();

		// highlighting function
		void checkHighlight();

	protected:
		/** reimplement wheelEvent */
		virtual void wheelEvent(QWheelEvent *event_);
		/** reimplement paintEvent */
		virtual void paintEvent(QPaintEvent *event_);
		/** reimplement drag&drop Event */
		virtual void dropEvent(QDropEvent* event_);
		virtual void dragEnterEvent(QDragEnterEvent *event_);
		virtual void dragMoveEvent(QDragMoveEvent* event);
		virtual void dragLeaveEvent(QDragLeaveEvent* event);
		
	signals:
		/** emited if the document catch dropEvent (to open dropped file) */
		void fileDropped(QStringList);
		
	protected:
		void setFoldMarker(int marknr, int mark, const QColor& foreground_, const QColor& background_);

		// highlighting function
		void createIndicator(int id_, int type_, const QColor &color_, bool under_ = true);
		void highlightVisible(const QString &text_, int id1_, int id2_);

		/** @return true if the line is empty */
		bool isLineEmpty(int line_) const;
		/** @return true if the string is a qualified word (letter or number sequence of char) */
		bool isQualifiedWord(const QString& str_) const;
		bool isWordChar(char ch) const;

	private:
		/** highlight marker */
		int _HLID1, _HLID2;
		/** highlight text */
		QString _HLText;

};

#endif /* __SCINTILLA_EXT_H__ */
