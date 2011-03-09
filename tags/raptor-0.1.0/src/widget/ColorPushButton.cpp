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