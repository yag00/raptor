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

#ifndef __PY_PLUGIN_H__
#define __PY_PLUGIN_H__

#include <PythonQt.h>
#include <QObject>

class QMenu;

class PyPlugin : public QObject{

	Q_OBJECT

	public:
		PyPlugin(PythonQtObjectPtr plugin_, QMenu& pluginMenu_, QObject* parent_ = 0);
		~PyPlugin();
		
		QIcon getIcon() const;
		QString getName() const;
		QString getShortDescription() const;
		QString getDescription() const;
		QString getAuthor() const;
		QString getLicense() const;
		QString getVersion() const;
	
		bool isEnabled() const;
	
	public slots:
		void setEnabled(bool enable_);
	
	protected slots:
		void execute();
	
	signals:
		void executed();
		
	private:
		struct Node{
			QString name;
			QString parent;
			QString icon;
			QString function;
			QStringList children;
		};
	
		void createMenu(QMenu* parent_, Node& node_, QMap<QString, Node>& tree_);
		bool getActionInfo(QVariantList& action_, QString& name_, QString& icon_,
							QString& shotcut_, QString& function_, QString& parent_);
	
		void setPluginMenuVisible(bool visible_);
		
	private:
		PythonQtObjectPtr _plugin;
	
		QString _icon;
		QString _name;
		QString _shortDescription;
		QString _description;
		QString _author;
		QString _license;
		QString _version;

		QObject* _menu;
};

#endif // __PLUGIN_ENGINE_H__
