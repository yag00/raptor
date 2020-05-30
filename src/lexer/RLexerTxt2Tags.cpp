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

#include <QtWidgets>
#include "RLexerTxt2Tags.h"


RLexerTxt2Tags::RLexerTxt2Tags(QObject *parent_) : QsciLexer(parent_){
}
RLexerTxt2Tags::~RLexerTxt2Tags(){
}

const char *RLexerTxt2Tags::language() const{
	return "Txt2Tags";
}

const char *RLexerTxt2Tags::lexer() const{
	return "txt2tags";
}

QString RLexerTxt2Tags::description(int style) const{
	switch (style) {
		case Default:
			return tr("Default");
		case Special:
			return tr("Special");
		case STRONG:
			return tr("** Bold **");
		case STRONG_2:
			return tr("strong (unused)");
		case EM1:
			return tr("// italic //");
		case EM2:
			return tr("__ underline __");
		case H1:
			return tr("Title H1");
		case H2:
			return tr("Title H2");
		case H3:
			return tr("Title H3");
		case H4:
			return tr("Title H4");
		case H5:
			return tr("Title H5");
		case H6:
			return tr("Title H6");
		case PRECHAR:
			return tr("prechar (unused)");
		case LIST:
			return tr("-: list");
		case LIST2:
			return tr("+ list");
		case BLOCKQUOTE:
			return tr("BLOCKQUOTE");
		case STRIKEOUT:
			return tr("-- strike out --");
		case HRULE:
			return tr("HRULE");
		case LINK:
			return tr("[ link ]");
		case CODE:
			return tr("| code |");
		case CODE2:
			return tr("`` code2 ``");
		case CODEBLOCK:
			return tr("``` codeblock");
		case COMMENT:
			return tr("% comment");
		case OPTION:
			return tr("OPTION");
		case PREPROC:
			return tr("%!preproc");
		case POSTPROC:
			return tr("%!postproc");
	}			
	return QString();
}

QColor RLexerTxt2Tags::defaultColor(int style) const{
	switch (style) {
		case Default:
			return QColor(0x0, 0x0, 0x0);
		case Special:
			return QColor(0x0, 0x0, 0x0);
		case STRONG:
			return QColor(0x44, 0x56, 0x75);
		case STRONG_2:
			return QColor(0x44, 0x56, 0x75);
		case EM1:
			return QColor(0x65, 0x3A, 0x39);
		case EM2:
			return QColor(0x38, 0x67, 0x42);
		case H1:
			return QColor(0xE2, 0x07, 0x0);
		case H2:
			return QColor(0xE2, 0x07, 0x0);
		case H3:
			return QColor(0xA8, 0xD1, 0x05);
		case H4:
			return QColor(0x81, 0x17, 0x04);
		case H5:
			return QColor(0x81, 0x17, 0x04);
		case H6:
			return QColor(0x81, 0x17, 0x04);
		case PRECHAR:
			return QColor(0x81, 0x17, 0x04);
		case LIST:
			return QColor(0xE3, 0x0, 0xEE);
		case LIST2:
			return QColor(0xE3, 0x0, 0xEE);
		case BLOCKQUOTE:
			return QColor(0x01, 0x5F, 0x52);
		case STRIKEOUT:
			return QColor(0x64, 0x4A, 0x9B);
		case HRULE:
			return QColor(0xff, 0x90, 0x1e);
		case LINK:
			return QColor(0x09, 0x30, 0xDE);
		case CODE:
			return QColor(0x0, 0x9f, 0x0);
		case CODE2:
			return QColor(0x0, 0x9f, 0x0);
		case CODEBLOCK:
			return QColor(0x0, 0x5f, 0x0);
		case COMMENT:
			return QColor(0x77, 0x77, 0x77);
		case OPTION:
			return QColor(0xC0, 0x03, 0x6E);
		case PREPROC:
			return QColor(0x84, 0x8B, 0x0);
		case POSTPROC:
			return QColor(0xC0, 0x56, 0x0);
	}			

    return QsciLexer::defaultColor(style);
}

QFont RLexerTxt2Tags::defaultFont(int style) const{
    QFont f;
#if defined(Q_OS_WIN)
	f = QFont("Courier New",10);
#else
	f = QFont("DejaVu Sans Mono",10);
#endif

    switch (style) {		
		case STRONG:
		case STRONG_2:
		case H1:
		case H2:
		case H3:
		case H4:
		case H5:
		case H6:
			f.setBold(true);
			break;
		
		case EM1:
		case LINK:
		case Special:
			f.setItalic(true);
			break;
		
		case EM2:
			f.setUnderline(true);
			break;
	}			
    return f;
}


#if WAF
#include "RLexerTxt2Tags.moc"
#endif

