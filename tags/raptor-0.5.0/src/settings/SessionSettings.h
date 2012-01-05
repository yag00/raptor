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

#ifndef __SESSION_SETTINGS_H__
#define __SESSION_SETTINGS_H__

#include <QSettings>

class QAction;

/** handle all session settings of the application */
class SessionSettings : public QSettings{

	Q_OBJECT

	public:
		SessionSettings(QObject* parent_ = 0);
		~SessionSettings();

		void save(const QString& session_, const QList<QStringList>& view_);
		void load(const QString& session_, QList<QStringList>& view_);

		QString loadStartSessionName(const QString& id_);
		void saveStartSessionName(const QString& id_, const QString& session_);

		/** @return the number of documents in the session */
		int getDocumentNumber(const QString& session_);

};

#endif // __SESSION_SETTINGS_H__
