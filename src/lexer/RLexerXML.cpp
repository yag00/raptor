#include <QSettings>
#include "RLexerXML.h"

RLexerXML::RLexerXML(QObject *parent) : RLexerHTML(parent), scripts(true) {
}

RLexerXML::~RLexerXML() {
}

const char *RLexerXML::language() const {
	return "XML";
}

const char *RLexerXML::lexer() const {
	return "xml";
}

QColor RLexerXML::defaultColor(int style) const {
	return RLexerHTML::defaultColor(style);
}

bool RLexerXML::defaultEolFill(int style) const {
	if (style == CDATA)
		return true;
	return RLexerHTML::defaultEolFill(style);
}

QFont RLexerXML::defaultFont(int style) const {
	QFont f;
#if defined(Q_OS_WIN)
	f = QFont("Courier New",10);
#else
	f = QFont("DejaVu Sans Mono",10);
#endif

	switch (style) {
		case XMLStart:
		case XMLEnd:
		case SGMLCommand:
			f.setBold(true);
			break;

		default:
			f = RLexerHTML::defaultFont(style);
	}

	return f;
}

const char *RLexerXML::keywords(int set) const {
	if (set == 6)
		return RLexerHTML::keywords(set);

	return 0;
}

QColor RLexerXML::defaultPaper(int style) const {
	switch (style) {

		case CDATA:
			return QColor(0xff,0xdf,0x00);

		case SGMLDefault:
		case SGMLCommand:
		case SGMLParameter:
		case SGMLDoubleQuotedString:
		case SGMLSingleQuotedString:
		case SGMLSpecial:
		case SGMLEntity:
		case SGMLComment:
			return QColor(0xef,0xef,0xff);

		case SGMLError:
			return QColor(0xff,0x66,0x66);

		case SGMLBlockDefault:
			return QColor(0xcc,0xcc,0xe0);
	}

	return RLexerHTML::defaultPaper(style);
}

void RLexerXML::refreshProperties() {
	setScriptsProp();
}

bool RLexerXML::readProperties(QSettings &qs, const QString &prefix) {
	int rc = QsciLexerHTML::readProperties(qs, prefix);

	scripts = qs.value(prefix + "scriptsstyled", true).toBool();

	return rc;
}

bool RLexerXML::writeProperties(QSettings &qs, const QString &prefix) const {
	int rc = QsciLexerHTML::writeProperties(qs, prefix);

	qs.setValue(prefix + "scriptsstyled", scripts);

	return rc;
}

bool RLexerXML::scriptsStyled() const {
	return scripts;
}

void RLexerXML::setScriptsStyled(bool styled) {
	scripts = styled;
	setScriptsProp();
}

void RLexerXML::setScriptsProp() {
	emit propertyChanged("lexer.xml.allow.scripts",(scripts ? "1" : "0"));
}
