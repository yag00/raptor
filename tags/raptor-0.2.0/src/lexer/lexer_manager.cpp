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

#include <assert.h>
#include <QtGui>
#include <Qsci/qscilexer.h>
#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexerbatch.h>
#include <Qsci/qscilexercmake.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexercsharp.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qscilexerd.h>
#include <Qsci/qscilexerdiff.h>
#include <Qsci/qscilexerfortran.h>
#include <Qsci/qscilexerfortran77.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexeridl.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qscilexermakefile.h>
#include <Qsci/qscilexerpascal.h>
#include <Qsci/qscilexerperl.h>
#include <Qsci/qscilexerpostscript.h>
#include <Qsci/qscilexerpov.h>
#include <Qsci/qscilexerproperties.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerruby.h>
#include <Qsci/qscilexerspice.h>
#include <Qsci/qscilexersql.h>
#include <Qsci/qscilexertcl.h>
#include <Qsci/qscilexertex.h>
#include <Qsci/qscilexerverilog.h>
#include <Qsci/qscilexervhdl.h>
#include <Qsci/qscilexerxml.h>
#include <Qsci/qscilexeryaml.h>

#include "RLexerCpp.h"
#include "RLexerJava.h"
#include "RLexerHTML.h"
#include "RLexerXML.h"
#include "RLexerTxt2Tags.h"
#include "RLexerMatlab.h"
#include "RLexerOctave.h"

#include "../settings/settings_dialog.h"
#include "../document_editor.h"
#include "lexer_manager.h"

LexerManager::LexerManager(QWidget* parent_){
	(void)parent_;
	Settings settings;
	
	_availableLexers = settings.availableLanguages();

	//Load Lexer association
	setAssociationList(settings.getAssociations());
	setInvisibleLexers(settings.getInvisibleLexers());

	_actionGroup = 0;
}

LexerManager::~LexerManager(){

}


QStringList LexerManager::getAvailableLexer() const{
	return _availableLexers;
}

void LexerManager::initialize(QMenu* menu_){
	_actionGroup = new QActionGroup(menu_);
	QAction* action = menu_->addAction("Normal Text");
	action->setObjectName("lexer_0");
	action->setCheckable(true);
	_actionGroup->addAction(action);
	foreach(QString lexer, _availableLexers){
		action = menu_->addAction(lexer);
		action->setObjectName("lexer_" + lexer);
		action->setCheckable(true);
		_actionGroup->addAction(action);
	}
	_actionGroup->actions().first()->setChecked(true);
}

void LexerManager::connectTo(const QObject * receiver_, const char * method_){
	QObject::connect(_actionGroup, SIGNAL(triggered(QAction*)), receiver_, method_);
}

void LexerManager::setAssociationList(const QMap<QString, QStringList>& associations_){
	_associations = associations_;
}

void LexerManager::setInvisibleLexers(const QStringList& lexers_){
	_invisibleLexers = lexers_;
}

void LexerManager::resetLexer(QsciLexer* lexer_) {
	// cancel if no lexer
	if (!lexer_)
		return;
	// set default styles
	for ( int i = 0; i < 128; ++i )	{
		if ( !lexer_->description( i ).isEmpty() ) {
			lexer_->setColor( lexer_->defaultColor( i ), i );
			lexer_->setEolFill( lexer_->defaultEolFill( i ), i );
			lexer_->setFont( lexer_->defaultFont( i ), i );
			lexer_->setPaper( lexer_->defaultPaper( i ), i );
		}
	}
}

bool LexerManager::setLexerProperty( const QString& property_, QsciLexer* lexer_, const QVariant& value_){
	// cancel no property, no lexer or if variant is not valid
	if ( !lexer_ || property_.trimmed().isEmpty() || !value_.isValid() )
		return false;
	// if bool
	if ( value_.type() == QVariant::Bool )
		return QMetaObject::invokeMethod(lexer_, property_.toLocal8Bit().constData(), Q_ARG(bool, value_.toBool()));
	// if int
	else if ( value_.type() == QVariant::Int )
		return QMetaObject::invokeMethod( lexer_, property_.toLocal8Bit().constData(), Q_ARG(QsciLexerPython::IndentationWarning, (QsciLexerPython::IndentationWarning)value_.toInt()));
	// return default value
	return false;
}

QVariant LexerManager::lexerProperty(const QString& property_, QsciLexer* lexer_){
	// if no member orlexer_return null variant
	if ( !lexer_ || property_.isEmpty() )
		return QVariant();
	// get language
	const QString lng = QString( lexer_->language() ).toLower();
	// checking property_
	if ( property_ == "foldComments" ) {
		if ( lng == "bash" )
			return qobject_cast<QsciLexerBash*>(lexer_)->foldComments();
		else if ( lng == "css" )
			return qobject_cast<QsciLexerCSS*>(lexer_)->foldComments();
		else if ( lng == "d" )
			return qobject_cast<QsciLexerD*>(lexer_)->foldComments();
		else if ( lng == "perl" )
			return qobject_cast<QsciLexerPerl*>(lexer_)->foldComments();
		else if ( lng == "pov" )
			return qobject_cast<QsciLexerPOV*>(lexer_)->foldComments();
		else if ( lng == "python" )
			return qobject_cast<QsciLexerPython*>(lexer_)->foldComments();
		else if ( lng == "sql" )
			return qobject_cast<QsciLexerSQL*>(lexer_)->foldComments();
		else if ( lng == "vhdl" )
			return qobject_cast<QsciLexerVHDL*>(lexer_)->foldComments();
		else if ( lng == "javascript" )
			return qobject_cast<QsciLexerJavaScript*>(lexer_)->foldComments();
		else if ( lng == "java" )
			return qobject_cast<RLexerJava*>(lexer_)->foldComments();
		else if ( lng == "c#" )
			return qobject_cast<QsciLexerCSharp*>(lexer_)->foldComments();
		else if ( lng == "c++" )
			return qobject_cast<QsciLexerCPP*>(lexer_)->foldComments();
		else if ( lng == "pascal" )
			return qobject_cast<QsciLexerPascal*>(lexer_)->foldComments();
		else if ( lng == "yaml" )
			return qobject_cast<QsciLexerYAML*>(lexer_)->foldComments();
		else if ( lng == "verilog" )
			return qobject_cast<QsciLexerVerilog*>(lexer_)->foldComments();
		else if ( lng == "tcl" )
			return qobject_cast<QsciLexerTCL*>(lexer_)->foldComments();
	}
	else if ( property_ == "foldCompact" ) {
		if ( lng == "bash" )
			return qobject_cast<QsciLexerBash*>(lexer_)->foldCompact();
		else if ( lng == "css" )
			return qobject_cast<QsciLexerCSS*>(lexer_)->foldCompact();
		else if ( lng == "d" )
			return qobject_cast<QsciLexerD*>(lexer_)->foldCompact();
		else if ( lng == "html" )
			return qobject_cast<RLexerHTML*>(lexer_)->foldCompact();
		else if ( lng == "lua" )
			return qobject_cast<QsciLexerLua*>(lexer_)->foldCompact();
		else if ( lng == "perl" )
			return qobject_cast<QsciLexerPerl*>(lexer_)->foldCompact();
		else if ( lng == "pov" )
			return qobject_cast<QsciLexerPOV*>(lexer_)->foldCompact();
		else if ( lng == "properties" )
			return qobject_cast<QsciLexerProperties*>(lexer_)->foldCompact();
		else if ( lng == "sql" )
			return qobject_cast<QsciLexerSQL*>(lexer_)->foldCompact();
		else if ( lng == "vhdl" )
			return qobject_cast<QsciLexerVHDL*>(lexer_)->foldCompact();
		else if ( lng == "javascript" )
			return qobject_cast<QsciLexerJavaScript*>(lexer_)->foldCompact();
		else if ( lng == "java" )
			return qobject_cast<RLexerJava*>(lexer_)->foldCompact();
		else if ( lng == "c#" )
			return qobject_cast<QsciLexerCSharp*>(lexer_)->foldCompact();
		else if ( lng == "c++" )
			return qobject_cast<QsciLexerCPP*>(lexer_)->foldCompact();
		else if ( lng == "fortran" )
			return qobject_cast<QsciLexerFortran*>(lexer_)->foldCompact();
		else if ( lng == "fortran77" )
			return qobject_cast<QsciLexerFortran77*>(lexer_)->foldCompact();
		else if ( lng == "pascal" )
			return qobject_cast<QsciLexerPascal*>(lexer_)->foldCompact();
		else if ( lng == "postscript" )
			return qobject_cast<QsciLexerPostScript*>(lexer_)->foldCompact();
		else if ( lng == "xml" )
			return qobject_cast<RLexerXML*>(lexer_)->foldCompact();
		else if ( lng == "verilog" )
			return qobject_cast<QsciLexerVerilog*>(lexer_)->foldCompact();
	}
	else if ( property_ == "foldQuotes" ) {
		if ( lng == "python" )
			return qobject_cast<QsciLexerPython*>(lexer_)->foldQuotes();
	}
	else if ( property_ == "foldDirectives" ) {
		if ( lng == "pov" )
			return qobject_cast<QsciLexerPOV*>(lexer_)->foldDirectives();
	}
	else if ( property_ == "foldAtBegin" ) {
		if ( lng == "vhdl" )
			return qobject_cast<QsciLexerVHDL*>(lexer_)->foldAtBegin();
	}
	else if ( property_ == "foldAtParenthesis" ) {
		if ( lng == "vhdl" )
			return qobject_cast<QsciLexerVHDL*>(lexer_)->foldAtParenthesis();
	}
	else if ( property_ == "foldAtElse" ) {
		if ( lng == "cmake" )
			return qobject_cast<QsciLexerCMake*>(lexer_)->foldAtElse();
		else if ( lng == "d" )
			return qobject_cast<QsciLexerD*>(lexer_)->foldAtElse();
		else if ( lng == "vhdl" )
			return qobject_cast<QsciLexerVHDL*>(lexer_)->foldAtElse();
		else if ( lng == "javascript" )
			return qobject_cast<QsciLexerJavaScript*>(lexer_)->foldAtElse();
		else if ( lng == "java" )
			return qobject_cast<RLexerJava*>(lexer_)->foldAtElse();
		else if ( lng == "c#" )
			return qobject_cast<QsciLexerCSharp*>(lexer_)->foldAtElse();
		else if ( lng == "c++" )
			return qobject_cast<QsciLexerCPP*>(lexer_)->foldAtElse();
		else if ( lng == "postscript" )
			return qobject_cast<QsciLexerPostScript*>(lexer_)->foldAtElse();
		else if ( lng == "verilog" )
			return qobject_cast<QsciLexerVerilog*>(lexer_)->foldAtElse();
	}
	else if ( property_ == "foldAtModule" ) {
		if ( lng == "verilog" )
			return qobject_cast<QsciLexerVerilog*>(lexer_)->foldAtModule();
	}
	else if ( property_ == "foldPreprocessor" )	{
		if ( lng == "html" )
			return qobject_cast<RLexerHTML*>(lexer_)->foldPreprocessor();
		else if ( lng == "javascript" )
			return qobject_cast<QsciLexerJavaScript*>(lexer_)->foldPreprocessor();
		else if ( lng == "java" )
			return qobject_cast<RLexerJava*>(lexer_)->foldPreprocessor();
		else if ( lng == "c#" )
			return qobject_cast<QsciLexerCSharp*>(lexer_)->foldPreprocessor();
		else if ( lng == "c++" )
			return qobject_cast<QsciLexerCPP*>(lexer_)->foldPreprocessor();
		else if ( lng == "pascal" )
			return qobject_cast<QsciLexerPascal*>(lexer_)->foldPreprocessor();
		else if ( lng == "xml" )
			return qobject_cast<RLexerXML*>(lexer_)->foldPreprocessor();
		else if ( lng == "verilog" )
			return qobject_cast<QsciLexerVerilog*>(lexer_)->foldPreprocessor();
	}
	else if ( property_ == "stylePreprocessor" ) {
		if ( lng == "javascript" )
			return qobject_cast<QsciLexerJavaScript*>(lexer_)->stylePreprocessor();
		else if ( lng == "java" )
			return qobject_cast<RLexerJava*>(lexer_)->stylePreprocessor();
		else if ( lng == "c#" )
			return qobject_cast<QsciLexerCSharp*>(lexer_)->stylePreprocessor();
		else if ( lng == "c++" )
			return qobject_cast<QsciLexerCPP*>(lexer_)->stylePreprocessor();
	}
	else if ( property_ == "caseSensitiveTags" ) {
		if ( lng == "html" )
			return qobject_cast<RLexerHTML*>(lexer_)->caseSensitiveTags();
		else if ( lng == "xml" )
			return qobject_cast<RLexerXML*>(lexer_)->caseSensitiveTags();
	}
	else if ( property_ == "backslashEscapes" ) {
		if ( lng == "sql" )
			return qobject_cast<QsciLexerSQL*>(lexer_)->backslashEscapes();
	}
	else if ( property_ == "indentationWarning" ) {
		if ( lng == "python" )
			return qobject_cast<QsciLexerPython*>(lexer_)->indentationWarning();
	}
	// default return value
	return QVariant();
}

QsciLexer* LexerManager::lexerFactory(const QString& name_, DocumentEditor* parent_){
	QsciLexer* lexer = 0;
	if(name_ == "Bash")
		lexer =  (QsciLexer*)new QsciLexerBash(parent_);
	else if(name_ == "Batch")
		lexer =  (QsciLexer*)new QsciLexerBatch(parent_);
	else if(name_ == "CMake")
		lexer =  (QsciLexer*)new QsciLexerCMake(parent_);
	else if(name_ == "C")
		lexer =  (QsciLexer*)new RLexerCPP(parent_);
	else if(name_ == "C++")
		lexer =  (QsciLexer*)new RLexerCPP(parent_);
	else if(name_ == "C#")
		lexer =  (QsciLexer*)new QsciLexerCSharp(parent_);
	else if(name_ == "CSS")
		lexer =  (QsciLexer*)new QsciLexerCSS(parent_);
	else if(name_ == "D")
		lexer =  (QsciLexer*)new QsciLexerD(parent_);
	else if(name_ == "Diff")
		lexer =  (QsciLexer*)new QsciLexerDiff(parent_);
	else if(name_ == "Fortran")
		lexer =  (QsciLexer*)new QsciLexerFortran(parent_);
	else if(name_ == "Fortran77")
		lexer =  (QsciLexer*)new QsciLexerFortran77(parent_);
	else if(name_ == "HTML")
		lexer =  (QsciLexer*)new RLexerHTML(parent_);
	else if(name_ == "IDL")
		lexer =  (QsciLexer*)new QsciLexerIDL(parent_);
	else if(name_ == "Java")
		lexer =  (QsciLexer*)new RLexerJava(parent_);
	else if(name_ == "JavaScript")
		lexer =  (QsciLexer*)new QsciLexerJavaScript(parent_);
	else if(name_ == "Lua")
		lexer =  (QsciLexer*)new QsciLexerLua(parent_);
	else if(name_ == "Makefile")
		lexer =  (QsciLexer*)new QsciLexerMakefile(parent_);
	else if(name_ == "Matlab")
		lexer =  (QsciLexer*)new RLexerMatlab(parent_);
	else if(name_ == "Octave")
		lexer =  (QsciLexer*)new RLexerOctave(parent_);	
	else if(name_ == "Pascal")
		lexer =  (QsciLexer*)new QsciLexerPascal(parent_);
	else if(name_ == "Perl")
		lexer =  (QsciLexer*)new QsciLexerPerl(parent_);
	else if(name_ == "PostScript")
		lexer =  (QsciLexer*)new QsciLexerPostScript(parent_);
	else if(name_ == "POV")
		lexer =  (QsciLexer*)new QsciLexerPOV(parent_);
	else if(name_ == "Properties")
		lexer =  (QsciLexer*)new QsciLexerProperties(parent_);
	else if(name_ == "Python")
		lexer =  (QsciLexer*)new QsciLexerPython(parent_);
	else if(name_ == "Ruby")
		lexer =  (QsciLexer*)new QsciLexerRuby(parent_);
	else if(name_ == "Spice")
		lexer =  (QsciLexer*)new QsciLexerSpice(parent_);
	else if(name_ == "SQL")
		lexer =  (QsciLexer*)new QsciLexerSQL(parent_);
	else if(name_ == "Txt2Tags")
		lexer =  (QsciLexer*)new RLexerTxt2Tags(parent_);
	else if(name_ == "TCL")
		lexer =  (QsciLexer*)new QsciLexerTCL(parent_);
	else if(name_ == "TeX")
		lexer =  (QsciLexer*)new QsciLexerTeX(parent_);
	else if(name_ == "Verilog")
		lexer =  (QsciLexer*)new QsciLexerVerilog(parent_);
	else if(name_ == "VHDL")
		lexer =  (QsciLexer*)new QsciLexerVHDL(parent_);
	else if(name_ == "XML")
		lexer =  (QsciLexer*)new RLexerXML(parent_);
	else if(name_ == "YAML")
		lexer =  (QsciLexer*)new QsciLexerYAML(parent_);

	if(lexer){
		QFont f;
#if defined(Q_OS_WIN)
		f = QFont("Courier New",10);
#else
		f = QFont("DejaVu Sans Mono",10);
#endif
		lexer->setDefaultFont(f);
	}

	return lexer;
}

void LexerManager::update(QsciLexer* lexer_){
	QString name;
	if(lexer_ == 0)
		_actionGroup->actions().first()->setChecked(true);
	else
		name = lexer_->language();

	foreach(QAction* action, _actionGroup->actions()){
		bool v = _invisibleLexers.contains(action->text());
		action->setVisible(!v);
		if(action->text() == name)
			action->setChecked(true);
	}
}

QsciLexer* LexerManager::getAutoLexer(DocumentEditor* document_){
	QString filename = document_->getName();
	foreach(QString k, _associations.keys()){
		if(QDir::match(_associations.value(k), filename)){
			return lexerFactory(k, document_);
		}
	}
	return 0;
}
