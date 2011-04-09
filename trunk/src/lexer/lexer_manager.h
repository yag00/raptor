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
	
		
		void setAssociationList(QMap<QString, QStringList> associations_);
		
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

		LexerManager(QWidget* parent_ = 0);
		~LexerManager();
						
		QActionGroup* _actionGroup;
		
		/** available lexer */
		QStringList _availableLexer;
		QMap<QString, QString> _fileType;
		
		/** associations */
		QMap<QString, QStringList> _associations;

};

#endif /* __LEXER_MANAGER_H__ */
