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

#ifndef __PLUGIN_ENGINE_H__
#define __PLUGIN_ENGINE_H__

#include <PythonQt.h>
#include <QObject>

class QMenu;
class PyPlugin;

class PluginEngine : public QObject{

	Q_OBJECT

	public:
		PluginEngine(QMenu& pluginMenu_, QObject* parent_ = 0);
		~PluginEngine();

		PyPlugin* getPlugin(const QString& name_);

		/** @return the list of all avail plugin 
		 * the map contains the file and all plugins found into the file */
		QMap<QString, QStringList> getAvailablePluginList();
		/** @return the list of loaded plugin */
		QList<PyPlugin*> getPluginList();
	
	public slots:
		void loadPlugins();
		void dropPlugins();
	
	signals:
		void pluginExecuted();
		
	private:
		QMenu& _pluginMenu;

		PythonQtObjectPtr _module;
		PythonQtObjectPtr _loader;

		QMap<QString, PyPlugin*> _plugins;
		QMap<QString, QStringList> _availablePlugins;

};

#endif // __PLUGIN_ENGINE_H__
