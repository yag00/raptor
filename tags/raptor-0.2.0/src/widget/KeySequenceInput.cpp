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

#include <QKeySequence>
#include <QKeyEvent>

#include "KeySequenceInput.h"

KeySequenceInput::KeySequenceInput(QWidget* parent_) : QLineEdit(parent_) {
	_sequenceEnd = true;
}

KeySequenceInput::~KeySequenceInput() {

}

void KeySequenceInput::keyPressEvent(QKeyEvent* event_) {	
	// return if auto repeat
	if (event_->isAutoRepeat())
		return;
	
	// if user press something, sequence is not finished
	_sequenceEnd = false;
	
	// show current sequence
	setText(checkKeyEvent(event_));
}

void KeySequenceInput::keyReleaseEvent(QKeyEvent* event_) {
	// return if auto repeat
	if (event_->isAutoRepeat() )
		return;
	
	// check if sequence is finished or not
	if (_sequenceEnd)
		return;
	
	// show current sequence
	setText(checkKeyEvent(event_));
}

QString KeySequenceInput::checkKeyEvent(QKeyEvent* event_) {
	// is key pressed or key released ?
	const bool keyPressed = (event_->type() == QEvent::KeyPress);
	
	int keys = 0;
	
	// check modifiers pressed
	if (event_->modifiers() & Qt::ControlModifier)
		keys |= Qt::ControlModifier;
	if (event_->modifiers() & Qt::AltModifier)
		keys |= Qt::AltModifier;
	if (event_->modifiers() & Qt::ShiftModifier)
		keys |= Qt::ShiftModifier;
	if (event_->modifiers() & Qt::MetaModifier)
		keys |= Qt::MetaModifier;
	
	if (keyPressed)	{
		// get press key
		switch(event_->key()) {
			// this keys can't be used
			case Qt::Key_Shift:
			case Qt::Key_Control:
			case Qt::Key_Meta:
			case Qt::Key_Alt:
			case Qt::Key_AltGr:
			case Qt::Key_Super_L:
			case Qt::Key_Super_R:
			case Qt::Key_Menu:
			case Qt::Key_Hyper_L:
			case Qt::Key_Hyper_R:
			case Qt::Key_Help:
			case Qt::Key_Direction_L:
			case Qt::Key_Direction_R:
				break;
			default:
				// add pressed key
				keys |= event_->key();
				
				// set sequence finished
				_sequenceEnd = true;
				break;
		}
	}
	
	// return human readable key sequence
	return QKeySequence(keys).toString();
}
