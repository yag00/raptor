#include <QMenu>
#include <QMouseEvent>
#include "MenuLabel.h"

MenuLabel::MenuLabel(QWidget *parent_) : QPushButton(parent_) {
	setFlat(true);
	_menu = new QMenu(this);
}

MenuLabel::~MenuLabel(){
	
}

QMenu* MenuLabel::getMenu() {
	return _menu;
}

void MenuLabel::setPixmap(const QPixmap & pixmap_){
	QIcon icon(pixmap_);
	setIcon(icon);
}

void MenuLabel::mousePressEvent(QMouseEvent* event_) {
	if(_menu)
		_menu->popup(event_->globalPos());
}
