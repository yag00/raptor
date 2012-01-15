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

#include <QKeyEvent>
#include <QStandardItem>
#include <QStandardItemModel>
#include "Switcher.h"

Switcher::Switcher(QWidget* parent_) : QDialog(parent_), _model(0){
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
	
	switcherList->setEditTriggers(QAbstractItemView::NoEditTriggers);	
	switcherLineEdit->setFocus();
}

Switcher::~Switcher(){

}

void Switcher::setModel(QStandardItemModel* model_){
	_model = model_;
	switcherList->setModel(_model);
	switcherList->expandAll();
	connect(_model, SIGNAL(rowsInserted(QModelIndex, int, int)), switcherList, SLOT(expandAll()));
}

void Switcher::centerSwitcher(QWidget* parent_){
	//center switcher in the middle of the parent_
	if(parent_)
		move(parent_->rect().center() - rect().center());
}

void Switcher::on_switcherList_clicked(const QModelIndex& index_){
	if(!switcherLineEdit->hasFocus())
		switcherLineEdit->setFocus(Qt::OtherFocusReason);
}

void Switcher::on_switcherList_activated(const QModelIndex& index_){
	QStandardItem* item = _model->itemFromIndex(index_);
	selectedItem(item);
	close();
}

void Switcher::on_switcherLineEdit_textChanged(const QString& text_) {	
	walkTree(_model->invisibleRootItem(), text_);
}

void Switcher::walkTree(QStandardItem* item_, const QString& text_){
	for(int i = 0; i < item_->rowCount(); i++) {
		QStandardItem* item = item_->child(i);
		if(item->hasChildren())
			walkTree(item, text_);
		else{
			bool hide = !item->text().contains(text_, Qt::CaseInsensitive);
			QModelIndex index = _model->indexFromItem(item);
			switcherList->setRowHidden(index.row(), index.parent(), hide);
		}
	}
}

void Switcher::on_switcherLineEdit_returnPressed() {
	QModelIndex index = switcherList->currentIndex();
	if(!switcherList->isRowHidden(index.row(), index.parent())){
		on_switcherList_activated(index);
	}
}

void Switcher::selectNextVisibleItem(){	
	QModelIndex index = switcherList->currentIndex();
	while(1){
		//get the next index
		index = switcherList->indexBelow(index);
		
		if(index == switcherList->currentIndex()){
			//full loop is done
			break;
		}
		
		if(!index.isValid()){
			//end of the tree
			//select the first item
			index = _model->index(0, 0);
		}
		
		//check if the index is visible
		bool rowHidden = switcherList->isRowHidden(index.row(), index.parent());
		if(rowHidden == false){
			//found the next visible item
			break;
		}
	}
	switcherList->setCurrentIndex(index);
}

void Switcher::selectPrevVisibleItem(){	
	QModelIndex index = switcherList->currentIndex();
	while(1){
		//get the previous index
		index = switcherList->indexAbove(index);
		
		if(index == switcherList->currentIndex()){
			//full loop is done
			break;
		}
		
		if(!index.isValid()){
			//beginning of the tree
			//select the last item
			index = _model->index(_model->rowCount() - 1, 0);
		}
		
		//check if the index is visible
		bool rowHidden = switcherList->isRowHidden(index.row(), index.parent());
		if(rowHidden == false){
			//found the previous visible item
			break;
		}
	}
	switcherList->setCurrentIndex(index);
}

void Switcher::keyPressEvent(QKeyEvent* event_){
	switch(event_->key()){
		case Qt::Key_Up:
			selectPrevVisibleItem();	
			event_->accept();
			break;
		case Qt::Key_Down:
			selectNextVisibleItem();
			event_->accept();
			break;
		case Qt::Key_Escape:
			event_->accept();
			close();
			break;		
		default:
			QWidget::keyPressEvent(event_);
			break;
	}
}

