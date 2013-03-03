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

#include <QHeaderView>
#include <QModelIndex>
#include <QStandardItemModel>
#include "SymbolManager.h"
#include "SymbolTreeView.h"

SymbolTreeView::SymbolTreeView(QStandardItemModel* model_, QWidget* parent_) : QTreeView(parent_){
	header()->hide();
	setRootIsDecorated(false);
    setItemsExpandable(false);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setModel(model_);
	connect(model_, SIGNAL(rowsInserted(QModelIndex, int, int)), this, SLOT(expandAll()));
	connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));
	connect(this, SIGNAL(activated(const QModelIndex&)), this, SLOT(activated(const QModelIndex&)));
}

SymbolTreeView::~SymbolTreeView(){

}

void SymbolTreeView::clicked(const QModelIndex& index_){
	activated(index_);
}

void SymbolTreeView::activated(const QModelIndex& index_){
	int line = index_.data(SymbolManager::TAG_LINE).toInt();
	if(line){
		emit symbolActivated(line);
	}
}

#if WAF
#include "SymbolTreeView.moc"
#endif

