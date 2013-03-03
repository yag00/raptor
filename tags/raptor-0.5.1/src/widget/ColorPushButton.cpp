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

#include <QColorDialog>
#include "ColorPushButton.h"

ColorPushButton::ColorPushButton(QWidget* parent) : QPushButton(parent) {
	init(QColor(Qt::black));
}

ColorPushButton::ColorPushButton(const QColor& color, QWidget* parent) : QPushButton(parent) {
	init(color);
}

ColorPushButton::~ColorPushButton(){
}

void ColorPushButton::init(const QColor& color) {
	connect(this, SIGNAL(clicked()), this, SLOT(onClicked()));
	_isColorNameHidden = true;
	setColor(color);
}

void ColorPushButton::updateColorName() {
	setToolTip(_color.name().replace("#", QString::number(_color.alpha(), 16).prepend("#")));
	if (_isColorNameHidden)
		setText(QString::null);
	else
		setText(toolTip());
}

bool ColorPushButton::isColorNameHidden() const {
	return _isColorNameHidden;
}

void ColorPushButton::setColorNameHidden(bool hidden) {
	_isColorNameHidden = hidden;
	updateColorName();
}

const QColor& ColorPushButton::color() const {
	return _color;
}

void ColorPushButton::setColor(const QColor& color) {
	_color = color;
	QPixmap pixmap(iconSize());
	pixmap.fill(_color);
	setIcon(QIcon(pixmap));
	updateColorName();
}

void ColorPushButton::onClicked() {
	QColor color = QColorDialog::getColor(_color, this, "", QColorDialog::ShowAlphaChannel);
	if (color.isValid())
		setColor( color );
}

void ColorPushButton::setState(int state_){
	if(state_ == 0)
		setEnabled(false);
	else
		setEnabled(true);
}


#if WAF
#include "ColorPushButton.moc"
#endif

