#include <QtGui>
#include "lexerCpp.h"


LexerCPP::LexerCPP(QObject *parent_, bool caseInsensitiveKeywords_)
    : QsciLexerCPP(parent_, caseInsensitiveKeywords_){
}
LexerCPP::~LexerCPP(){
}

QColor LexerCPP::defaultColor(int style) const {
    switch (style) {
		case Default:
			return QColor(0,0,0);

		case Comment:
		case CommentLine:
			return QColor(0x00,0x80,0x00);

		case CommentDoc:
		case CommentLineDoc:
			return QColor(0x00,0x80,0x80);

		case Number:
			return QColor(0xff,0x80,0x00);

		case Keyword:
			return QColor(0x00,0x00,0xff);

		case DoubleQuotedString:
		case SingleQuotedString:
			return QColor(0x80,0x80,0x80);

		case PreProcessor:
			return QColor(0x80,0x40,0x00);

		case Operator:
			return QColor(0x00,0x00,0x80);
			
		case UnclosedString:
			return QColor(0xff,0x00,0x80);

		case Regex:
			return QColor(0x3f,0x7f,0x3f);

		case KeywordSet2:
			return QColor(0x80,0x00,0xff);
			
		case CommentDocKeyword:
			return QColor(0x00,0x80,0x80);

		case CommentDocKeywordError:
			return QColor(0x80,0x40,0x20);
	}

    return QsciLexer::defaultColor(style);
}

// Returns the set of keywords.
const char *LexerCPP::keywords(int set) const {
    if (set == 1)
        return						
			"bitand bitor break case catch compl const_cast continue "
            "default delete do dynamic_cast else false  for  goto if "
            "namespace new not not_eq operator or or_eq reinterpret_cast "
			"return sizeof static_cast switch this throw true try "
			"typedef typeid using while xor xor_eq";
			
	if (set == 2)
        return
  			"asm auto  bool char class const double enum explicit "
			"extern float friend inline int long mutable private "
			"protected public register short signed static struct "
			"template typename union unsigned virtual void volatile "
			"uint8_t uint16_t uint32_t uint64_t int8_t int16_t int32_t int64_t ";
			
    if (set == 3)
        return
            "a addindex addtogroup anchor arg attention author b "
            "brief bug c class code date def defgroup deprecated "
            "dontinclude e em endcode endhtmlonly endif "
            "endlatexonly endlink endverbatim enum example "
            "exception f$ f[ f] file fn hideinitializer "
            "htmlinclude htmlonly if image include ingroup "
            "internal invariant interface latexonly li line link "
            "mainpage name namespace nosubgrouping note overload "
            "p page par param post pre ref relates remarks return "
            "retval sa section see showinitializer since skip "
            "skipline struct subsection test throw todo typedef "
            "union until var verbatim verbinclude version warning "
            "weakgroup $ @ \\ & < > # { }";

    return 0;
}

// Returns the user name of a style.
QString LexerCPP::description(int style) const {
    switch (style) {
		case Default:
			return tr("Default");
		case Comment:
			return tr("/* Block Comment */");
		case CommentLine:
			return tr("// Line Comment");
		case CommentDoc:
			return tr("/** Block Comment Doc */");
		case CommentLineDoc:
			return tr("/// Line Comment Doc");			
		case Number:
			return tr("Number");
		case Keyword:
			return tr("Keyword");
		case DoubleQuotedString:
			return tr("Double-quoted string");
		case SingleQuotedString:
			return tr("Single-quoted string");
		case UUID:
			return tr("IDL UUID");
		case PreProcessor:
			return tr("Pre-processor block");
		case Operator:
			return tr("Operator");
		case Identifier:
			return tr("Identifier");
		case UnclosedString:
			return tr("Unclosed string");
		case VerbatimString:
			return tr("VerbatimString");		
		case Regex:
			return tr("Regex");
		case KeywordSet2:
			return tr("Type");
		case CommentDocKeyword:
			return tr("JavaDoc/Doxygen keyword");
		case CommentDocKeywordError:
			return tr("JavaDoc/Doxygen keyword error");
		case GlobalClass:
			return tr("Global classes and typedefs");
	}
    return QString();
}

// Returns the font of the text for a style.
QFont LexerCPP::defaultFont(int style) const
{
    QFont f;
#if defined(Q_OS_WIN)
	f = QFont("Courier New",10);
#else
	f = QFont("Courier 10 Pitch",10);
#endif

    switch (style) {

		case Keyword:
		case Operator:
		case Regex:
		case CommentDocKeyword:
			f.setBold(true);
			break;
	}

    return f;
}