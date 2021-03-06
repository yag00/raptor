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

#include <PythonQt.h>
#include <gui/PythonQtScriptingConsole.h>
#include <QDebug>
#include <QMenu>

#include "PyPlugin.h"
#include "PluginSettings.h"
#include "PluginEngine.h"

PluginEngine::PluginEngine(QMenu& pluginMenu_, QObject* parent_) : 
	QObject(parent_), _pluginMenu(pluginMenu_) {
	//_pydocmgr = new PyDocumentManager(*this, docMgr_, this);
	initialize();
}

PluginEngine::~PluginEngine(){
	dropPlugins();
	PythonQt::cleanup();
}

void PluginEngine::initialize(){
	//initialize python qt
	PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut, "PyRaptor");
	// Allow the python system path to recognize QFile paths in the sys.path
	PythonQt::self()->setImporter(0);
	
	_module = PythonQt::self()->getMainModule();
	_module.evalScript(QString("import sys\n"));

	_module.evalScript("sys.path.append(':/pyscript')\n");
	//load base plugin class
	_module.evalFile(":/pyscript/BasePlugin.py");
	//load plugin loader class
	_module.evalFile(":/pyscript/PluginLoader.py");
	
	_loader = _module.evalScript("PluginLoader()\n", Py_eval_input);
		
	/* other method but crash on import from PluginLoader (import os, ...)
	//add python ressource file to path
	_module.evalScript("sys.path.append(':/pyscript')\n");
	//import module
	_module.evalScript(QString("import BasePlugin\n"));
	_module.evalScript(QString("import PluginLoader\n"));
	
	_loader = _module.evalScript("PluginLoader.PluginLoader()\n", Py_eval_input);
	*/
	
	if(_loader.isNull()){
		qCritical() << "ERROR : Can't load plugin loader";
	}
	
	//load plugins
	loadPlugins();
	//add c++ object
	//PythonQt::self()->registerQObjectClassNames(QStringList() << "PyDocument");
	//_module.addObject("mgr", _pydocmgr);
}

/*
void PluginEngine::garbageCollect(PyDocument* pyDoc_){
	_pydoclist.push_back(pyDoc_);
}

void PluginEngine::remove(PyDocument* pyDoc_){
	_pydoclist.removeAll(pyDoc_);
}

void PluginEngine::aboutToExecutePlugin(){
	_tmplist.clear();
	_tmplist = _pydoclist;
}

void PluginEngine::cleanUp(){
	if(_pydoclist.isEmpty())
		return;
	int i = 0;
	while(i < _pydoclist.count()){
		if(!_tmplist.contains(_pydoclist[i])){
			PyDocument* doc = _pydoclist[i];
			remove(doc);
			_pydocmgr->deleteDocument(doc);
		}else{
			i++;
		}
	}
	_tmplist.clear();
}

void PluginEngine::cleanAll(){
	while(_pydoclist.count() != 0){
		PyDocument* doc = _pydoclist[0];
		remove(doc);
		_pydocmgr->deleteDocument(doc);
	}
	_pydoclist.clear();
}
*/

void PluginEngine::dropPlugins(){
	//cleanAll();
	for(QMap<QString, PyPlugin*>::iterator it = _plugins.begin(); it != _plugins.end(); ++it){
		PyPlugin* p = it.value();
		delete p;
	}
	_plugins.clear();
	_availablePlugins.clear();
}

void PluginEngine::loadPlugins(){
	dropPlugins();
	
	PluginSettings settings;
	QStringList pluginPaths = settings.getPluginPaths();
	
	//load plugin
	QVariant ret = _loader.call("load", QVariantList() << pluginPaths);
	QMap<QString, QVariant> pluginMap = ret.toMap();
	
	if(!pluginMap.isEmpty())
		_pluginMenu.addSeparator();
	
	for(QMap<QString, QVariant>::iterator it = pluginMap.begin(); it != pluginMap.end(); ++it){
		//qDebug() << it.key() << "\t: " << it.value().toStringList();
		QStringList plugins = it.value().toStringList();
		_availablePlugins[it.key()] = plugins;
		//add python plugin to file to path
		_module.evalFile(it.key());
		
		foreach(QString plugin, plugins){
			if(settings.getLoadStatus(plugin) == false)
				continue;

			//get plugin info
			QString pluginConstructor = plugin + "()\n";
			PythonQtObjectPtr pyPluginObj = _module.evalScript(pluginConstructor, Py_eval_input);
			if(pyPluginObj.isNull()){
				qDebug() << "Can't load plugin " << plugin << " from " << it.key();
				continue;
			}
			QMap<QString, QVariant>::iterator found = pluginMap.find(plugin);
			if(found != pluginMap.end()){
				qDebug() << "Already load a plugin named " << plugin << " !";
				continue;
			}
			
			PyPlugin* p = new PyPlugin(pyPluginObj, _pluginMenu, this);
			connect(p, SIGNAL(aboutToExecute()), this, SLOT(aboutToExecutePlugin()));
			connect(p, SIGNAL(executed()), this, SLOT(cleanUp()));
			connect(p, SIGNAL(executed()), this, SIGNAL(pluginExecuted()));
			_plugins[plugin] = p;
		}
	}	
}

PyPlugin* PluginEngine::getPlugin(const QString& name_){
	QMap<QString, PyPlugin*>::iterator found = _plugins.find(name_);
	if(found != _plugins.end())
		return found.value();
	else
		return 0;
}

QMap<QString, QStringList> PluginEngine::getAvailablePluginList(){
	return _availablePlugins;
}

QList<PyPlugin*> PluginEngine::getPluginList(){
	return _plugins.values();
}

PythonQtScriptingConsole* PluginEngine::getConsole(QWidget* parent_){
	PythonQtScriptingConsole* pyconsole = new PythonQtScriptingConsole(parent_, _module);
	connect(this, SIGNAL(pluginExecuted()), pyconsole, SLOT(externalUpdate()));
	return pyconsole;
}

#if WAF
#include "PluginEngine.moc"
#endif
