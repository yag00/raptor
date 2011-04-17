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

#include <sstream>
#include <iterator>
#include <astyle.h>
#include <QString>
#include "AStyleIndenter.h"
#include "SourceIterator.h"

AStyleIndenter::AStyleIndenter(){

}

AStyleIndenter::~AStyleIndenter(){

}
	
void AStyleIndenter::setStyle(astyle::FormatStyle style_){
	_formatter.setFormattingStyle(style_);
}
	
QString AStyleIndenter::format(const QString& text_){
	std::istringstream iss(text_.toStdString());
	SStreamIterator<std::istringstream> streamIterator(&iss);
	_formatter.init(&streamIterator);
	
	std::ostringstream out;
	while (_formatter.hasMoreLines()) {
		out << _formatter.nextLine();
		if (_formatter.hasMoreLines())
			out << streamIterator.getOutputEOL();
	}
	return QString(out.str().c_str());
}