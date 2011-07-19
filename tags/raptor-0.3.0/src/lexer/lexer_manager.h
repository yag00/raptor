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

#ifndef __LEXER_MANAGER_H__
#define __LEXER_MANAGER_H__

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>

#include "../singleton.h"

class QActionGroup;
class QMenu;
class QsciLexer;
class DocumentEditor;

class LexerManager : public  Singleton<LexerManager>
{
	public:
		/** initialize a menu with all language
		 * @param menu_ menu to initialize*/
		void initialize(QMenu* menu_);
		/** connect language menu action to a slot
		 * @param receiver_ object to connect
		 * @param method_ slot */
		void connectTo(const QObject * receiver_, const char * method_);

		/** @return auto lexer from extension file association defined in settings */
		QsciLexer* getAutoLexer(DocumentEditor* document_);

		/** @return a sorted list of all available lexer */
		QStringList getAvailableLexer() const;

		/** set the lexers associations */
		void setAssociationList(const QMap<QString, QStringList>& associations_);
		/** set the invisible lexers */
		void setInvisibleLexers(const QStringList& lexers_);


		void update(QsciLexer* lexer_);

		/** Reset the properties of a lexer
		 * @param lexer_ The lexer to reset	*/
		static void resetLexer(QsciLexer* lexer_);
		/**	Set property for lexer to value
		 * @param property The property name
		 * @param lexer The lexer to modify
		 * @param value The value to set
		 * @return true if can set property else return false */
		static bool setLexerProperty(const QString& property_, QsciLexer* lexer_, const QVariant& value_);
		/** Return a lexer property value
		 * @param property_ The property to query
		 * @param lexer_ The lexer to query property from
		 * @return property */
		static QVariant lexerProperty(const QString& property_, QsciLexer* lexer_);
		/** Lexer factory : instanciate a new lexer (don't forget to delete it)
		 * @param name_ Name of lexer to instanciate
		 * @return new lexer */
		static QsciLexer* lexerFactory(const QString& name_, DocumentEditor* parent_ = 0);

	private:
		friend class Singleton<LexerManager>;

		LexerManager(QObject* parent_ = 0);
		~LexerManager();

		QActionGroup* _actionGroup;

		/** available lexer */
		QStringList _availableLexers;
		QStringList _invisibleLexers;

		/** associations */
		QMap<QString, QStringList> _associations;

};

#endif /* __LEXER_MANAGER_H__ */
