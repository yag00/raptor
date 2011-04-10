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

#ifndef __RLEXER_JAVA_H__
#define __RLEXER_JAVA_H__

#include "RLexerCpp.h"

class RLexerJava : public RLexerCPP
{
    Q_OBJECT

	public:
		//! Construct a RLexerJava with parent \a parent.  \a parent is typically
		//! the QsciScintilla instance.
		RLexerJava(QObject *parent = 0);

		//! Destroys the RLexerJava instance.
		virtual ~RLexerJava();

		   //! Returns the name of the language.
		const char *language() const;
	
		//! Returns the set of keywords for the keyword set \a set recognised
		//! by the lexer as a space separated string.
		const char *keywords(int set) const;
		
	private:
		RLexerJava(const RLexerJava &);
		RLexerJava &operator=(const RLexerJava &);
};

#endif //__RLEXER_JAVA_H__
