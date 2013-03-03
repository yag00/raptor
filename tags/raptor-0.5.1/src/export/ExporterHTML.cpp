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

#include <stdint.h>
#include <QIODevice>
#include <QFile>
#include <QTextStream>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>
#include "../document_editor.h"
#include "ExporterHTML.h"

ExporterHTML::ExporterHTML() : Exporter(){

}

ExporterHTML::~ExporterHTML(){

}

void ExporterHTML::write(const DocumentEditor& document_, const QString& file_){
	QFile file(file_);
	if(!file.open(QIODevice::WriteOnly)){
		return;
	}
	QTextStream stream(&file);
	
	//write header
	stream << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
	stream << "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n";
	stream << "<head>\n";
	stream << "<title>"<< document_.getName() << "</title>\n";
	stream << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n";

	//@todo js folding would be nice
				  
	//write css
	QsciLexer* lexer = document_.lexer();
	if(lexer){
		stream << "<style type=\"text/css\">\n";
		for ( int i = 0; i < 128; i++ )	{
			QString desc = lexer->description(i);
			if (!desc.isEmpty()) {
				if (i == QsciScintilla::STYLE_DEFAULT)
					stream << "span {\n";
				else
					stream << ".S" << i << " {\n";
				stream << "\tcolor: " << lexer->color(i).name() << ";\n";
				if(lexer->paper(i) != Qt::white){
					stream << "\tbackground: " << lexer->paper(i).name() << ";\n";
					stream << "\ttext-decoration: inherit;\n";
				}
				QFont font = lexer->font(i);
				stream << "\tfont-family: '" << font.family() << "';\n";
				if(font.bold())
					stream << "\tfont-weight: bold;\n";
				if(font.italic())
					stream << "\tfont-style: italic;\n";
				stream << "\tfont-size: " << font.pointSize() << "pt;\n";
				stream << "}\n";
			}
		}
		stream << "</style>\n";
	}
	
	stream << "</head>\n";

	if(lexer){
		if (lexer->paper(QsciScintilla::STYLE_DEFAULT) != Qt::white)
			stream << "<body bgcolor=\"" << lexer->paper(QsciScintilla::STYLE_DEFAULT).name() << "\">\n";
		else
			stream << "<body>\n";		
	}else
		stream << "<body>\n";
	
	//write txt data
	//code extracted from scite
	int tabSize = document_.tabWidth();
	int styleCurrent = -1;
	bool inStyleSpan = false;
	int column = 0;
	
	stream << "<span>";
	
	for (int i = 0; i < document_.text().length(); i++) {
		char ch = document_.getCharAt(i);
		int style = document_.getStyleAt(i);

		if (style != styleCurrent) {
			if (inStyleSpan) {
				stream << "</span>";
				inStyleSpan = false;
			}
			if (ch != '\r' && ch != '\n') {	// No need of a span for the EOL
				stream << "<span class=\"S" << style << "\">";
				inStyleSpan = true;
				styleCurrent = style;
			}
		}
		if (ch == ' ') {
				char prevCh = '\0';
				if (column == 0) {	// At start of line, must put a &nbsp; because regular space will be collapsed
					prevCh = ' ';
				}
				while (i < document_.text().length() && document_.getCharAt(i) == ' ') {
					if (prevCh != ' ') {
						stream << ' ';
					} else {
						stream << "&nbsp;";
					}
					prevCh = document_.getCharAt(i);
					i++;
					column++;
				}
				i--; // the last incrementation will be done by the for loop
		} else if (ch == '\t') {
			int ts = tabSize - (column % tabSize);
			for (int itab = 0; itab < ts; itab++) {
				if (itab % 2) {
					stream << ' ';
				} else {
					stream << "&nbsp;";
				}
			}
			column += ts;
		} else if (ch == '\r' || ch == '\n') {
			if (inStyleSpan) {
				stream << "</span>";
				inStyleSpan = false;
			}
			if (ch == '\r' && document_.getCharAt(i + 1) == '\n') {
				i++;	// CR+LF line ending, skip the "extra" EOL char
			}
			column = 0;
			stream << "<br />";

			styleCurrent = document_.getStyleAt(i + 1);
			stream << '\n';
			
			if (document_.getCharAt(i + 1) != '\r' && document_.getCharAt(i + 1) != '\n') {
				// We know it's the correct next style,
				// but no (empty) span for an empty line
				stream << "<span class=\"S" << styleCurrent << "\">";
				inStyleSpan = true;
			}
		} else {
			switch (ch) {
			case '<':
				stream << "&lt;";
				break;
			case '>':
				stream << "&gt;";
				break;
			case '&':
				stream << "&amp;";
				break;
			default:
				stream << ch;
			}
			column++;
		}
	}

	if (inStyleSpan) {
		stream << "</span>";
	}
	stream << "</span>";
	
	//write footer
	stream << "\n</body>\n</html>\n";
	stream.flush();
	file.close();
}
