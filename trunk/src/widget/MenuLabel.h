#ifndef __MENU_LABEL_H__
#define __MENU_LABEL_H__

#include <QPushButton>

class QMenu;
class QMouseEvent;

class MenuLabel : public QPushButton {
		Q_OBJECT
	public:
		MenuLabel(QWidget* parent_ = 0);
		~MenuLabel();

		QMenu* getMenu();
		void setPixmap(const QPixmap & pixmap_);
	
	protected:
		virtual void mousePressEvent(QMouseEvent* event_);

	private:
		QMenu *_menu;
};

#endif // __MENU_LABEL_H__
