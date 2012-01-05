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

#include <QStringList>
#include "SessionSettings.h"

SessionSettings::SessionSettings(QObject* parent_)
	: QSettings (QSettings::IniFormat, QSettings::UserScope, "raptor", "session", parent_){

}
SessionSettings::~SessionSettings(){

}

void SessionSettings::save(const QString& session_, const QList<QStringList>& view_){
	remove(session_);
	beginGroup(session_);
	setValue("viewnb", view_.size());
	int i = 1;
	foreach(const QStringList view, view_){
		beginWriteArray(QString("view%1").arg(i++));
		int j = 0;
		foreach(QString s, view){
			setArrayIndex(j++);
			setValue("URL", s);
		}
		endArray();
	}
	endGroup();
}

void SessionSettings::load(const QString& session_, QList<QStringList>& view_){
	beginGroup(session_);
	for(int i = 0; i < value("viewnb", 0).toInt(); i++){
		QStringList urls;
		int size = beginReadArray(QString("view%1").arg(i+1));
		if(size == 0){
			endArray();
		}else{
			for (int i = 0; i < size; ++i) {
				setArrayIndex(i);
				urls << value("URL").toString();
			}
			endArray();
		}
		view_.push_back(urls);
	}
	endGroup();
}

QString SessionSettings::loadStartSessionName(const QString& id_){
	beginGroup(id_);
	QString session = value("name", "").toString();
	endGroup();
	return session;
}

void SessionSettings::saveStartSessionName(const QString& id_, const QString& session_){
	beginGroup(id_);
	setValue("name", session_);
	endGroup();
}

int SessionSettings::getDocumentNumber(const QString& session_){
	int cpt = 0;
	beginGroup(session_);
	for(int i = 0; i < value("viewnb", 0).toInt(); i++){
		cpt += beginReadArray(QString("view%1").arg(i+1));
		endArray();
	}
	endGroup();
	return cpt;
}
