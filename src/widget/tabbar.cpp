#include <QtGui>
#include "tabbar.h"

TabBar::TabBar(QWidget* parent_) : QTabBar(parent_) {
	setAcceptDrops(true);
}

TabBar::~TabBar(){

}

void TabBar::mousePressEvent(QMouseEvent* event_) {
	QTabBar::mousePressEvent(event_);
	_dragCurrentIndex = -1;
	for(int i = 0; i < count(); i++){
		if(tabRect(i).contains(event_->pos())){
			_dragCurrentIndex = i;
			emit tabClicked(i);
			break;
		}
	}
		
	if (event_->button() == Qt::LeftButton){
		_dragStartPos = event_->pos();
	}
}

void TabBar::mouseDoubleClickEvent(QMouseEvent* event_){
	QTabBar::mouseDoubleClickEvent(event_);
	for(int i = 0; i < count(); i++){
		if(tabRect(i).contains(event_->pos())){
			return;
		}
	}
	//click is not on a tab -> emit signal
	emit tabBarDoubleClicked();
}

void TabBar::mouseMoveEvent(QMouseEvent* event_) {
	QTabBar::mouseMoveEvent(event_);
	/*	
	// If the left button isn't pressed anymore then return
	if (!(event_->buttons() & Qt::LeftButton))
		return;

	// If we are sill in the tabbar then return else start Drag
	if(((QTabWidget*)parent())->rect().contains(event_->pos()))
		return;
		
	// If the distance is too small then return
	if ((event_->pos() - _dragStartPos).manhattanLength() < QApplication::startDragDistance())
		return;

	// initiate Drag
	QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << _dragCurrentIndex;
	QMimeData* mimeData = new QMimeData;
	mimeData->setData("tab-drag&drop", itemData) ;
	
	QDrag* drag = new QDrag(this);
	// drag->setDragCursor(QPixmap(QString(":/images/moveto.png")), Qt::CopyAction );
	// drag->setDragCursor(QPixmap(QString(":/images/moveto.png")), Qt::MoveAction  );
	// drag->setDragCursor(QPixmap(QString(":/images/moveto.png")), Qt::LinkAction  );
	// drag->setDragCursor(QPixmap(QString(":/images/moveto.png")), Qt::TargetMoveAction  );
	drag->setMimeData(mimeData);
	drag->exec(Qt::CopyAction | Qt::MoveAction | Qt::TargetMoveAction);*/
}

void TabBar::dragEnterEvent(QDragEnterEvent* event_) {
	QTabBar::dragEnterEvent(event_);
	// Only accept if it's an tab-drag&drop request
	/*const QMimeData* m = event_->mimeData();
	if(m->hasFormat("tab-drag&drop")){
		event_->accept();
		//event_->acceptProposedAction();
	}else{
		event_->ignore();
	}*/
}

void TabBar::dragMoveEvent(QDragMoveEvent* event_){
	QTabBar::dragMoveEvent(event_);
	event_->accept();
}

void TabBar::dragLeaveEvent(QDragLeaveEvent *event_) {
	QTabBar::dragLeaveEvent(event_);
	event_->accept();
}

void TabBar::dropEvent(QDropEvent* event_) {
	QTabBar::dropEvent(event_);
	const QMimeData* m = event_->mimeData();
	if(m->hasFormat("tab-drag&drop")){       
		event_->setDropAction(Qt::MoveAction);
		if(this == event_->source()){
			//drag&drop in the same tabBar
			int fromIndex   = tabAt(_dragStartPos);
			int toIndex     = tabAt(event_->pos());
			if (fromIndex != toIndex){
				if(toIndex == -1)
					toIndex = count()-1;
				moveTab(fromIndex, toIndex);
			}
		}else{
			//drag&drop in different tabBar
			QByteArray itemData = m->data("tab-drag&drop");
			QDataStream dataStream(&itemData, QIODevice::ReadOnly);
			int fromIndex;
			dataStream >> fromIndex;
			int toIndex = tabAt(event_->pos());
			emit tabMoveRequested(event_->source(), fromIndex, this, toIndex);
		}
		event_->accept();
		//event_->acceptProposedAction();
	}else{
		event_->ignore();
	}
}
