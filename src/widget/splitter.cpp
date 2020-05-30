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

#include <QtWidgets>
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


#if WAF
#include "splitter.moc"
#endif

