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
#include <QApplication>
#include <qtsingleapplication.h>
#include <Qsci/qsciglobal.h>
#include "settings/settings.h"
#include "mainwindow.h"

namespace {
	void displayOption(const std::string& shortOption, const std::string& fullOption, const std::string& desc){	
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
	
	void version(){
		std::cout << PACKAGE_NAME << " - " << PACKAGE_VERSION << std::endl;
	}
	void qsciVersion(){
		version();
		std::cout << "build on qscintilla " << QSCINTILLA_VERSION_STR << std::endl;
	}
	void help(){
		std::cout << "Usage : " << std::endl;
		std::cout << "  raptor [args] <files...> - Edit files" << std::endl;
		std::cout << std::endl;
		std::cout << "Arguments : " << std::endl;
		displayOption("v", "version", "show version information and quit");
		displayOption("h", "help", "show this message and quit");
		displayOption("", "reset-lexers", "reset all lexers configuration to default");
		std::cout << std::endl;
	}
	void resetLexer(){
		Settings settings;
		settings.remove("/Scintilla");
	}
}

int main(int argc, char *argv[]) {
	Q_INIT_RESOURCE(ressources);
	
	QString message;
	for (int a = 1; a < argc; ++a) {
		QString arg = argv[a];
		
		if(arg[0] == '-'){
			if(arg == "-v" || arg =="--version"){
				version();
				return 0;
			}
			if(arg == "--qsci-version"){
				qsciVersion();
				return 0;
			}
			if(arg == "--reset-lexers"){
				resetLexer();
				return 0;
			}			
			help();
			return 0;
		}else{
			message += arg;
			if (a < argc-1)
				message += ";";
		}
	}

	//QApplication app(argc, argv);
	QtSingleApplication app(argc, argv);
	if (app.sendMessage(message))
		return 0;

	MainWindow mainWin;
	app.installEventFilter(&mainWin);

	mainWin.handleMessage(message);
	mainWin.show();

	app.setActivationWindow(&mainWin, false);
	QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &mainWin, SLOT(handleMessage(const QString&)));

	return app.exec();
}
