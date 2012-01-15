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

#ifndef __SWITCHER_H__
#define __SWITCHER_H__

#include <QDialog>
#include "ui_switcher.h"

class QStandardItem;
class QStandardItemModel;

class Switcher : public QDialog, public Ui::Switcher{
	Q_OBJECT

	public:
		Switcher(QWidget* parent_ = 0);	
		virtual ~Switcher();
	
	private slots:
		void on_switcherLineEdit_textChanged(const QString& text_);
		void on_switcherLineEdit_returnPressed();
		void on_switcherList_clicked(const QModelIndex& index_);
		void on_switcherList_activated(const QModelIndex& index_);

	protected:
		virtual void keyPressEvent(QKeyEvent* event_);
		virtual void selectedItem(QStandardItem* item_) = 0;
	
		void centerSwitcher(QWidget* parent_);
		void setModel(QStandardItemModel* model_);

		QStandardItemModel* _model;

	private:
		void selectNextVisibleItem();
		void selectPrevVisibleItem();
		void walkTree(QStandardItem* item_, const QString& text_);
		
		
};

#endif // __VERTICAL_FILE_SWITCHER_H__
