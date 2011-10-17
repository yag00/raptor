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

#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QListWidget>

#include "StringListEditor.h"

StringListEditor::StringListEditor(const QString& title, QWidget* parent) : QGroupBox(title, parent) {
	// create layout
	_hLayout = new QVBoxLayout( this );
	_hLayout->setMargin(5);
	_hLayout->setSpacing(3);
	
	// create toolbar
	QToolBar* tb = new QToolBar;
	tb->layout()->setMargin(0);
	tb->layout()->setSpacing(0);
	tb->setIconSize(QSize(16, 16));
	_hLayout->addWidget(tb);
	
	// create listwidget
	_list = new QListWidget;
	_list->setMinimumHeight(40);
	_list->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	_hLayout->addWidget(_list);
	
	// create actions
	QAction* addAction = new QAction( QIcon( ":/listeditor/images/listeditor/add.png" ), tr("Add Item"), tb);
	QAction* removeAction = new QAction( QIcon( ":/listeditor/images/listeditor/remove.png" ), tr("Remove Item"), tb);
	QAction* clearAction = new QAction( QIcon( ":/listeditor/images/listeditor/clear.png" ), tr("Clear Items"), tb);
	QAction* upAction = new QAction( QIcon( ":/listeditor/images/listeditor/up.png" ), tr("Move Item Up"), tb);
	QAction* downAction = new QAction( QIcon( ":/listeditor/images/listeditor/down.png" ), tr("Move Item Down"), tb);
	QAction* editAction = new QAction( QIcon( ":/listeditor/images/listeditor/edit.png" ), tr("Edit Item"), tb);
	
	// add actions to toolbar
	tb->addAction(addAction);
	tb->addAction(removeAction);
	tb->addAction(clearAction);
	tb->addAction(upAction);
	tb->addAction(downAction);
	tb->addAction(editAction);
	
	// connections
	connect(addAction, SIGNAL(triggered()), this, SLOT(onAddItem()));
	connect(removeAction, SIGNAL(triggered()), this, SLOT(onRemoveItem()));
	connect(clearAction, SIGNAL(triggered()), this, SLOT(onClearItem()));
	connect(upAction, SIGNAL(triggered()), this, SLOT(onMoveUpItem()));
	connect(downAction, SIGNAL(triggered()), this, SLOT(onMoveDownItem()));
	connect(editAction, SIGNAL(triggered()), this, SLOT(onEditItem()));
	connect(_list, SIGNAL(itemChanged(QListWidgetItem*)), this, SIGNAL(edited()));
	connect( this, SIGNAL(edited()), this, SLOT(onEdited()));
}

QVBoxLayout* StringListEditor::verticalLayout() const {
	return _hLayout;
}

/*!
	\details Set the editor values
	\param values The string list to set as values
*/
void StringListEditor::setValues(const QStringList& values) {
	_list->clear();
	foreach ( QString value, values ) {
		QListWidgetItem* it = new QListWidgetItem( value, _list );
		it->setFlags( it->flags() | Qt::ItemIsEditable );
		_list->setCurrentItem( it );
		_list->scrollToItem( it );
	}
	emit edited();
}

/*!
	\details Return the editor QStringList values
*/
QStringList StringListEditor::values() const {
	QStringList values;
	foreach ( QListWidgetItem* it, _list->findItems( "*", Qt::MatchWildcard | Qt::MatchRecursive ) )
		values << it->text();
	return values;
}

void StringListEditor::onEdited() {
	for ( int i = 0; i < _list->count(); i++ ) {
		QListWidgetItem* item = _list->item( i );
		item->setToolTip( item->text() );
	}
}

void StringListEditor::onAddItem() {
	QListWidgetItem* it = new QListWidgetItem( tr( "New item" ), _list );
	it->setFlags( it->flags() | Qt::ItemIsEditable );
	_list->setCurrentItem( it );
	_list->scrollToItem( it );
	_list->editItem( it );
	emit edited();
}

void StringListEditor::onRemoveItem() {
	if ( QListWidgetItem* it = _list->selectedItems().value( 0 ) )
	{
		delete it;
		emit edited();
	}
}

void StringListEditor::onClearItem() {
	if ( _list->count() )
	{
		_list->clear();
		emit edited();
	}
}

void StringListEditor::onMoveUpItem() {
	if ( QListWidgetItem* it = _list->selectedItems().value( 0 ) )
	{
		int i = _list->row( it );
		if ( i != 0 )
			_list->insertItem( i -1, _list->takeItem( i ) );
		_list->setCurrentItem( it );
		emit edited();
	}
}

void StringListEditor::onMoveDownItem() {
	if ( QListWidgetItem* it = _list->selectedItems().value( 0 ) )
	{
		int i = _list->row( it );
		if ( i != _list->count() -1 )
			_list->insertItem( i +1, _list->takeItem( i ) );
		_list->setCurrentItem( it );
		emit edited();
	}
}

void StringListEditor::onEditItem() {
	if ( QListWidgetItem* it = _list->selectedItems().value( 0 ) )
		_list->editItem( it );
}
