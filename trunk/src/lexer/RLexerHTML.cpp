#include "RLexerHTML.h"

RLexerHTML::RLexerHTML(QObject *parent) : QsciLexerHTML(parent) {
}

RLexerHTML::~RLexerHTML() {
}

QColor RLexerHTML::defaultColor(int style) const {
	switch (style) {
		//HTML
		case Default:
			return QColor(0x00,0x00,0x00);
		case HTMLComment:
			return QColor(0x00,0x80,0x00);
		case HTMLNumber:
			return QColor(0xff,0x00,0x00);

		case HTMLDoubleQuotedString:
		case HTMLSingleQuotedString:
			return QColor(0x80,0x00,0xff);

		case Tag:
		case XMLTagEnd:
		case Script:
			return QColor(0x00,0x00,0xff);

		case Attribute:
			return QColor(0xff,0x00,0x00);

		case UnknownTag:
		case UnknownAttribute:
			return QColor(0x80,0x00,0x00);

		case CDATA:
			return QColor(0xff,0x80,0x00);

		case HTMLValue:
			return QColor(0xff,0x00,0xff);

		case Entity:
			return QColor(0x80,0x00,0x80);

		//SGML
		case SGMLDefault:
			return QColor(0x80,0x00,0x00);

		case SGMLCommand:
			return QColor(0x00,0x00,0x80);

		case SGMLParameter:
			return QColor(0x00,0x66,0x00);

		case SGMLDoubleQuotedString:
		case SGMLError:
			return QColor(0x80,0x00,0x00);

		case SGMLSingleQuotedString:
			return QColor(0x99,0x33,0x00);

		case SGMLSpecial:
			return QColor(0x33,0x66,0xff);

		case SGMLEntity:
			return QColor(0x33,0x33,0x33);

		case SGMLBlockDefault:
			return QColor(0x00,0x00,0x66);

		case SGMLComment:
			return QColor(0x80,0x80,0x00);

		//XML
		case XMLStart:
		case XMLEnd:
			return QColor(0x00,0x00,0xff);

		//PHP
		case PHPDefault:
			return QColor(0x00,0x00,0x00);

		case PHPNumber:
			return QColor(0xff,0x80,0x00);

		case PHPComment:
		case PHPCommentLine:
			return QColor(0x00,0x80,0x00);

		case PHPOperator:
			return QColor(0x80,0x00,0xff);

		case PHPKeyword:
			return QColor(0x00,0x00,0xff);

		case PHPStart:
			return QColor(0xff,0x00,0x00);

		case PHPVariable:
		case PHPDoubleQuotedVariable:
			return QColor(0x00,0x00,0x80);

		case PHPSingleQuotedString:
		case PHPDoubleQuotedString:
			return QColor(0x80,0x80,0x80);

		//JavaScript
		case JavaScriptDefault:
		case JavaScriptWord:
		case JavaScriptSymbol:
			return QColor(0x00,0x00,0x00);

		case JavaScriptNumber:
			return QColor(0xFF,0x00,0x00);

		case JavaScriptDoubleQuotedString:
		case JavaScriptSingleQuotedString:
			return QColor(0x80,0x80,0x80);

		case JavaScriptStart:
			return QColor(0x7f,0x7f,0x00);

		case JavaScriptComment:
		case JavaScriptCommentLine:
			return QColor(0x00,0x80,0x00);

		case JavaScriptCommentDoc:
			return QColor(0x00,0x80,0x80);

		case JavaScriptKeyword:
			return QColor(0x00,0x00,0x80);

		//ASP/VB/Python
		case ASPJavaScriptDefault:
		case ASPJavaScriptWord:
		case ASPJavaScriptSymbol:
		case ASPVBScriptDefault:
		case VBScriptDefault:
			return QColor(0x00,0x00,0x00);

		case VBScriptKeyword:
		case VBScriptIdentifier:
		case VBScriptUnclosedString:
		case ASPVBScriptKeyword:
		case ASPVBScriptIdentifier:
		case ASPVBScriptUnclosedString:
			return QColor(0x00,0x00,0x80);

		case VBScriptNumber:
		case ASPVBScriptNumber:
			return QColor(0x00,0x80,0x80);

		case ASPJavaScriptNumber:
		case PythonNumber:
		case PythonFunctionMethodName:
		case ASPPythonNumber:
		case ASPPythonFunctionMethodName:
			return QColor(0x00,0x7f,0x7f);

		case ASPJavaScriptDoubleQuotedString:
		case ASPJavaScriptSingleQuotedString:
		case PythonDoubleQuotedString:
		case PythonSingleQuotedString:
		case ASPPythonDoubleQuotedString:
		case ASPPythonSingleQuotedString:
			return QColor(0x7f,0x00,0x7f);

		case OtherInTag:
		case VBScriptString:
		case ASPVBScriptString:
			return QColor(0x80,0x00,0x80);

		case PythonClassName:
		case ASPPythonClassName:
			return QColor(0x00,0x00,0xff);

		case ASPJavaScriptStart:
			return QColor(0x7f,0x7f,0x00);

		case ASPJavaScriptComment:
		case ASPJavaScriptCommentLine:
		case PythonComment:
		case ASPPythonComment:
			return QColor(0x00,0x7f,0x00);

		case ASPJavaScriptKeyword:
		case PythonKeyword:
		case ASPPythonKeyword:
			return QColor(0x00,0x00,0x7f);

		case ASPJavaScriptCommentDoc:
			return QColor(0x7f,0x7f,0x7f);

		case VBScriptComment:
		case ASPVBScriptComment:
			return QColor(0x00,0x80,0x00);

		case PythonStart:
		case PythonDefault:
		case ASPPythonStart:
		case ASPPythonDefault:
			return QColor(0x80,0x80,0x80);

		case PythonTripleSingleQuotedString:
		case PythonTripleDoubleQuotedString:
		case ASPPythonTripleSingleQuotedString:
		case ASPPythonTripleDoubleQuotedString:
			return QColor(0x7f,0x00,0x00);
	}
	return QsciLexer::defaultColor(style);
}

QFont RLexerHTML::defaultFont(int style) const {
	QFont f;
#if defined(Q_OS_WIN)
	f = QFont("Courier New",10);
#else
	f = QFont("DejaVu Sans Mono",10);
#endif

	switch (style) {
		case Default:
		case HTMLDoubleQuotedString:
		case HTMLSingleQuotedString:

		case PHPKeyword:

		case SGMLCommand:
		case PythonKeyword:
		case PythonClassName:
		case PythonFunctionMethodName:
		case PythonOperator:
		case ASPPythonKeyword:
		case ASPPythonClassName:
		case ASPPythonFunctionMethodName:
		case ASPPythonOperator:

		case JavaScriptDefault:
		case JavaScriptCommentDoc:
		case JavaScriptKeyword:
		case JavaScriptSymbol:

		case ASPJavaScriptDefault:
		case ASPJavaScriptCommentDoc:
		case ASPJavaScriptKeyword:
		case ASPJavaScriptSymbol:

		case VBScriptKeyword:
		case ASPVBScriptKeyword:
			f.setBold(true);
			break;
	}

	return f;
}

QColor RLexerHTML::defaultPaper(int style) const {
	switch (style) {
		case ASPAtStart:
			return QColor(0xff,0xff,0x00);

		case ASPStart:
		case CDATA:
			return QColor(0xff,0xdf,0x00);

		case PHPStart:
			return QColor(0xfd,0xf8,0xe3);

		case HTMLValue:
			return QColor(0xff,0xef,0xff);

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

		case JavaScriptDefault:
		case JavaScriptComment:
		case JavaScriptCommentLine:
		case JavaScriptCommentDoc:
		case JavaScriptNumber:
		case JavaScriptWord:
		case JavaScriptKeyword:
		case JavaScriptDoubleQuotedString:
		case JavaScriptSingleQuotedString:
		case JavaScriptSymbol:
			return QColor(0xf2,0xf4,0xff);

		case JavaScriptUnclosedString:
		case ASPJavaScriptUnclosedString:
			return QColor(0xbf,0xbb,0xb0);

		case JavaScriptRegex:
		case ASPJavaScriptRegex:
			return QColor(0xff,0xbb,0xb0);

		case ASPJavaScriptDefault:
		case ASPJavaScriptComment:
		case ASPJavaScriptCommentLine:
		case ASPJavaScriptCommentDoc:
		case ASPJavaScriptNumber:
		case ASPJavaScriptWord:
		case ASPJavaScriptKeyword:
		case ASPJavaScriptDoubleQuotedString:
		case ASPJavaScriptSingleQuotedString:
		case ASPJavaScriptSymbol:
			return QColor(0xdf,0xdf,0x7f);

		case VBScriptDefault:
		case VBScriptComment:
		case VBScriptNumber:
		case VBScriptKeyword:
		case VBScriptString:
		case VBScriptIdentifier:
			return QColor(0xef,0xef,0xff);

		case VBScriptUnclosedString:
		case ASPVBScriptUnclosedString:
			return QColor(0x7f,0x7f,0xff);

		case ASPVBScriptDefault:
		case ASPVBScriptComment:
		case ASPVBScriptNumber:
		case ASPVBScriptKeyword:
		case ASPVBScriptString:
		case ASPVBScriptIdentifier:
			return QColor(0xcf,0xcf,0xef);

		case PythonDefault:
		case PythonComment:
		case PythonNumber:
		case PythonDoubleQuotedString:
		case PythonSingleQuotedString:
		case PythonKeyword:
		case PythonTripleSingleQuotedString:
		case PythonTripleDoubleQuotedString:
		case PythonClassName:
		case PythonFunctionMethodName:
		case PythonOperator:
		case PythonIdentifier:
			return QColor(0xef,0xff,0xef);

		case ASPPythonDefault:
		case ASPPythonComment:
		case ASPPythonNumber:
		case ASPPythonDoubleQuotedString:
		case ASPPythonSingleQuotedString:
		case ASPPythonKeyword:
		case ASPPythonTripleSingleQuotedString:
		case ASPPythonTripleDoubleQuotedString:
		case ASPPythonClassName:
		case ASPPythonFunctionMethodName:
		case ASPPythonOperator:
		case ASPPythonIdentifier:
			return QColor(0xcf,0xef,0xcf);

		case PHPDefault:
		case PHPDoubleQuotedString:
		case PHPSingleQuotedString:
		case PHPKeyword:
		case PHPNumber:
		case PHPVariable:
		case PHPComment:
		case PHPCommentLine:
		case PHPDoubleQuotedVariable:
		case PHPOperator:
			return QColor(0xfe,0xfc,0xf5);
	}

	return QsciLexer::defaultPaper(style);
}

const char *RLexerHTML::keywords(int set) const {
	if (set == 1)
		return
			"a abbr acronym address applet area "
			"b base basefont bdo big blockquote body br button "
			"caption center cite code col colgroup "
			"dd del dfn dir div dl dt "
			"em "
			"fieldset font form frame frameset "
			"h1 h2 h3 h4 h5 h6 head hr html "
			"i iframe img input ins isindex "
			"kbd "
			"label legend li link "
			"map menu meta "
			"noframes noscript "
			"object ol optgroup option "
			"p param pre "
			"q "
			"s samp script select small span strike strong style "
			"sub sup "
			"table tbody td textarea tfoot th thead title tr tt "
			"u ul "
			"var "
			"xml xmlns "
			"abbr accept-charset accept accesskey action align "
			"alink alt archive axis "
			"background bgcolor border "
			"cellpadding cellspacing char charoff charset checked "
			"cite class classid clear codebase codetype color "
			"cols colspan compact content coords "
			"data datafld dataformatas datapagesize datasrc "
			"datetime declare defer dir disabled "
			"enctype event "
			"face for frame frameborder "
			"headers height href hreflang hspace http-equiv "
			"id ismap label lang language leftmargin link "
			"longdesc "
			"marginwidth marginheight maxlength media method "
			"multiple "
			"name nohref noresize noshade nowrap "
			"object onblur onchange onclick ondblclick onfocus "
			"onkeydown onkeypress onkeyup onload onmousedown "
			"onmousemove onmouseover onmouseout onmouseup onreset "
			"onselect onsubmit onunload "
			"profile prompt "
			"readonly rel rev rows rowspan rules "
			"scheme scope selected shape size span src standby "
			"start style summary "
			"tabindex target text title topmargin type "
			"usemap "
			"valign value valuetype version vlink vspace "
			"width "
			"text password checkbox radio submit reset file "
			"hidden image "
			"public !doctype";

	if (set == 2)
		//return QsciLexerJavaScript::keywordClass;
		return
			"abstract boolean break byte case catch char class const continue "
			"debugger default delete do double else enum export extends final "
			"finally float for function goto if implements import in instanceof "
			"int interface long native new package private protected public "
			"return short static super switch synchronized this throw throws "
			"transient try typeof var void volatile while with";

	if (set == 3)
		return
			// Move these to QsciLexerVisualBasic when we
			// get round to implementing it.
			"and begin case call continue do each else elseif end "
			"erase error event exit false for function get gosub "
			"goto if implement in load loop lset me mid new next "
			"not nothing on or property raiseevent rem resume "
			"return rset select set stop sub then to true unload "
			"until wend while with withevents attribute alias as "
			"boolean byref byte byval const compare currency date "
			"declare dim double enum explicit friend global "
			"integer let lib long module object option optional "
			"preserve private property public redim single static "
			"string type variant";

	if (set == 4)
		//return QsciLexerPython::keywordClass;
		return
			"and as assert break class continue def del elif else except exec "
			"finally for from global if import in is lambda None not or pass "
			"print raise return try while with yield";

	if (set == 5)
		return
			"and argv as argc break case cfunction class continue "
			"declare default do die array "
			"echo else elseif empty enddeclare endfor endforeach "
			"endif endswitch endwhile e_all e_parse e_error "
			"e_warning eval exit extends "
			"false for foreach function global "
			"http_cookie_vars http_get_vars http_post_vars "
			"http_post_files http_env_vars http_server_vars "
			"if include include_once list new not null "
			"old_function or "
			"parent php_os php_self php_version print "
			"require require_once return "
			"static switch stdclass this true var xor virtual "
			"while "
			"__file__ __line__ __sleep __wakeup";

	if (set == 6)
		return "ELEMENT DOCTYPE ATTLIST ENTITY NOTATION";

	return 0;
}