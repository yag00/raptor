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

#ifndef __RLEXER_CPP_H__
#define __RLEXER_CPP_H__

#include <Qsci/qscilexercpp.h>


//! \brief The RLexerCPP class overload QsciRLexerCPP C++ lexer.
class RLexerCPP : public QsciLexerCPP 
{
    Q_OBJECT

	public:
		//! Construct a RLexerCPP with parent \a parent.  \a parent is typically
		//! the QsciScintilla instance.  \a caseInsensitiveKeywords is true if the
		//! lexer ignores the case of keywords.
		RLexerCPP(QObject *parent = 0, bool caseInsensitiveKeywords = false);

		//! Destroys the RLexerCPP instance.
		virtual ~RLexerCPP();

		//! Returns the set of keywords for the keyword set \a set recognised
		//! by the lexer as a space separated string.
		const char *keywords(int set) const;

		//! Returns the descriptive name for style number \a style.  If the
		//! style is invalid for this language then an empty QString is returned.
		//! This is intended to be used in user preference dialogs.
		QString description(int style) const;

		QColor defaultColor(int style) const;
		QFont defaultFont(int style) const;
		
	private:
		RLexerCPP(const RLexerCPP &);
		RLexerCPP &operator=(const RLexerCPP &);
};

#endif //__RLEXER_CPP_H__
