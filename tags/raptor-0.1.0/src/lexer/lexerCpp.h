#ifndef __LEXER_CPP_H__
#define __LEXER_CPP_H__

#include <Qsci/qscilexercpp.h>


//! \brief The QsciLexerCPP class overload QsciLexerCPP C++ lexer.
class LexerCPP : public QsciLexerCPP
{
    Q_OBJECT

	public:
		//! Construct a LexerCPP with parent \a parent.  \a parent is typically
		//! the QsciScintilla instance.  \a caseInsensitiveKeywords is true if the
		//! lexer ignores the case of keywords.
		LexerCPP(QObject *parent = 0, bool caseInsensitiveKeywords = false);

		//! Destroys the LexerCPP instance.
		virtual ~LexerCPP();

		//! Returns the set of keywords for the keyword set \a set recognised
		//! by the lexer as a space separated string.
		const char *keywords(int set) const;

		//! Returns the descriptive name for style number \a style.  If the
		//! style is invalid for this language then an empty QString is returned.
		//! This is intended to be used in user preference dialogs.
		QString description(int style) const;

		QColor defaultColor(int style) const;
		QFont defaultFont(int style) const;
		
	private:
		LexerCPP(const LexerCPP &);
		LexerCPP &operator=(const LexerCPP &);
};

#endif //__LEXER_CPP_H__
