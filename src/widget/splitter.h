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

#ifndef __CUSTOM_SPLITTER_H__
#define __CUSTOM_SPLITTER_H__

#include <QSplitter>

class Splitter : public QSplitter
{
    Q_OBJECT
	public:	
		Splitter(QWidget * parent = 0);
		Splitter(Qt::Orientation orientation, QWidget * parent = 0);
	
	private slots:
		void rotate();
		void swap();
		
	protected:
		void contextMenuEvent(QContextMenuEvent *event);
	

};

#endif /* __CUSTOM_SPLITTER_H__ */
