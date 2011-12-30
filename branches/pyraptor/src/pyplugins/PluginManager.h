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

#ifndef __PLUGIN_MANAGER_H__
#define __PLUGIN_MANAGER_H__

#include <QDialog>
#include "ui_pluginmanager.h"

class PluginEngine;

class PluginManager : public QDialog, private Ui::PluginManager {
	
	Q_OBJECT

	public:
		PluginManager(PluginEngine& pluginEngine_, QWidget* parent_ = 0);
		~PluginManager();
	
	private slots:
		void on_tbClear_clicked();
		void on_leFilter_textChanged(const QString& text_);

		void availablePluginsItemChanged(QTreeWidgetItem* item_, int column_);
	
	private:
		PluginEngine& _pluginEngine;

};

#endif // __PLUGIN_MANAGER_H__
