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

#ifndef __PY_DOCUMENT_MANAGER_H__
#define __PY_DOCUMENT_MANAGER_H__

#include <QObject>
#include <QStringList>

class DocumentManager;
class PluginEngine;
class PyDocument;

class PyDocumentManager : public QObject{

	Q_OBJECT

	public:
		PyDocumentManager(PluginEngine& pluginEngine_, DocumentManager& docMgr_, QObject* parent_ = 0);
		~PyDocumentManager();
	
	public slots:
		/** @return list of document (fullpath)*/
		QStringList getDocumentsNameList();
		/** @return list of document (fullpath) by view */
		QStringList getDocumentsNameList(int view_);
		/** @return number of active view */
		int getViewNumber();
	
		/** get a document
		 * @param name_ fullpath of the document
		 * @return the document or null if doesn't exist */
		PyDocument* getDocument(const QString& name_);

		PyDocument* currentDocument();
		
		/** PyDocument explicit destructor */
		void deleteDocument(PyDocument* pyDoc_);
	
	private:
		PluginEngine& _pluginEngine;
		DocumentManager& _docMgr;
		
};

#endif // __PY_DOCUMENT_MANAGER_H__
