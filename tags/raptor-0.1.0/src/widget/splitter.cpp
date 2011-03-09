#include <QtGui>
#include "splitter.h"

Splitter::Splitter(QWidget * parent) : QSplitter(parent){

}

Splitter::Splitter(Qt::Orientation orientation, QWidget * parent) : QSplitter(orientation, parent){

}
		
void Splitter::contextMenuEvent(QContextMenuEvent *event){
	if(this->x() <= event->x() && this->y() <= event->y()){
		QMenu contextMenu;
		QAction *rotate = contextMenu.addAction(tr("Rotate"));
		rotate->setIcon(QIcon(":/images/rotate_clockwise.png"));
		connect(rotate, SIGNAL(triggered()), this, SLOT(rotate()));
		QAction *swap = contextMenu.addAction(tr("Swap"));
		swap->setIcon(QIcon(":/images/swap.png"));
		connect(swap, SIGNAL(triggered()), this, SLOT(swap()));	
		contextMenu.exec(QCursor::pos());
	}
}

void Splitter::rotate(){
	if(orientation() == Qt::Horizontal)
		setOrientation(Qt::Vertical);
	else
		setOrientation(Qt::Horizontal);
}

void Splitter::swap(){
	QWidget* w = widget(count() - 1);
	insertWidget(0, w);
}