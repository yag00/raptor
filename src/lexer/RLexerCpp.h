#ifndef __RLEXER_CPP_H__
#define __RLEXER_CPP_H__

#include <Qsci/qscilexercpp.h>


//! \brief The RLexerCPP class overload QsciRLexerCPP C++ lexer.
class RLexerCPP : public QsciLexerCPP 
{
    Q_OBJECT

	public:
		//! Construct a RLexerCPP with parent \a parent.  \a parent is typically
		//! the QsciScintilla instance.  \a caseInsensitiveKeywords is true if the
		//! lexer ignores the case of keywords.
		RLexerCPP(QObject *parent = 0, bool caseInsensitiveKeywords = false);

		//! Destroys the RLexerCPP instance.
		virtual ~RLexerCPP();

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
		RLexerCPP(const RLexerCPP &);
		RLexerCPP &operator=(const RLexerCPP &);
};

#endif //__RLEXER_CPP_H__
