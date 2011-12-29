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

#ifndef __EXPLORER_H__
#define __EXPLORER_H__

#include <QWidget>
#include "ui_explorer.h"

class QFileSystemModel;

class Explorer : public QWidget, private Ui::FileExplorer {
	
	Q_OBJECT
	
	public:
		Explorer(QWidget *parent_ = 0);
		~Explorer();

		void navigate(const QString& name_);
		void synchronize(const QString& name_);
		void forceSynchronize(const QString& name_);

	signals:
		void fileClicked(const QString& name_);
		void synchFileRequest();

	protected slots:
		void validate();
		
		void on_lvDirs_activated(const QModelIndex& index_);
		void on_tbUp_clicked();
		void on_tbRoot_clicked();
		void on_tbOpenDir_clicked();
		void on_cbFilter_textChanged(const QString& text_);
		void on_tbSynch_clicked();
		
		void on_actionOpen_triggered();
		void on_actionCopyPath_triggered();
		void on_actionCopyName_triggered();

	protected:
		bool eventFilter(QObject* obj_, QEvent* event_);

	private:
		QFileSystemModel* _model;
};

#endif // __EXPLORER_H__
