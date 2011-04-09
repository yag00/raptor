#ifndef __RLEXER_TXT2TAGS_H__
#define __RLEXER_TXT2TAGS_H__

#include <Qsci/qscilexer.h>


//! \brief The RLexerTxt2Tags class overload QsciRLexerTxt2Tags C++ lexer.
class RLexerTxt2Tags : public QsciLexer
{
    Q_OBJECT

	public:
	    //! This enum defines the meanings of the different styles used by the
		//! Properties lexer.
		enum {
			//! Default
			Default = 0,

			//! Special
			Special = 1,

			//! STRONG
			STRONG = 2,

			//! STRONG 2 (not used)
			STRONG_2 = 3,

			//! EM1 (italic)
			EM1 = 4,

			//! EM2 (underline)
			EM2 = 5,

			//! H1
			H1 = 6,

			//! H2
			H2 = 7,

			//! H3
			H3 = 8,

			//! H4
			H4 = 9,

			//! H5
			H5 = 10,

			//! H6
			H6 = 11,

			//! prechar (not used)
			PRECHAR = 12,

			//! list
			LIST = 13,

			//! list
			LIST2 = 14,

			//! BLOCKQUOTE
			BLOCKQUOTE = 15,

			//! STRIKEOUT
			STRIKEOUT = 16,

			//! HRULE
			HRULE = 17,

			//! LINK
			LINK = 18,

			//! CODE
			CODE = 19,

			//! CODE2
			CODE2 = 20,

			//! CODEBLOCK
			CODEBLOCK = 21,

			//! COMMENT
			COMMENT = 22,

			//! OPTION
			OPTION = 23,

			//! PREPROC
			PREPROC = 24,

			//! POSTPROC
			POSTPROC = 25,
		};
			
		//! Construct a RLexerTxt2Tags with parent \a parent.  \a parent is typically
		//! the QsciScintilla instance.  \a caseInsensitiveKeywords is true if the
		//! lexer ignores the case of keywords.
		RLexerTxt2Tags(QObject *parent = 0);

		//! Destroys the RLexerTxt2Tags instance.
		virtual ~RLexerTxt2Tags();

		//! Returns the name of the language.  It must be re-implemented by a
		//! sub-class.
		virtual const char *language() const;

		//! Returns the name of the lexer.  If 0 is returned then the lexer's
		//! numeric identifier is used.  The default implementation returns 0.
		//!
		//! \sa lexerId()
		virtual const char *lexer() const;
	
	    //! Returns the descriptive name for style number \a style.  If the
		//! style is invalid for this language then an empty QString is returned.
		//! This is intended to be used in user preference dialogs.
		virtual QString description(int style) const;
		
		//! Returns the default text colour for style number \a style.
		virtual QColor defaultColor(int style) const;

		//! Returns the default font for style number \a style.
		virtual QFont defaultFont(int style) const;
		
	private:
		RLexerTxt2Tags(const RLexerTxt2Tags &);
		RLexerTxt2Tags &operator=(const RLexerTxt2Tags &);
};

#endif //__RLEXER_TXT2TAGS_H__
