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

#ifndef __PLUGIN_SETTINGS_H__
#define __PLUGIN_SETTINGS_H__

#include <QSettings>

/** handle all plugins settings of the application */
class PluginSettings : public QSettings{

	Q_OBJECT

	public:
		PluginSettings(QObject* parent_ = 0);
		~PluginSettings();
		
		QStringList getPluginPaths();
		void setPluginPaths(const QStringList& paths_);
	
		bool getEnableStatus(const QString& plugin_);
		void setEnableStatus(const QString& plugin_, bool enable_);

		bool getLoadStatus(const QString& plugin_);
		void setLoadStatus(const QString& plugin_, bool enable_);
};

#endif // __PLUGIN_SETTINGS_H__
