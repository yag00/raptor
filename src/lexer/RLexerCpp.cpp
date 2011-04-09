#include <QtGui>
#include "RLexerCpp.h"


RLexerCPP::RLexerCPP(QObject *parent_, bool caseInsensitiveKeywords_)
    : QsciLexerCPP(parent_, caseInsensitiveKeywords_){
}
RLexerCPP::~RLexerCPP(){
}

QColor RLexerCPP::defaultColor(int style) const {
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
		case RawString:
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
			
		case InactiveComment:
		case InactiveCommentLine:
		case InactiveNumber:
			return QColor(0x90, 0xb0, 0x90);

		case InactiveCommentDoc:
			return QColor(0xd0, 0xd0, 0xd0);

		case InactiveKeyword:
			return QColor(0x90, 0x90, 0xb0);

		case InactiveDoubleQuotedString:
		case InactiveSingleQuotedString:
		case InactiveRawString:
			return QColor(0xb0, 0x90, 0xb0);

		case InactivePreProcessor:
			return QColor(0xb0, 0xb0, 0x90);

		case InactiveOperator:
		case InactiveIdentifier:
		case InactiveGlobalClass:
			return QColor(0xb0, 0xb0, 0xb0);

		case InactiveUnclosedString:
			return QColor(0x00, 0x00, 0x00);

		case InactiveVerbatimString:
			return QColor(0x00, 0x7f, 0x00);

		case InactiveRegex:
			return QColor(0x7f, 0xaf, 0x7f);			
	}

    return QsciLexer::defaultColor(style);
}

// Returns the set of keywords.
const char *RLexerCPP::keywords(int set) const {
    if (set == 1)
        return						
			"bitand bitor break case catch compl const_cast continue "
            "default delete do dynamic_cast else false  for  goto if "
            "namespace new not not_eq operator or or_eq reinterpret_cast "
			"return sizeof static_cast switch this throw true try "
			"typedef typeid using while xor xor_eq NULL and and_eq";
			
	if (set == 2)
        return
  			"asm auto bool char class const double enum explicit "
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
QString RLexerCPP::description(int style) const {
    switch (style) {
		case Default:
			return tr("Default");
		case InactiveDefault:
			return tr("Inactive default");			
		case Comment:
			return tr("/* Block Comment */");
		case InactiveComment:
			return tr("/* Inactive Block Comment */");
		case CommentLine:
			return tr("// Line Comment");
		case InactiveCommentLine:
			return tr("// Inactive Line comment");
		case CommentDoc:
			return tr("/** Block Comment Doc */");
		case InactiveCommentDoc:
			return tr("/** Inactive Block Comment Doc */");			
		case CommentLineDoc:
			return tr("/// Line Comment Doc");	
		case InactiveCommentLineDoc:
			return tr("/// Inactive Line Comment Doc");			
		case Number:
			return tr("Number");
		case InactiveNumber:
			return tr("Inactive number");			
		case Keyword:
			return tr("Keyword");
		case InactiveKeyword:
			return tr("Inactive keyword");
		case DoubleQuotedString:
			return tr("Double-quoted string");
		case InactiveDoubleQuotedString:
			return tr("Inactive double-quoted string");			
		case SingleQuotedString:
			return tr("Single-quoted string");
		case InactiveSingleQuotedString:
			return tr("Inactive single-quoted string");			
		case UUID:
			return tr("IDL UUID");
		case InactiveUUID:
			return tr("Inactive IDL UUID");			
		case PreProcessor:
			return tr("Pre-processor block");
		case InactivePreProcessor:
			return tr("Inactive pre-processor block");
		case Operator:
			return tr("Operator");
		case InactiveOperator:
			return tr("Inactive operator");			
		case Identifier:
			return tr("Identifier");
		case InactiveIdentifier:
			return tr("Inactive identifier");			
		case UnclosedString:
			return tr("Unclosed string");
		case InactiveUnclosedString:
			return tr("Inactive unclosed string");			
		case VerbatimString:
			return tr("VerbatimString");		
		case Regex:
			return tr("Regex");
		case KeywordSet2:
			return tr("Type");
		case InactiveKeywordSet2:
			return tr("Inactive secondary keywords and identifiers");			
		case CommentDocKeyword:
			return tr("JavaDoc/Doxygen keyword");
		case InactiveCommentDocKeyword:
			return tr("Inactive JavaDoc/Doxygen keyword");			
		case CommentDocKeywordError:
			return tr("JavaDoc/Doxygen keyword error");
		case InactiveCommentDocKeywordError:
			return tr("Inactive JavaDoc/Doxygen keyword error");			
		case GlobalClass:
			return tr("Global classes and typedefs");
		case InactiveGlobalClass:
			return tr("Inactive global classes and typedefs");			
	}
    return QString();
}

// Returns the font of the text for a style.
QFont RLexerCPP::defaultFont(int style) const {
    QFont f;
#if defined(Q_OS_WIN)
	f = QFont("Courier New",10);
#else
	f = QFont("DejaVu Sans Mono",10);
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