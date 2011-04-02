#ifndef __RLEXER_JAVA_H__
#define __RLEXER_JAVA_H__

#include "RLexerCpp.h"

class RLexerJava : public RLexerCPP
{
    Q_OBJECT

	public:
		//! Construct a RLexerJava with parent \a parent.  \a parent is typically
		//! the QsciScintilla instance.
		RLexerJava(QObject *parent = 0);

		//! Destroys the RLexerJava instance.
		virtual ~RLexerJava();

		   //! Returns the name of the language.
		const char *language() const;
	
		//! Returns the set of keywords for the keyword set \a set recognised
		//! by the lexer as a space separated string.
		const char *keywords(int set) const;
		
	private:
		RLexerJava(const RLexerJava &);
		RLexerJava &operator=(const RLexerJava &);
};

#endif //__RLEXER_JAVA_H__
