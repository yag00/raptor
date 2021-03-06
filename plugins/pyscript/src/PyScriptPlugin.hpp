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

#ifndef __PY_SCRIPT_PLUGIN_H__
#define __PY_SCRIPT_PLUGIN_H__

#include <RaptorPlugin.hpp>

class QString;
class QMenu;
class QDockWidget;

class PluginEngine;

class PyScriptPlugin : public RaptorPlugin {
	Q_OBJECT
	Q_INTERFACES(RaptorPlugin)

	public:
		PyScriptPlugin();
		virtual ~PyScriptPlugin();
	
		/** @return the name of the plugin */
		virtual QString getName() const;
		/** the plugin menu
		 * @param preferedAreaInPluginMenu_ prefered menu area
		 * @return the plugin menu or null */
		virtual QMenu* getMenu(bool& preferedAreaInPluginMenu_) const;
		/** the plugin dock
		 * @param preferedArea_ prefered dock area
		 * @return the plugin dock or null */
		virtual QDockWidget* getDock(Qt::DockWidgetArea& preferedArea_) const;
	
	private slots:
		void manager();
	
	private:
		/** plugin menu */
		QMenu* _menu;
		/** script engine */
		PluginEngine* _engine;
		/** python console dock */
		QDockWidget* _pyConsoleDock;
};

#endif // __PY_SCRIPT_PLUGIN_H__
