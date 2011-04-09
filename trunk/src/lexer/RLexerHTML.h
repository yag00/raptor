#ifndef __RLEXER_HTML_H__
#define __RLEXER_HTML_H__

#include <Qsci/qscilexerhtml.h>

//! \brief The RLexerHTML class overload QsciRLexerHTML C++ lexer.
class RLexerHTML : public QsciLexerHTML
{
	Q_OBJECT

	public:

		//! Construct a RLexerHTML with parent \a parent.  \a parent is
		//! typically the QsciScintilla instance.
		RLexerHTML(QObject *parent = 0);

		//! Destroys the RLexerHTML instance.
		virtual ~RLexerHTML();

		//! Returns the foreground colour of the text for style number \a style.
		//!
		//! \sa defaultPaper()
		QColor defaultColor(int style) const;

		//! Returns the font for style number \a style.
		QFont defaultFont(int style) const;

		//! Returns the background colour of the text for style number \a style.
		//!
		//! \sa defaultColor()
		QColor defaultPaper(int style) const;

		//! Returns the set of keywords for the keyword set \a set recognised
		//! by the lexer as a space separated string.
		const char *keywords(int set) const;

	private:
		RLexerHTML(const RLexerHTML &);
		RLexerHTML &operator=(const RLexerHTML &);
};

#endif //__RLEXER_HTML_H__
