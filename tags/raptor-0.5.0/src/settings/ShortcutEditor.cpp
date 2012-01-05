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

#include <QMessageBox>
#include <QTreeWidget>
#include <QHeaderView>
#include <QAction>
#include <QMenu>

#include "ShortcutSettings.h"
#include "ShortcutEditor.h"

ShortcutEditor::ShortcutEditor(QList<QMenu*>& menus_, QWidget* parent ) : QDialog( parent ) {
	_settings = new ShortcutSettings(this);
	//init ui
	setupUi(this);
	actionTreeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);

	foreach(QMenu* menu, menus_)
		addItems(menu);
}

ShortcutEditor::~ShortcutEditor() {

}


void ShortcutEditor::addItem(QAction *action_, QTreeWidgetItem* parent_) {
	//discard unwanted action
	if (action_->menu())
		return;
	if (action_->isSeparator())
		return;
	if (action_->objectName().isEmpty())
		return;

	QTreeWidgetItem* item = new QTreeWidgetItem(parent_);
	item->setIcon(0, action_->icon());
	item->setText(0, action_->text());
	item->setData(0, Qt::UserRole, QVariant::fromValue((void*)action_));
	item->setTextAlignment(1, Qt::AlignVCenter | Qt::AlignRight);
	item->setText(1, action_->shortcut().toString());
}

void ShortcutEditor::addItems(QList<QAction*> &actions_, QTreeWidgetItem* parent_) {
	foreach(QAction *action, actions_) {
		addItem(action, parent_);
	}
}

void ShortcutEditor::addItems(QMenu *menu_) {
	//discard unwanted menu
	if(menu_->title().isEmpty())
		return;
	if(menu_->objectName() == "menuLanguage")
		return;
	if(menu_->objectName() == "menuUseCharset")
		return;
	if(menu_->objectName() == "menuSaveWithCharset")
		return;
	if(menu_->objectName() == "menuSaveWithCharsetAs")
		return;
	if(menu_->objectName() == "menuForceSaveWithCharsetAs")
		return;

	QTreeWidgetItem* item = new QTreeWidgetItem(actionTreeWidget);
	item->setExpanded(true);
	item->setFirstColumnSpanned(true);
	item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
	item->setBackgroundColor(0, Qt::gray);
	item->setTextAlignment(0, Qt::AlignCenter);
	item->setIcon(0, menu_->icon());
	item->setText(0, menu_->title().remove("&"));

	QList<QAction*> actions = menu_->actions();
	addItems(actions, item);
}


void ShortcutEditor::on_actionTreeWidget_itemSelectionChanged() {
	// get selected item
	QTreeWidgetItem* item = actionTreeWidget->selectedItems().value(0);
	if(item == 0)
		return;

	// get action
	QAction* action = (QAction*)item->data(0, Qt::UserRole).value<void*>();
	// set shortcut
	shortcutLineEdit->setText(action->shortcut().toString());
	// give focus to lineedit
	shortcutLineEdit->setFocus();
}

void ShortcutEditor::on_actionFilterLineEdit_textChanged(const QString& text_) {
	for(int i = 0; i < actionTreeWidget->topLevelItemCount(); i++) {
		QTreeWidgetItem* item = actionTreeWidget->topLevelItem(i);
		bool hideParent = true;
		for(int j = 0; j < item->childCount(); j++){
			QTreeWidgetItem* child = item->child(j);
			child->setHidden(!child->text(0).contains(text_, Qt::CaseInsensitive));
			if(!child->isHidden())
				hideParent = false;
		}
		item->setHidden(hideParent);
	}
}

void ShortcutEditor::on_resetButton_clicked(){
	// get selected item
	QTreeWidgetItem* item = actionTreeWidget->selectedItems().value(0);
	if(item == 0)
		return;
	// get action
	QAction* action = (QAction*)item->data(0, Qt::UserRole).value<void*>();
	// set shortcut
	_settings->restoreDefaultShortcut(action);
	item->setText(1, action->shortcut().toString());
}
void ShortcutEditor::on_resetAllButton_clicked(){
	_settings->remove("/Shortcut");
	for(int i = 0; i < actionTreeWidget->topLevelItemCount(); i++) {
		QTreeWidgetItem* item = actionTreeWidget->topLevelItem(i);
		for(int j = 0; j < item->childCount(); j++){
			QTreeWidgetItem* child = item->child(j);
			// get action
			QAction* action = (QAction*)child->data(0, Qt::UserRole).value<void*>();
			_settings->restoreDefaultShortcut(action);
			child->setText(1, action->shortcut().toString());
		}
	}
}

void ShortcutEditor::on_setShortcutButton_clicked() {
	// get selected item
	QTreeWidgetItem* item = actionTreeWidget->selectedItems().value(0);
	if(item == 0)
		return;
	QString shortcut = shortcutLineEdit->text();
	if(shortcut == item->text(1))
		return;
	// get action
	QAction* action = (QAction*)item->data(0, Qt::UserRole).value<void*>();
	// set shortcut
	for(int i = 0; i < actionTreeWidget->topLevelItemCount(); i++) {
		QTreeWidgetItem* item = actionTreeWidget->topLevelItem(i);
		for(int j = 0; j < item->childCount(); j++){
			QTreeWidgetItem* child = item->child(j);
			if((child->text(1) == shortcut) && (!shortcut.isEmpty())){
				QMessageBox::warning(this, PACKAGE_NAME,
					tr("Conflicting shortcut %1!\nShortcut already used by action %2!").arg(shortcut).arg(child->text(0)));
				return;
			}
		}
	}

	action->setShortcut(QKeySequence(shortcut));
	item->setText(1, action->shortcut().toString());

	_settings->saveUserShortcut(action);
}
