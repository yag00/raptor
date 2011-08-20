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
		virtual void mouseReleaseEvent(QMouseEvent* event_);
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
		/** emit when a tab is middle clicked */
		void tabMiddleClicked(int index_);
		/** emit when the tabBar is double clicked */
		void tabBarDoubleClicked();
};

#endif // __TABBAR_H__
