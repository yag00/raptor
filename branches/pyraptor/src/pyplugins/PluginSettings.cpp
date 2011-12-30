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

#include <QStringList>
#include "PluginSettings.h"

PluginSettings::PluginSettings(QObject* parent_)
	: QSettings (QSettings::IniFormat, QSettings::UserScope, "raptor", "plugins", parent_){

}
PluginSettings::~PluginSettings(){

}

QStringList PluginSettings::getPluginPaths(){
	return value("PyPlugin/Paths").toStringList();
}

void PluginSettings::setPluginPaths(const QStringList& paths_){
	setValue("PyPlugin/Paths", paths_);
}

bool PluginSettings::getEnableStatus(const QString& plugin_){
	return value("PyPlugin/Disable/" + plugin_, true).toBool();
}

void PluginSettings::setEnableStatus(const QString& plugin_, bool enable_){
	if(enable_){
		//remove from list
		if(contains("PyPlugin/Disable/" + plugin_))
			remove("PyPlugin/Disable/" + plugin_);
	}else{
		//add plugin to list
		setValue("PyPlugin/Disable/" + plugin_, false);
	}
}

bool PluginSettings::getLoadStatus(const QString& plugin_){
	return (not value("PyPlugin/DoNotLoad/" + plugin_, false).toBool());
}

void PluginSettings::setLoadStatus(const QString& plugin_, bool enable_){
	if(enable_){
		//remove from list
		if(contains("PyPlugin/DoNotLoad/" + plugin_))
			remove("PyPlugin/DoNotLoad/" + plugin_);
	}else{
		//add plugin to list
		setValue("PyPlugin/DoNotLoad/" + plugin_, true);
	}
}
