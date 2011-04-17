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

#ifndef __SHORTCUT_EDITOR_H__
#define __SHORTCUT_EDITOR_H__

#include <QDialog>
#include "ui_shortcut.h"

class ShortcutSettings;

class ShortcutEditor : public QDialog, private Ui::UIShortcut{
	Q_OBJECT

	public:
		ShortcutEditor(QList<QMenu*>& menus_, QWidget* parent_ = 0);
		~ShortcutEditor();
		
		void addItems(QMenu *menu_);

	private:
		void addItem(QAction *action_, QTreeWidgetItem* parent_);
		void addItems(QList<QAction*>& actions_, QTreeWidgetItem* parent_);
	
	private slots:
		void on_actionTreeWidget_itemSelectionChanged();
		void on_actionFilterLineEdit_textChanged(const QString& text_);
		void on_resetButton_clicked();
		void on_resetAllButton_clicked();
		void on_setShortcutButton_clicked();
	
	private:	
		ShortcutSettings* _settings;
	
};

#endif // __SHORTCUT_EDITOR_H__
