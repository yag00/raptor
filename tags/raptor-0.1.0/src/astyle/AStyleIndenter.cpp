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