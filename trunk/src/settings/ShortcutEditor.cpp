#include <QTreeWidget>
#include <QHeaderView>
#include <QAction>
#include <QMenu>

#include "ShortcutEditor.h"

ShortcutEditor::ShortcutEditor(QList<QMenu*>& menus_, QWidget* parent ) : QDialog( parent ) {
	//init ui
	setupUi(this);
	actionTreeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);

	foreach(QMenu* menu, menus_)
		addItems(menu);

	// connections
	connect(clearShortcutButton, SIGNAL(clicked()), shortcutLineEdit, SLOT(clear()));
	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

	//connect(setShortcutButton, SIGNAL(clicked()), shortcutLineEdit, SLOT(set()));
	//connect(restoreButton, SIGNAL(clicked()), shortcutLineEdit, SLOT(restoreDefault()));
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
	shortcutLineEdit->setText( action->shortcut().toString() );
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

void ShortcutEditor::set() {

}

void ShortcutEditor::restoreDefault() {

}
