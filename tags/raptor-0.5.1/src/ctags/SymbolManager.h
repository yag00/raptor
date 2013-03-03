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

#ifndef __SYMBOL_MANAGER_H__
#define __SYMBOL_MANAGER_H__

#include <QObject>
#include <QIcon>

class QStandardItem;
class QStandardItemModel;

class SymbolManager : public QObject {
	Q_OBJECT
	
	public:
		SymbolManager(QObject* parent_);
		virtual ~SymbolManager();
	
		QStandardItemModel* getSymbolModel();

		static const int TAG_KIND = Qt::UserRole + 1;
		static const int TAG_NAME = Qt::UserRole + 2;
		static const int TAG_LINE = Qt::UserRole + 3;
		static const int SCOPE_KIND = Qt::UserRole + 4;
		static const int SCOPE_NAME = Qt::UserRole + 5;
		
	public slots:
		void tagFile(const QString& file_);
	
	signals:
		void symbolActivated(int line_);
	
	private:
		QIcon getSymbolIcon(const QString& kind_, const QString& access_);
		
		QStandardItem* getItem(const QString& name_, const QString& kind_);
		QStandardItem* getItemChild(QStandardItem* child_, const QString& fullname_, const QString& name_, const QString& kind_);
		
		QStandardItemModel* _model;
};

#endif /* __SYMBOL_MANAGER_H__ */
