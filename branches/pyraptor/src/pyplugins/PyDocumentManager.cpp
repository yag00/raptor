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

#include <PythonQt.h>

#include "../document_editor.h"
#include "../document_manager.h"
#include "PluginEngine.h"
#include "PyDocument.h"
#include "PyDocumentManager.h"

PyDocumentManager::PyDocumentManager(PluginEngine& pluginEngine_, DocumentManager& docMgr_, QObject* parent_) 
	: QObject(parent_), _pluginEngine(pluginEngine_), _docMgr(docMgr_) {

}

PyDocumentManager::~PyDocumentManager(){

}

QStringList PyDocumentManager::getDocumentsNameList(){
	return _docMgr.getDocumentsNameList();
}

QStringList PyDocumentManager::getDocumentsNameList(int view_){
	return _docMgr.getDocumentsNameList(view_);
}

int PyDocumentManager::getViewNumber(){
	return _docMgr.getViewNumber();
}

void PyDocumentManager::deleteDocument(PyDocument* pyDoc_){
	_pluginEngine.remove(pyDoc_);
	delete pyDoc_;
}

PyDocument* PyDocumentManager::getDocument(const QString& name_){
	DocumentEditor* doc = _docMgr.getDocument(name_);
	if(doc){
		PyDocument* pydoc = new PyDocument(doc, this);
		_pluginEngine.garbageCollect(pydoc);
		return pydoc;
	}else{
		return 0;
	}
}

PyDocument* PyDocumentManager::currentDocument(){
	PyDocument* pydoc = new PyDocument(_docMgr.getActiveDocument(), this);
	_pluginEngine.garbageCollect(pydoc);
	return pydoc;
}
