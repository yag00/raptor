#include <iostream>
#include <QApplication>
#include <qtsingleapplication.h>
#include <Qsci/qsciglobal.h>
#include "mainwindow.h"

namespace {
	void version(){
		std::cout << PACKAGE_NAME << " - " << PACKAGE_VERSION << std::endl;
	}
	void qsciVersion(){
		version();
		std::cout << "build on qscintilla " << QSCINTILLA_VERSION_STR << std::endl;
	}
	void help(){
		std::cout << "Usage : raptor [args] [files ...] open specified file(s)" << std::endl;
		std::cout << std::endl;
		std::cout << "Arguments : " << std::endl;
		std::cout << "\t" << "-v, --version" << "\t" << "show version information and quit" << std::endl;
		std::cout << "\t" << "-h, --help" << "\t" << "show this message and quit" << std::endl;
		std::cout << std::endl;
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
