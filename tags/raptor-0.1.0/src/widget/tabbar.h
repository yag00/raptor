#ifndef __TABBAR_H__
#define __TABBAR_H__

#include <QTabBar>

class TabBar : public QTabBar {
	Q_OBJECT
	public:
		TabBar(QWidget* parent_);
		~TabBar();
		
	protected:
		virtual void mousePressEvent(QMouseEvent* event_);
		virtual void mouseDoubleClickEvent(QMouseEvent* event_);
		virtual void mouseMoveEvent(QMouseEvent* event_);
		virtual void dragEnterEvent(QDragEnterEvent* event_);
		virtual void dragMoveEvent(QDragMoveEvent* event_);
		virtual void dragLeaveEvent(QDragLeaveEvent *event_);
		virtual void dropEvent(QDropEvent* event_);

	private:
		QPoint _dragStartPos;
		int _dragCurrentIndex;
		
	signals:
		/** emit when a drop event occurs in another TabBar */
		void tabMoveRequested(QWidget* src_, int srcIndex_, QWidget* dest_, int destIndex_);
		/** emit when a tab is clicked */
		void tabClicked(int index_);
		/** emit when the tabBar is double clicked */
		void tabBarDoubleClicked();
};

#endif // __TABBAR_H__
