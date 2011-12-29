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

#ifndef __COLOR_BUTTON_H__
#define __COLOR_BUTTON_H__

#include <QPushButton>

class ColorPushButton : public QPushButton {
	Q_OBJECT
	Q_PROPERTY( bool colorNameHidden READ isColorNameHidden WRITE setColorNameHidden )
	Q_PROPERTY( QColor color READ color WRITE setColor )
	
	public:
		ColorPushButton(QWidget* parent = 0);
		ColorPushButton(const QColor& color, QWidget* parent = 0);
		virtual ~ColorPushButton();
		
		bool isColorNameHidden() const;
		const QColor& color() const;
	
	public slots:
		void setState(int state_);
		void setColorNameHidden(bool hidden);
		void setColor(const QColor& color);
	
	protected:
		void init(const QColor& color);
		void updateColorName();
	
	protected slots:
		void onClicked();
		
	private:
		bool _isColorNameHidden;
		QColor _color;
};

#endif // __COLOR_BUTTON_H__
