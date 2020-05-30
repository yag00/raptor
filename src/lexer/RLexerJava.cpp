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

#include <QWidget>
#include "RLexerJava.h"


// The ctor.
RLexerJava::RLexerJava(QObject *parent) : RLexerCPP(parent) {
}

// The dtor.
RLexerJava::~RLexerJava() {
}


// Returns the language name.
const char *RLexerJava::language() const {
    return "Java";
}


// Returns the set of keywords.
const char *RLexerJava::keywords(int set) const {
	if (set == 1)
		return								
			"instanceof assert if else switch case default break goto return for "
			"while do continue new throw throws try catch finally this super extends "
			"implements import true false null ";
			
	if (set == 2)
        return
			"package transient strictfp void char short int long double float const "
			"static volatile byte boolean class interface native private protected "
			"public final abstract synchronized enum ";
	if (set == 3)
        return	
           " future generic inner operator outer rest var ";
		   
	return 0;
}

#if WAF
#include "RLexerJava.moc"
#endif

