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

#include <QStandardItemModel>
#include "ctags/SymbolManager.h"
#include "SymbolSwitcher.h"

SymbolSwitcher::SymbolSwitcher(QStandardItemModel* model_, QWidget* parent_) : Switcher(parent_){	
    //set the treeview model
    setModel(model_);
	//compute width size of the window
	int maxwidth = 0;
	QStyleOptionViewItem option;
	option.decorationSize = QSize(16,16);
	QModelIndex index = model_->index(0, 0);
	while(index.isValid()){
		QSize size = switcherList->itemDelegate()->sizeHint(option, index);
		if(maxwidth < size.width())
			maxwidth = size.width();
		index = switcherList->indexBelow(index);
	}
	
	int w = qMax(width(), maxwidth + 50);
	resize(w, height());

	//center switcher in the middle of the parent_
	centerSwitcher(parent_);
	
	switcherList->sortByColumn(0, Qt::AscendingOrder);
	QModelIndex first = _model->index(0, 0);
	switcherList->setCurrentIndex(first);
	switcherLineEdit->setFocus();
}

SymbolSwitcher::~SymbolSwitcher(){

}

void SymbolSwitcher::selectedItem(QStandardItem* item_){
	int line = item_->data(SymbolManager::TAG_LINE).toInt();
	if(line){
		emit symbolActivated(line);
	}
}

#if WAF
#include "SymbolSwitcher.moc"
#endif

