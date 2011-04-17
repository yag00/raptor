/**
 * This file is part of the raptor project.
 *
 * Copyright (C) 2011 Christophe Duvernois <christophe.duvernois@gmail.com>
 *
 * Raptor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Raptor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 */

#ifndef __RLEXER_XML_H__
#define __RLEXER_XML_H__

#include "RLexerHTML.h"

class QSettings;

//! \brief The RLexerXML class encapsulates the Scintilla XML lexer.
class RLexerXML : public RLexerHTML
{
    Q_OBJECT

	public:

		//! Construct a RLexerXML with parent \a parent.  \a parent is
		//! typically the QsciScintilla instance.
		RLexerXML(QObject *parent = 0);

		//! Destroys the RLexerXML instance.
		virtual ~RLexerXML();

		//! Returns the name of the language.
		const char *language() const;

		//! Returns the name of the lexer.  Some lexers support a number of
		//! languages.
		const char *lexer() const;

		//! Returns the foreground colour of the text for style number \a style.
		//!
		//! \sa defaultPaper()
		QColor defaultColor(int style) const;

		//! Returns the end-of-line fill for style number \a style.
		bool defaultEolFill(int style) const;

		//! Returns the font for style number \a style.
		QFont defaultFont(int style) const;

		//! Returns the background colour of the text for style number \a style.
		//!
		//! \sa defaultColor()
		QColor defaultPaper(int style) const;

		//! Returns the set of keywords for the keyword set \a set recognised
		//! by the lexer as a space separated string.
		const char *keywords(int set) const;

		//! Causes all properties to be refreshed by emitting the
		//! propertyChanged() signal as required.
		void refreshProperties();

		//! If \a allowed is true then scripts are styled.  The default is true.
		//!
		//! \sa scriptsStyled()
		void setScriptsStyled(bool styled);

		//! Returns true if scripts are styled.
		//!
		//! \sa setScriptsStyled()
		bool scriptsStyled() const;

	protected:
		//! The lexer's properties are read from the settings \a qs.  \a prefix
		//! (which has a trailing '/') should be used as a prefix to the key of
		//! each setting.  true is returned if there is no error.
		//!
		bool readProperties(QSettings &qs, const QString &prefix);

		//! The lexer's properties are written to the settings \a qs.
		//! \a prefix (which has a trailing '/') should be used as a prefix to
		//! the key of each setting.  true is returned if there is no error.
		//!
		bool writeProperties(QSettings &qs, const QString &prefix) const;

	private:
		void setScriptsProp();

		bool scripts;

	private:
		RLexerXML(const RLexerXML &);
		RLexerXML &operator=(const RLexerXML &);
};

#endif //__RLEXER_XML_H__
