#include <QApplication>
#include <QtGui>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(ressources);	
    QApplication app(argc, argv);
    MainWindow mainWin;
	app.installEventFilter(&mainWin);
    mainWin.show();
    return app.exec();
}
