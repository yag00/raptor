#include <QApplication>
#include <QtGui>
#include <qtsingleapplication.h>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
	Q_INIT_RESOURCE(ressources);

	QString message;
	for (int a = 1; a < argc; ++a) {
		message += argv[a];
		if (a < argc-1)
			message += " ";
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
