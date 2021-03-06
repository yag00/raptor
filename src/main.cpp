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
#include <sstream>

#include <QLibraryInfo>
#include <QTranslator>
#include <QApplication>
#include <QDir>
#include <QMessageBox>

#include <qtsingleapplication.h>
#include <Qsci/qsciglobal.h>
#include "settings/Settings.h"
#include "about/ApplicationPath.h"
#include "about/Version.h"
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
		QString raptorversion = QString("%1 - %2\n").arg(PACKAGE_NAME).arg(PACKAGE_VERSION);
		QString buildOs = QString("Build for %1\n").arg(PACKAGE_OS);
		QString gcc = "Build with gcc " + Version::gccVersion();
		QString qtversion = QString("Build with Qt %1 (using Qt %2)\n").arg(QT_VERSION_STR).arg(qVersion());
#if defined(Q_OS_WIN)
		gcc += (" MinGW " + Version::mingwVersion() + "\n");
#else
		gcc += "\n";
#endif
		QString versionStr = raptorversion + buildOs + qtversion + gcc;
#if defined(Q_OS_WIN)
		QMessageBox::about(0, "Version", versionStr);
#else
		std::cout << versionStr.toStdString() << std::endl;
#endif
	}
	inline void qsciVersion(){
		version();
		std::cout << "Build on qscintilla " << QSCINTILLA_VERSION_STR << std::endl;
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

int main(int argc, char *argv[]) {
	QtSingleApplication app("ac0452da134c2a204d7b5a7f5bb516147d27ee84", argc, argv);	// sha1(raptor)

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
		if(qtTranslator.load("qt_" + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath))){
			app.installTranslator(&qtTranslator);
		}
	}
	
	MainWindow mainWin;
	app.installEventFilter(&mainWin);

	mainWin.handleMessage(message);
	mainWin.show();

	app.setActivationWindow(&mainWin, false);
	QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &mainWin, SLOT(handleMessage(const QString&)));

	return app.exec();
}
