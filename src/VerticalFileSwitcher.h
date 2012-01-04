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

#ifndef __VERTICAL_FILE_SWITCHER_H__
#define __VERTICAL_FILE_SWITCHER_H__

#include <QDialog>
#include "ui_switcher.h"

class DocumentEditor;

class VerticalFileSwitcher : public QDialog, private Ui::VerticalFileSwitcher{
	Q_OBJECT

	public:
		VerticalFileSwitcher(QList<DocumentEditor*>& list_, QWidget* parent_ = 0);
		~VerticalFileSwitcher();

	private slots:
		void on_switcherLineEdit_textChanged(const QString& text_);
		void on_switcherLineEdit_returnPressed();
		void on_switcherList_itemActivated(QListWidgetItem* item_);
			
	signals:
		void selected(QString);
	
	protected:
		void selectNextVisibleItem();
		void selectPrevVisibleItem();
	
		virtual void keyPressEvent(QKeyEvent* event_);

	private:
		void addItem(DocumentEditor* doc_);
		
		int _maxwidth;

};

#endif // __VERTICAL_FILE_SWITCHER_H__
