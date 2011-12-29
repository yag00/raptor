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

#include <iostream>

#include <QDebug>
#include <QLibraryInfo>
#include <QTranslator>
#include <QApplication>
#include <QDir>

#ifdef PYTHON_SUPPORT
#include <PythonQt.h>
extern void PythonQt_init_QtCore(PyObject*);
extern void PythonQt_init_QtGui(PyObject*);
#endif

#include <qtsingleapplication.h>
#include <Qsci/qsciglobal.h>
#include "settings/Settings.h"
#include "about/ApplicationPath.h"
#include "mainwindow.h"

namespace {
	inline void displayOption(const std::string& shortOption, const std::string& fullOption, const std::string& desc){
		std::cout.flags(std::ios::right);
		std::cout.width(2);
		std::cout << "  ";

		std::cout.flags(std::ios::left);
		std::cout.width(6);
		if(shortOption.empty())
			std::cout << "";
		else
			std::cout << ("-" + shortOption + ",");

		std::cout << "--";
		std::cout.flags(std::ios::left);
		std::cout.width(20);
		std::cout << fullOption;
		std::cout << desc << std::endl;
	}

	inline void version(){
		std::cout << PACKAGE_NAME << " - " << PACKAGE_VERSION << std::endl;
#ifdef PACKAGE_OS
		std::cout << "Build for " << PACKAGE_OS << std::endl;
#endif
	}
	inline void qsciVersion(){
		version();
		std::cout << "build on qscintilla " << QSCINTILLA_VERSION_STR << std::endl;
	}
	inline void help(){
		std::cout << "Usage : " << std::endl;
		std::cout << "  raptor [args] <files...> - Edit files" << std::endl;
		std::cout << std::endl;
		std::cout << "Arguments : " << std::endl;
		displayOption("v", "version", "show version information and quit");
		displayOption("h", "help", "show this message and quit");
		displayOption("", "reset-lexers", "reset all lexers configuration to default");
		displayOption("", "reset-associations", "reset all lexers associations to default");
		std::cout << std::endl;
	}
	inline void resetLexer(){
		Settings settings;
		settings.remove("/Scintilla");
	}
	inline void resetAssocitation(){
		Settings settings;
		settings.remove("/LexerAssociation");
	}

	inline int parseOption(const QString& option){
		if(option == "-v" || option =="--version"){
			version();
			return 0;
		}
		if(option == "--qsci-version"){
			qsciVersion();
			return 0;
		}
		if(option == "--reset-lexers"){
			resetLexer();
			return 0;
		}
		if(option == "--reset-associations"){
			resetLexer();
			return 0;
		}
		help();
		return 0;
	}
}

extern void PythonQt_init_QtCore(PyObject*);
extern void PythonQt_init_QtGui(PyObject*);

int main(int argc, char *argv[]) {
	QtSingleApplication app("ac0452da134c2a204d7b5a7f5bb516147d27ee84-dev", argc, argv);	// sha1(raptor)

	QStringList args = QCoreApplication::arguments();
	args.pop_front();

	QString message;
	foreach(QString arg, args){
		if(arg[0] == '-'){
			parseOption(arg);
			return 0;
		}else{
			QDir dir(arg);
			message += dir.absolutePath();
			message += ";";
		}
	}

	//check if raptor is already running
	if (app.isRunning()){
		//raptor is running, send full message
		app.sendMessage(message);
		//quit this instance of raptor
		return 0;
	}

	Settings settings;
	QLocale::Language language = settings.getTranslation();
	QString locale = QLocale(language).name();

	QTranslator raptorTranslator;
	if(raptorTranslator.load("raptor_" + locale, ApplicationPath::translationPath())){
		app.installTranslator(&raptorTranslator);
		QTranslator qtTranslator;
		if(qtTranslator.load("qt_" + locale,	QLibraryInfo::location(QLibraryInfo::TranslationsPath))){
			app.installTranslator(&qtTranslator);
		}
	}

//#ifdef PYTHON_SUPPORT
	//qDebug() << "PYTHON START";
	PythonQt::init(PythonQt::IgnoreSiteModule); // | PythonQt::RedirectStdOut);
    PythonQt_init_QtCore(0);
    PythonQt_init_QtGui(0);	
	PythonQtObjectPtr mainModule = PythonQt::self()->getMainModule();
	mainModule.evalScript(QString("import sys\n"));
	//mainModule.evalScript(QString("from PythonQt import *\n"));
	
	// Allow the python system path to recognize QFile paths in the sys.path
	PythonQt::self()->setImporter(NULL);
	
	/*mainModule.evalScript("sys.path.append(':')\n");
	  mainModule.evalScript("import eyed3tagger\n");
	  PythonQtObjectPtr tag = mainModule.evalScript("eyed3tagger.EyeD3Tagger()\n", Py_eval_input);
	qDebug() << __LINE__ << " " << "tag : " << tag.isNull();
	  Q_ASSERT(!tag.isNull());
	  tag.call("setFileName", QVariantList() << "toto.mp3");
	  QVariant fn = tag.call("fileName", QVariantList());
		qDebug() << __LINE__ << " " << fn.toString();
	  Q_ASSERT(fn.toString() == QString("t.mp3"));
	
	 {
      // evaluate a python file embedded in executable as resource:
      mainModule.evalFile("eyed3tagger.py");
      // create an object, hold onto its reference
      PythonQtObjectPtr tag = mainModule.evalScript("EyeD3Tagger()\n", Py_eval_input);
      Q_ASSERT(!tag.isNull());
		 qDebug() << __LINE__ << " "  << "tag : " << tag.isNull();
      tag.call("setFileName", QVariantList() << "t.mp3");
      QVariant fn = tag.call("fileName", QVariantList());
        qDebug() << __LINE__ << " " << fn.toString();
      Q_ASSERT(fn.toString() == QString("t.mp3"));
      // tag goes out of scope, reference count decremented.
    }
	
	 {
      // evaluate a python file embedded in executable as resource:
      mainModule.evalFile("plugin.py");
      // create an object, hold onto its reference
      PythonQtObjectPtr tag = mainModule.evalScript("Plugin()\n", Py_eval_input);
      Q_ASSERT(!tag.isNull());
      tag.call("run", QVariantList() << "t.mp3");
      QVariant fn = tag.call("getName", QVariantList());
        qDebug()<< __LINE__ << " "  << fn.toString();
      // tag goes out of scope, reference count decremented.
    }
	// append the Qt resource root directory to the sys.path
	mainModule.evalScript(QString("sys.path.append('%1')\n").arg(QDir::currentPath()));
	mainModule.evalScript("import plugin\n");
	PythonQtObjectPtr tag = mainModule.evalScript("plugin.Plugin()\n", Py_eval_input);
	QVariant fn = tag.call("getName", QVariantList());
	tag.call("run", QVariantList());
	qDebug() << fn.toString();
	if(fn.toString() == QString("helloWorld"))
		qDebug() << "OK";
	else
		qDebug() << "FAIL";
	qDebug() << "PYTHON OK";*/
//#endif
	
	MainWindow mainWin;
	app.installEventFilter(&mainWin);

	mainWin.handleMessage(message);
	mainWin.show();

	app.setActivationWindow(&mainWin, false);
	QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &mainWin, SLOT(handleMessage(const QString&)));

	return app.exec();
}
