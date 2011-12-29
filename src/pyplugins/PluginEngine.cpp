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

#include <QDebug>
#include <QMenu>
#include "PyPlugin.h"
#include "PluginEngine.h"

PluginEngine::PluginEngine(QMenu& pluginMenu_, QObject* parent_) : 
	QObject(parent_),_pluginMenu(pluginMenu_), _module(PythonQt::self()->getMainModule()) {
	//add python ressource file to path
	_module.evalScript("sys.path.append(':/pyplugin')\n");
	//load base plugin class
	_module.evalFile(":/pyplugin/BasePlugin.py");
	//load plugin loader class
	_module.evalFile(":/pyplugin/PluginLoader.py");
	_loader = _module.evalScript("PluginLoader()\n", Py_eval_input);
	if(_loader.isNull()){
		//@todo critical error
		qDebug() << "ERROR : Can't load plugin loader";
		return;
	}
	//load plugins
	loadPlugins();
}

PluginEngine::~PluginEngine(){
	for(QMap<QString, PyPlugin*>::iterator it = _plugins.begin(); it != _plugins.end(); ++it){
		PyPlugin* p = it.value();
		delete p;
	}
	_plugins.clear();
}

void PluginEngine::loadPlugins(){			
	//@todo get plugin path
	QStringList pluginPaths;
	pluginPaths << "/home/chris/Dev/raptorpy";
	
	//load plugin
	QVariant ret = _loader.call("load", QVariantList() << pluginPaths);
	QMap<QString, QVariant> pluginMap = ret.toMap();
	
	if(!pluginMap.isEmpty())
		_pluginMenu.addSeparator();
	
	for(QMap<QString, QVariant>::iterator it = pluginMap.begin(); it != pluginMap.end(); ++it){
		//qDebug() << it.key() << "\t: " << it.value().toStringList();
		QStringList plugins = it.value().toStringList();
		
		//add python plugin to file to path
		_module.evalFile(it.key());
		
		foreach(QString plugin, plugins){
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
			_plugins[plugin] = p;
		}
	}	
}
