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

#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>

#include "../document_manager.h"
#include "SessionDialog.h"
#include "SessionSettings.h"
#include "SessionManager.h"

const QString SessionManager::startSessionId = "ac0452da134c2a204d7b5a7f5bb516147d27ee84"; // sha1(raptor)

SessionManager::SessionManager(DocumentManager& docMgr_, QObject* parent_) : QObject(parent_), _docMgr(docMgr_) {

}

SessionManager::~SessionManager() {

}

void SessionManager::error(){
	QMessageBox::warning(0, tr("Session"), tr("Can't change session. Some documents are not saved!"));
}

void SessionManager::newSession() {
	if(_docMgr.closeAllIfPossible()){
		_lastSession = _currentSession;
		_currentSession = "";
	}else{
		error();
	}
}

void SessionManager::manageSessions() {
	SessionDialog dlg;
	if(dlg.exec() == QDialog::Accepted){
		_lastSession = _currentSession;
		open(dlg.getSelectedSession());
	}
}

void SessionManager::openSession() {
	SessionDialog dlg;
	dlg.disableEditMode();
	if(dlg.exec() == QDialog::Accepted){
		_lastSession = _currentSession;
		open(dlg.getSelectedSession());
	}
}

void SessionManager::open(const QString& name_){
	if(_docMgr.closeAllIfPossible()){
		_currentSession = name_;
		SessionSettings s;
		QList<QStringList> views;
		s.load(_currentSession, views);
		for(int i = 0; i < views.size(); i++){
			_docMgr.open(views[i], i);
		}
	}else{
		error();
	}	
}

void SessionManager::openStartSession(){
	open(startSessionId);
}

void SessionManager::switchSession() {
	QString session = _currentSession;
	open(_lastSession);
	_lastSession = session;
}

void SessionManager::saveSession() {
	if(_currentSession.isEmpty()){
		saveSessionAs();
	}else{
		save();
	}
}

void SessionManager::saveStartSession(){
	QString tmp = _currentSession;
	_currentSession = startSessionId;
	save();
	_currentSession = tmp;
}

void SessionManager::saveSessionAs() {
	bool ok = false;
	QString	name = QInputDialog::getText(0, tr("Save Session"), tr("Session name"), QLineEdit::Normal, "", &ok);
	if(ok){
		_currentSession = name;
		save();
	}
}

void SessionManager::save(){
	SessionSettings s;
	QList<QStringList> views;
	for(int i = 0; i < _docMgr.getViewNumber(); i++){
		views << _docMgr.getDocumentsNameList(i);
	}
	s.save(_currentSession, views);
}

