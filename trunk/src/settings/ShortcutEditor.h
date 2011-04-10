#ifndef __SHORTCUT_EDITOR_H__
#define __SHORTCUT_EDITOR_H__

#include <QDialog>
#include "ui_shortcut.h"

class ShortcutEditor : public QDialog, private Ui::UIShortcut{
	Q_OBJECT

	public:
		ShortcutEditor(QList<QMenu*>& menus_, QWidget* parent_ = 0);
		~ShortcutEditor();
		
		void addItems(QMenu *menu_);

	private:
		void addItem(QAction *action_, QTreeWidgetItem* parent_);
		void addItems(QList<QAction*>& actions_, QTreeWidgetItem* parent_);
	
	private slots:
		void on_actionTreeWidget_itemSelectionChanged();
		void on_actionFilterLineEdit_textChanged(const QString& text_);
	
		void set();
		void restoreDefault();
	
};

#endif // __SHORTCUT_EDITOR_H__
