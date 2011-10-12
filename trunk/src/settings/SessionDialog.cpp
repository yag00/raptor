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

#include <QLineEdit>
#include <QInputDialog>
#include <QDebug>
#include "SessionSettings.h"
#include "SessionDialog.h"

SessionDialog::SessionDialog(QWidget* parent_) : QDialog(parent_) {
	//init ui
	setupUi(this);
	
	connect(renameButton, SIGNAL(clicked()), this, SLOT(renameSession()));
	connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteSession()));
	
	_sessions = new SessionSettings(this);
	
	load();
}

SessionDialog::~SessionDialog() {

}

void SessionDialog::load(){
	//clear tree
	sessionsTree->clear();
	//load sessions
	QStringList sessions = _sessions->childGroups();
	foreach(QString s, sessions){
		QTreeWidgetItem* item = new QTreeWidgetItem(sessionsTree);
		item->setText(0, s);
		item->setText(1, QString("%1").arg(_sessions->getDocumentNumber(s)));
	}
}

void SessionDialog::renameSession(){
	QTreeWidgetItem* item =	sessionsTree->currentItem();
	bool ok = false;
	QString	name = QInputDialog::getText(this, tr("Session Name"), tr("Session name"), QLineEdit::Normal, item->text(0), &ok);
	if(ok){
		QString old = item->text(0);
		item->setText(0, name);
		//rename session
		QList<QStringList> views;
		SessionSettings s;
		s.load(old, views);
		s.save(name, views);
		s.remove(old);
		//reload session file
		load();
	}
}

void SessionDialog::deleteSession(){
	QTreeWidgetItem* item =	sessionsTree->currentItem();
	QString session = item->text(0);
	delete item;
	//delete session
	SessionSettings s;
	s.remove(session);
}

void SessionDialog::disableEditMode(){
	renameButton->setVisible(false);
	deleteButton->setVisible(false);
}

QString SessionDialog::getSelectedSession(){
	return sessionsTree->currentItem()->text(0);
}
