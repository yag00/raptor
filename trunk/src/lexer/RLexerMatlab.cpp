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

#include "RLexerMatlab.h"

RLexerMatlab::RLexerMatlab(QObject *parent) : QsciLexerMatlab(parent) {
}

RLexerMatlab::~RLexerMatlab(){
	
}

const char *RLexerMatlab::keywords(int set) const {
	if ( set == 1 )
		return "break case catch classdef continue else elseif end for "
		       "function global if otherwise parfor persistent return "
		       "switch try while";
	return 0;
}

QColor RLexerMatlab::defaultColor(int style) const {
	switch (style) {
		case Default :
			return QColor(0x00, 0x00, 0x00);
		case Comment :
			return QColor(0x00, 0x80, 0x00);
		case Command :
			return QColor(0x80, 0x00, 0xFF);
		case Number :
			return QColor(0xFF, 0x80, 0x00);
		case Keyword :
			return QColor(0x00, 0x00, 0xFF);		
		case SingleQuotedString :
			return QColor(0x80, 0x80, 0x80);
		case Operator :
			return QColor(0x00, 0x00, 0x80);
		case Identifier :
			return QColor(0x00, 0x00, 0x00);
		case DoubleQuotedString :
			return QColor(0x80, 0x80, 0x80);
		default:
			return QsciLexer::defaultColor(style);
	}
}

QFont RLexerMatlab::defaultFont(int style) const{
    QFont f;
#if defined(Q_OS_WIN)
	f = QFont("Courier New",10);
#else
	f = QFont("DejaVu Sans Mono",10);
#endif

    switch (style) {		
		case Keyword :
		case Operator :
			f.setBold(true);
			break;
	}			
    return f;
}

#if WAF
#include "RLexerMatlab.moc"
#endif

