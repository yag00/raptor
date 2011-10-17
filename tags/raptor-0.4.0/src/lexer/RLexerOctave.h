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

#ifndef __RLEXER_OCTAVE_H__
#define __RLEXER_OCTAVE_H__

#include "RLexerMatlab.h"

class RLexerOctave : public RLexerMatlab {
	Q_OBJECT
	public:		
		RLexerOctave(QObject *parent = 0);
		virtual ~RLexerOctave();
	
		//! Returns the name of the language.
		const char *language() const;

		//! Returns the name of the lexer.  Some lexers support a number of
		//! languages.
		const char *lexer() const;

		//! Returns the set of keywords for the keyword set \a set recognised
		//! by the lexer as a space separated string.
		const char *keywords(int set) const;

	private:
		RLexerOctave(const RLexerOctave &);
		RLexerOctave &operator=(const RLexerOctave &);
};

#endif // __RLEXER_OCTAVE_H__
