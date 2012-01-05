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

#include <QObject>
#include <QEvent>
#include <QMouseEvent>
#include <QHeaderView>
#include <QTreeView>

#include "SymbolManager.h"
#include "SymbolTreeComboBox.h"

SymbolTreeComboBox::SymbolTreeComboBox(QAbstractItemModel* model_, QWidget* parent_) : QComboBox(parent_) {
	QTreeView* v = new ::QTreeView(this);
	setView(v);

	setMinimumContentsLength(22);

	// Make the combo box prefer to expand
	QSizePolicy policy = sizePolicy();
	policy.setHorizontalPolicy(QSizePolicy::Expanding);
	setSizePolicy(policy);

	setMaxVisibleItems(20);

	v->header()->hide();
	v->setItemsExpandable(false);
	v->setRootIsDecorated(false);
	v->setEditTriggers(QAbstractItemView::NoEditTriggers);

	setModel(model_);

	connect(v, SIGNAL(pressed(const QModelIndex&)), this, SLOT(pressed(const QModelIndex&)));
}

SymbolTreeComboBox::~SymbolTreeComboBox(){

}

void SymbolTreeComboBox::showPopup() {
	((QTreeView*)view())->expandAll();
	QComboBox::showPopup();
}

void SymbolTreeComboBox::hidePopup(){
	QComboBox::hidePopup();
}

void SymbolTreeComboBox::pressed(const QModelIndex& index_){
	int line = index_.data(SymbolManager::TAG_LINE).toInt();
	if(line){
		//emit symbolActivated(index_.data(SymbolManager::TAG_NAME).toString(), line);
		emit symbolActivated(line);
	}
	hidePopup();
}
