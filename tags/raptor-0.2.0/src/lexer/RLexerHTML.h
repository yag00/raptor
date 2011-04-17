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

#ifndef __RLEXER_HTML_H__
#define __RLEXER_HTML_H__

#include <Qsci/qscilexerhtml.h>

//! \brief The RLexerHTML class overload QsciRLexerHTML C++ lexer.
class RLexerHTML : public QsciLexerHTML
{
	Q_OBJECT

	public:

		//! Construct a RLexerHTML with parent \a parent.  \a parent is
		//! typically the QsciScintilla instance.
		RLexerHTML(QObject *parent = 0);

		//! Destroys the RLexerHTML instance.
		virtual ~RLexerHTML();

		//! Returns the foreground colour of the text for style number \a style.
		//!
		//! \sa defaultPaper()
		QColor defaultColor(int style) const;

		//! Returns the font for style number \a style.
		QFont defaultFont(int style) const;

		//! Returns the background colour of the text for style number \a style.
		//!
		//! \sa defaultColor()
		QColor defaultPaper(int style) const;

		//! Returns the set of keywords for the keyword set \a set recognised
		//! by the lexer as a space separated string.
		const char *keywords(int set) const;

	private:
		RLexerHTML(const RLexerHTML &);
		RLexerHTML &operator=(const RLexerHTML &);
};

#endif //__RLEXER_HTML_H__
