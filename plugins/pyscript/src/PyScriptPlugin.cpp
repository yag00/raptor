#include <QString>
#include <QMenu>
#include <QDockWidget>

#include <gui/PythonQtScriptingConsole.h>

#include "PluginManager.h"
#include "PluginEngine.h"
#include "PyScriptPlugin.hpp"

PyScriptPlugin::PyScriptPlugin() : RaptorPlugin() {
	//create the plugin menu
	_menu = new QMenu(tr("Scripts"));
	QAction* actionManager = _menu->addAction(QIcon(":/pyplugin/plugins.png"), tr("Script Manager"));
	connect(actionManager, SIGNAL(triggered()), this, SLOT(manager()));
	QAction* actionReload = _menu->addAction(QIcon(":/images/restore.png"), tr("Reload"));	//icon availabe in raptor
	connect(actionReload, SIGNAL(triggered()), this, SLOT(loadScript()));
	_menu->addSeparator();
	QAction* actionPyConsole = _menu->addAction(QIcon(":/pyplugin/console.png"), tr("Console"));
		
	//create the python script engine
	_engine = new PluginEngine(*_menu, this);
	//create the python console Dock
	_pyConsoleDock = new QDockWidget(tr("PyConsole"));
	_pyConsoleDock->setWidget(_engine->getConsole(_pyConsoleDock));
	_pyConsoleDock->setObjectName(QString::fromUtf8("PyConsole"));
	_pyConsoleDock->hide();
	connect(actionPyConsole, SIGNAL(triggered()), _pyConsoleDock->toggleViewAction(), SLOT(trigger()));
}

PyScriptPlugin::~PyScriptPlugin() {
	delete _menu;
	delete _pyConsoleDock;
}

QString PyScriptPlugin::getName() const {
	return "PyScript";
}

QMenu* PyScriptPlugin::getMenu(bool& preferedAreaInPluginMenu_) const {
	preferedAreaInPluginMenu_ = false;
	return _menu;
}

QDockWidget* PyScriptPlugin::getDock(Qt::DockWidgetArea& preferedArea_) const {
	preferedArea_ = Qt::BottomDockWidgetArea;
	return _pyConsoleDock;
}

void PyScriptPlugin::manager() {
	PluginManager dlg(*_engine);
	dlg.exec();
}

Q_EXPORT_PLUGIN2(raptor_pyscript, PyScriptPlugin)


#if WAF
#include "RaptorPlugin.moc"
#include "PyScriptPlugin.moc"
#endif
