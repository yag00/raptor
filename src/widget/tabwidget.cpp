#include <QtGui>
#include "tabwidget.h"

TabBar::TabBar(QWidget* parent) : QTabBar(parent) {
	setAcceptDrops(true);
}
