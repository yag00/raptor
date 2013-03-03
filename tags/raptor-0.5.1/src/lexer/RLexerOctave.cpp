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

#include "RLexerOctave.h"

RLexerOctave::RLexerOctave(QObject *parent) : RLexerMatlab(parent) {
}

RLexerOctave::~RLexerOctave(){
	
}

const char *RLexerOctave::language() const {
    return "Octave";
}

const char *RLexerOctave::lexer() const {
    return "octave";
}

const char *RLexerOctave::keywords(int set) const {
    if (set == 1)
        return
            "break case catch continue do else elseif end end_unwind_protect "
            "endfor endfunction endif endswitch endwhile for function "
            "global if otherwise persistent return switch try until "
            "unwind_protect unwind_protect_cleanup while";

    return 0;
}

#if WAF
#include "RLexerOctave.moc"
#endif

