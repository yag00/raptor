#include <QString>
#include <QMenu>
#include <QDockWidget>
#include <QMessageBox>

#include "PluginHelloWorld.hpp"

HelloWorld::HelloWorld() : RaptorPlugin() {
	_menu = new QMenu(getName());
	QAction* action = _menu->addAction("Hello");
	connect(action, SIGNAL(triggered()), this, SLOT(hello()));
}

HelloWorld::~HelloWorld() {
	delete _menu;
}

QString HelloWorld::getName() const {
	return "HelloWorld";
}

QMenu* HelloWorld::getMenu(bool& preferedAreaInPluginMenu_) const {
	return _menu;
}

void HelloWorld::hello() {
	QMessageBox::information(0, "Hello", "Hello");
}

Q_EXPORT_PLUGIN2(raptor_helloworld, HelloWorld)


#if WAF
#include "RaptorPlugin.moc"
#include "PluginHelloWorld.moc"
#endif
