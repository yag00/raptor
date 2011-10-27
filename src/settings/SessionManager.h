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

#ifndef __SESSION_MANAGER_H__
#define __SESSION_MANAGER_H__

#include <QObject>

class DocumentManager;

class SessionManager : public QObject {
	Q_OBJECT

	public:
		SessionManager(DocumentManager& docMgr_, QObject* parent_ = 0);
		~SessionManager();

		static const QString startSessionId;

	public slots:
		void newSession();
		void openSession();
		void switchSession();
		void saveSession();
		void saveSessionAs();
		void manageSessions();

		void openStartSession();
		void saveStartSession();

	signals:
		/** emit on a new a status message */
		void statusMessage(QString);

	private:
		bool open(const QString& name_);
		void save();
		void error();

	private:
		/** reference on document manager */
		DocumentManager& _docMgr;
		/** current session name */
		QString _currentSession;
		/** last session name */
		QString _lastSession;
};

#endif // __SESSION_MANAGER_H__
