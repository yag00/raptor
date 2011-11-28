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

#include <stdint.h>
#include <QAction>

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscicommand.h>
#include <Qsci/qscicommandset.h>

#include "../document_editor.h"
#include "ShortcutSettings.h"

ShortcutSettings::ShortcutSettings(QObject* parent_)
	: QSettings (QSettings::IniFormat, QSettings::UserScope, "raptor", "shortcut", parent_){

}
ShortcutSettings::~ShortcutSettings(){

}

bool ShortcutSettings::hasDefaultShortcut(){
	return childGroups().contains("DefaultShortcut");
}

void ShortcutSettings::restoreDefaultShortcut(QAction* action_){
	remove("Shortcut/" + action_->objectName());
	updateActionWithDefaultShortcut(action_);
}

void ShortcutSettings::saveDefaultShortcut(QAction* action_){
	setValue("DefaultShortcut/" + action_->objectName(), action_->shortcut());
	updateQsciCommand(action_);
}

void ShortcutSettings::saveUserShortcut(QAction* action_){
	setValue("Shortcut/" + action_->objectName(), action_->shortcut());
	updateQsciCommand(action_);
}

QKeySequence ShortcutSettings::getDefaultShortcut(QAction* action_){
	return value("DefaultShortcut/" + action_->objectName()).value<QKeySequence>();
}

QKeySequence ShortcutSettings::getUserShortcut(QAction* action_){
	return value("Shortcut/" + action_->objectName()).value<QKeySequence>();
}

void ShortcutSettings::updateActionWithDefaultShortcut(QAction* action_){
	if(contains("DefaultShortcut/" + action_->objectName())){
		action_->setShortcut(getDefaultShortcut(action_));
		updateQsciCommand(action_);
	}
}

void ShortcutSettings::updateActionWithUserShortcut(QAction* action_){
	if(contains("Shortcut/" + action_->objectName())){
		action_->setShortcut(getUserShortcut(action_));
	}
}

void ShortcutSettings::updateQsciCommand(QAction* action_){
	QsciCommand::Command cmd = (QsciCommand::Command)action_->data().toInt();
	if(cmd){
		//todo qsci -> update
		QKeySequence keys = action_->shortcut();
		uint32_t key = 0;
		for(uint32_t i = 0; i < keys.count(); i++){
			key |= keys[i];
		}
		QsciScintilla qsci;
		QsciCommandSet* commandset = qsci.standardCommands();
		QsciCommand* command = commandset->find(cmd);
		if(command){
			command->setKey(key);
		}
		commandset->writeSettings(*this);
	}
}

void ShortcutSettings::update(DocumentEditor* document_){
	QsciCommandSet* commandset = document_->standardCommands();
	commandset->readSettings(*this);
}
