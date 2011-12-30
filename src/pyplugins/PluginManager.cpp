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

#include "PyPlugin.h"
#include "PluginElement.h"
#include "PluginEngine.h"
#include "PluginManager.h"

PluginManager::PluginManager(PluginEngine& pluginEngine_, QWidget* parent_) : 
	QDialog(parent_), _pluginEngine(pluginEngine_) {
	//init ui
	setupUi(this);
		
	foreach(PyPlugin* plugin, _pluginEngine.getPluginList()){
		PluginElement* pel = new PluginElement(*plugin, this);
		QListWidgetItem* item = new QListWidgetItem(lwPlugins);
		item->setSizeHint(pel->sizeHint());
		item->setData(Qt::UserRole, plugin->getName());	//for filtering (by name)
		lwPlugins->setItemWidget(item, pel);
	}
	
	QMap<QString, QStringList> availablePlugin = _pluginEngine.getAvailablePluginList();
	for(QMap<QString, QStringList>::iterator it = availablePlugin.begin();
		it != availablePlugin.end(); ++it){
		foreach(QString pluginName, it.value()){
			QTreeWidgetItem* item = new QTreeWidgetItem(twAvailablePlugins);
			item->setText(0, pluginName);
			item->setText(1, it.key());
			item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
			item->setCheckState(0, Qt::Checked);
		}
	}
	connect(twAvailablePlugins, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(availablePluginsItemChanged(QTreeWidgetItem*, int)));
}

PluginManager::~PluginManager(){

}

void PluginManager::on_tbClear_clicked(){
	leFilter->clear();
}

void PluginManager::on_leFilter_textChanged(const QString& text_){
	for(int i = 0; i < lwPlugins->count(); i++){
		QListWidgetItem* item = lwPlugins->item(i);
		item->setHidden(!item->data(Qt::UserRole).toString().contains(text_, Qt::CaseInsensitive));
	}
}

void PluginManager::availablePluginsItemChanged(QTreeWidgetItem* item_, int column_){
	if(column_ == 0){
		if(item_->checkState(0) == Qt::Checked){
			qDebug() << item_->text(0) << " checked";
		}else{
			qDebug() << item_->text(0) << " not checked";
		}
	}
}	
