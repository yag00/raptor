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
#include "document_editor.h"
#include "VerticalFileSwitcher.h"

VerticalFileSwitcher::VerticalFileSwitcher(QList<DocumentEditor*>& list_, QWidget* parent_) : QDialog(parent_){
	setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
	
	_maxwidth = 0;
	foreach(DocumentEditor* doc, list_)
		addItem(doc);
	
	switcherList->sortItems();
	switcherList->setCurrentRow(0);
	switcherList->setResizeMode(QListView::Adjust);
	int w = qMax(width(), _maxwidth + 30);
	resize(w, height());
	//center switcher in the middle of the parent_ (mainwindow)
	if(parent_)
		move(parent_->rect().center() - rect().center());
	switcherLineEdit->setFocus();
}

VerticalFileSwitcher::~VerticalFileSwitcher(){

}

void VerticalFileSwitcher::addItem(DocumentEditor* doc_) {
	QString icoFile;
	if(doc_->isModified() || (doc_->stillExist() == false))
		icoFile = ":/images/unsaved.png";
	else if(doc_->isReadOnly())
		icoFile = ":/images/readonly.png";
	else
		icoFile = ":/images/saved.png";
	
	new QListWidgetItem(QIcon(icoFile), doc_->getFullPath(), switcherList);
	
	QAbstractItemModel* model = switcherList->model();
	QStyleOptionViewItem option;
	switcherList->setIconSize(QSize(16,16));
	option.decorationSize = switcherList->iconSize();
	QModelIndex index = model->index(switcherList->count()-1, 0);
	QSize size = switcherList->itemDelegate()->sizeHint(option, index);
	if(_maxwidth < size.width())
		_maxwidth = size.width();
}

void VerticalFileSwitcher::on_switcherLineEdit_textChanged(const QString& text_) {
	for(int i = 0; i < switcherList->count(); i++) {
		QListWidgetItem* item = switcherList->item(i);
		item->setHidden(!item->text().contains(text_, Qt::CaseInsensitive));
	}
	if(switcherList->currentItem()->isHidden())
		selectNextVisibleItem();
}

void VerticalFileSwitcher::on_switcherLineEdit_returnPressed() {
	if(!switcherList->currentItem()->isHidden()){
		emit selected(switcherList->currentItem()->text());
		close();
	}
}

void VerticalFileSwitcher::on_switcherList_itemActivated(QListWidgetItem* item_){
	emit selected(item_->text());
	close();	
}

void VerticalFileSwitcher::selectNextVisibleItem(){
	int currentRow = switcherList->currentRow();
	int row = currentRow;
	if(row >= switcherList->count() - 1)
		row = -1;
	while(switcherList->item(++row)->isHidden()){
		if(row == currentRow){
			row = 0;
			break;
		}
		if(row >= switcherList->count() - 1)
			row = -1;
	}
	switcherList->setCurrentRow(row);
}

void VerticalFileSwitcher::selectPrevVisibleItem(){
	int currentRow = switcherList->currentRow();
	int row = currentRow;
	if(row <= 0)
		row = switcherList->count();
	while(switcherList->item(--row)->isHidden()){
		if(row == currentRow){
			row = 0;
			break;
		}
		if(row <= 0)
			row = switcherList->count();
	}
	switcherList->setCurrentRow(row);
}

void VerticalFileSwitcher::keyPressEvent(QKeyEvent* event_){
	switch(event_->key()){
		case Qt::Key_Up:
			selectPrevVisibleItem();	
			event_->accept();
			break;
		case Qt::Key_Down:
			selectNextVisibleItem();
			event_->accept();
			break;
		default:
			QWidget::keyPressEvent(event_);
			break;
	}
}

