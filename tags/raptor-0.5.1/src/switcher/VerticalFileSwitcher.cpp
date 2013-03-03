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

#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>
#include "document_editor.h"
#include "VerticalFileSwitcher.h"

VerticalFileSwitcher::VerticalFileSwitcher(QList<DocumentEditor*>& list_, QWidget* parent_) : Switcher(parent_){
	//set the treeview model
	setModel(new QStandardItemModel(this));

	//add item
	_maxwidth = 0;
	foreach(DocumentEditor* doc, list_)
		addItem(doc);
	
	int w = qMax(width(), _maxwidth + 20);
	resize(w, height());
	//center switcher in the middle of the parent_ (mainwindow)
	centerSwitcher(parent_);
	
	switcherList->sortByColumn(0, Qt::AscendingOrder);
	QModelIndex first = _model->index(0, 0);
	switcherList->setCurrentIndex(first);
	switcherLineEdit->setFocus();
}

VerticalFileSwitcher::~VerticalFileSwitcher(){

}

void VerticalFileSwitcher::selectedItem(QStandardItem* item_){
	emit selected(item_->text());
}

void VerticalFileSwitcher::addItem(DocumentEditor* doc_) {
	QString icoFile;
	if(doc_->isModified() || (doc_->stillExist() == false))
		icoFile = ":/images/unsaved.png";
	else if(doc_->isReadOnly())
		icoFile = ":/images/readonly.png";
	else
		icoFile = ":/images/saved.png";
	
	QStandardItem* item = new QStandardItem(QIcon(icoFile), doc_->getFullPath());
	_model->appendRow(item);
	QStyleOptionViewItem option;
	option.decorationSize = switcherList->iconSize();
	QModelIndex index = _model->index(_model->rowCount()-1, 0);
	QSize size = switcherList->itemDelegate()->sizeHint(option, index);
	if(_maxwidth < size.width())
		_maxwidth = size.width();
}

#if WAF
#include "VerticalFileSwitcher.moc"
#endif

