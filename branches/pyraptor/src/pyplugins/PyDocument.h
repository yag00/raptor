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

#ifndef __PY_DOCUMENT_H__
#define __PY_DOCUMENT_H__

#include <QObject>

class DocumentEditor;
class DocumentManager;

class PyDocument : public QObject {
	
	Q_OBJECT
	
	public:
		PyDocument(DocumentEditor* document_, QObject* parent_ = 0);
		~PyDocument();

	public slots:
		QString text();

	private:
		DocumentEditor* _document;
};

#endif // __PY_DOCUMENT_H__
