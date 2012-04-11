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
#include <QMenu>
#include <QFileInfo>
#include "PluginSettings.h"
#include "PyPlugin.h"

enum PyActionType{
	NAME = 0,
	ICON = 1,
	SHORTCUT = 2,
	FUNCTION = 3,
	PARENT = 4,
};

PyPlugin::PyPlugin(PythonQtObjectPtr plugin_, QMenu& pluginMenu_, QObject* parent_) : 
	QObject(parent_), _plugin(plugin_) {
	_name = _plugin.call("name", QVariantList()).toString();
	_shortDescription = _plugin.call("shortDescription", QVariantList()).toString();
	_description = _plugin.call("description", QVariantList()).toString();
	_author = _plugin.call("author", QVariantList()).toString();
	_license = _plugin.call("license", QVariantList()).toString();
	_version = _plugin.call("version", QVariantList()).toString();
	_icon = _plugin.call("icon", QVariantList()).toString();

	QVariantList ret = _plugin.call("actions", QVariantList()).toList();
	
	//check if there is a menu or only one action
	if(ret.size() <= 1){
		//create the only action
		QVariantList action = ret[0].toList();
		QString name, icon, shortcut, function, parent;
		if(getActionInfo(action, name, icon, shortcut, function, parent) == false)
			return;
		if(_icon.isEmpty() && !icon.isEmpty())
			_icon = icon;
		QAction* a = pluginMenu_.addAction(QIcon(icon), name, this, SLOT(execute()));
		a->setData(function);
		_menu = a;
	}else{
		//create the plugin menu&submenus
		QMenu* menu = pluginMenu_.addMenu(_name);
		menu->setIcon(QIcon(_icon));
		_menu = menu;
		//intialize the tree map
		QMap<QString, Node> tree;
		QStringList treeRoot;
		tree.clear();
		foreach(QVariant variants, ret){
			QVariantList action = variants.toList();
			QString name, icon, shortcut, function, parent;
			if(getActionInfo(action, name, icon, shortcut, function, parent) == false)
				continue;

			tree[name].name = name;
			tree[name].icon = icon;
			tree[name].function = function;
			tree[name].parent = parent;
			
			if(parent.isEmpty())
				treeRoot << name;
			else
				tree[parent].children << name;
		}
		
		//create plugin menu
		foreach(QString node, treeRoot){
			createMenu(menu, tree[node], tree);
		}
	}
	
	setPluginMenuVisible(isEnabled());
}

PyPlugin::~PyPlugin(){
	//plugin destroy his menu
	//allow to completely remove the plugin during runtime
	//the root menu should be valid at this time
	delete _menu;
}

void PyPlugin::createMenu(QMenu* parent_, PyPlugin::Node& node_, QMap<QString, Node>& tree_){
	if(node_.children.isEmpty()){
		QAction* a = parent_->addAction(QIcon(node_.icon), node_.name, this, SLOT(execute()));
		a->setData(node_.function);
	}else{
		QMenu* submenu = parent_->addMenu(node_.name);
		foreach(QString nodeName, node_.children){
			Node& node = tree_[nodeName];
			createMenu(submenu, node, tree_);
		}
	}
}

bool PyPlugin::getActionInfo(QVariantList& action_, QString& name_, QString& icon_, 
	QString& shortcut_, QString& function_, QString& parent_){
	if(action_.isEmpty())
		return false;
	name_ = action_[NAME].toString();
	if(name_.isEmpty())
		return false;

	if(action_.size() > ICON)
		icon_ = action_[ICON].toString();
	if(action_.size() > SHORTCUT)
		shortcut_ = action_[SHORTCUT].toString();
	if(action_.size() > ICON)
		function_ = action_[FUNCTION].toString();
	if(action_.size() > ICON)
		parent_ = action_[PARENT].toString();
	return true;
}

void PyPlugin::execute(){
	emit aboutToExecute();
	QAction* action = qobject_cast<QAction*>(sender());
	QString function = action->data().toString();
	
	QVariant ret;
	if(function.isEmpty())
		ret = _plugin.call("run", QVariantList() << action->text());
	else
		ret = _plugin.call(function, QVariantList());
	//@todo use ret ???

	emit executed();
}

QString PyPlugin::getName() const {
	return _name;
}

QIcon PyPlugin::getIcon() const {
	QFileInfo f(_icon);
	if (f.exists())
		return QIcon(_icon);
	return QIcon(":/pyplugin/pyscript.png");
}

QString PyPlugin::getShortDescription() const {
	return _shortDescription;
}

QString PyPlugin::getAuthor() const {
	return _author;
}

QString PyPlugin::getLicense() const {
	return _license;
}

QString PyPlugin::getDescription() const {
	return _description;
}

QString PyPlugin::getVersion() const {
	return _version;
}

bool PyPlugin::isEnabled() const{
	PluginSettings settings;
	return settings.getEnableStatus(_name);
}

void PyPlugin::setEnabled(bool enable_){
	PluginSettings settings;
	settings.setEnableStatus(_name, enable_);
	setPluginMenuVisible(enable_);
}

void PyPlugin::setPluginMenuVisible(bool visible_){
	QAction* action = dynamic_cast<QAction*>(_menu);
	if(action != 0){
		action->setVisible(visible_);
	}else{
		QMenu* menu = dynamic_cast<QMenu*>(_menu);
		if(menu != 0){
			menu->menuAction()->setVisible(visible_);
		}else{
			qCritical() << "PARANOIA OWNS";
		}
	}
}

#if WAF
#include "PyPlugin.moc"
#endif
