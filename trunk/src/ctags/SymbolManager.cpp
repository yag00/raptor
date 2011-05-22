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

#include <ctags/ctags.h>

#include <QDebug>
#include <QWidgetAction>
#include <QHeaderView>
#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>

#include "SymbolComboBoxAction.h"
#include "SymbolManager.h"

SymbolManager::SymbolManager(QObject* parent_) : QObject(parent_) {
	initializeParsing();
	installLanguageMapDefaults();

	_model = new QStandardItemModel(this);
	
	//create the treeview widget
	_treeView = new QTreeView();
	_treeView->header()->hide();
	_treeView->setRootIsDecorated(false);
    _treeView->setItemsExpandable(false);
	_treeView->setModel(_model);
	_treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	
	connect(_treeView, SIGNAL(clicked(const QModelIndex&)), this, SLOT(clicked(const QModelIndex&)));
	connect(_treeView, SIGNAL(activated(const QModelIndex&)), this, SLOT(activated(const QModelIndex&)));
	
	//create the combobox action
	_action = new SymbolComboBoxAction(_model, this);
	connect(_action, SIGNAL(symbolActivated(int)), this, SIGNAL(symbolActivated(int)));
}

SymbolManager::~SymbolManager() {
	delete _treeView;
	freeParserResources();
}

QAction* SymbolManager::getSymbolBrowerAction(){
	return _action;
}

QTreeView* SymbolManager::getSymbolBrowerTreeView(){
	return _treeView;
}

void SymbolManager::tagFile(const QString& file_){
	_model->clear();
	
	TagEntryListItem* tagsEntryList = createTagEntryListItem(file_.toLocal8Bit().constData(), 0);

	QStandardItem* anonymousNamespace = 0;
		
	TagEntryListItem* tags = tagsEntryList;
	while (tags != 0) {
		tagEntryInfo tag = tags->tag;

		QString symbol = QString(tag.name);
		if(tag.extensionFields.signature)
			symbol += tag.extensionFields.signature;
		
		QString scopeKind(tag.extensionFields.scope[0]);
		QString scopeName(tag.extensionFields.scope[1]);
		
		if(scopeName.isEmpty()){
			QStandardItem* item = new QStandardItem(getSymbolIcon(tag.kindName, tag.extensionFields.access), symbol);
			item->setData(tag.kindName, TAG_KIND);
			item->setData(tag.name, TAG_NAME);
			item->setData((qulonglong)tag.lineNumber, TAG_LINE);
			item->setData(scopeKind, SCOPE_KIND);
			item->setData(scopeName, SCOPE_NAME);			
			_model->appendRow(item);
		}else{
			if((scopeKind == "class") && (QString(tag.kindName) == "function")){
				symbol.prepend(scopeName + "::");
				QStandardItem* item = new QStandardItem(getSymbolIcon(tag.kindName, tag.extensionFields.access), symbol);
				item->setData(tag.kindName, TAG_KIND);
				item->setData(tag.name, TAG_NAME);
				item->setData((qulonglong)tag.lineNumber, TAG_LINE);
				item->setData(scopeKind, SCOPE_KIND);
				item->setData(scopeName, SCOPE_NAME);
				_model->appendRow(item);
			}else{
				QStandardItem* parent = getItem(scopeName, scopeKind);
				if(parent){
					QStandardItem* item = new QStandardItem(getSymbolIcon(tag.kindName, tag.extensionFields.access), symbol);
					item->setData(tag.kindName, TAG_KIND);
					item->setData(tag.name, TAG_NAME);
					item->setData((qulonglong)tag.lineNumber, TAG_LINE);
					item->setData(scopeKind, SCOPE_KIND);
					item->setData(scopeName, SCOPE_NAME);
					parent->appendRow(item);
				}else{
					if(scopeName.contains("__anon") && (scopeKind == "namespace")){
						if(anonymousNamespace == 0){
							anonymousNamespace = new QStandardItem(getSymbolIcon("namespace", ""), "anonymous");
							_model->appendRow(anonymousNamespace);
						}
						QStandardItem* item = new QStandardItem(getSymbolIcon(tag.kindName, tag.extensionFields.access), symbol);
						item->setData(tag.kindName, TAG_KIND);
						item->setData(tag.name, TAG_NAME);
						item->setData((qulonglong)tag.lineNumber, TAG_LINE);
						item->setData(scopeKind, SCOPE_KIND);
						item->setData(scopeName, SCOPE_NAME);			
						anonymousNamespace->appendRow(item);
					} else if(scopeName.contains("__anon") && (scopeKind == "enum")){
						QStandardItem* item = new QStandardItem(getSymbolIcon(tag.kindName, tag.extensionFields.access), symbol);
						item->setData(tag.kindName, TAG_KIND);
						item->setData(tag.name, TAG_NAME);
						item->setData((qulonglong)tag.lineNumber, TAG_LINE);
						item->setData(scopeKind, SCOPE_KIND);
						item->setData(scopeName, SCOPE_NAME);
						_model->appendRow(item);						
					}else{
						qWarning() << "ERROR symbol ignored : " << tag.kindName << tag.name << scopeKind << scopeName;
					}
				}
			}
		}
		
		tags = tags->next;
	}
	
	_treeView->expandAll();
	
	freeTagEntryListItem(tagsEntryList);
}

QStandardItem* SymbolManager::getItem(const QString& name_, const QString& kind_){
	QStringList	splittedScope = name_.split("::");
	//start by the end, the required item is probably the last.
	int i = _model->rowCount();
	while(i-- != 0){
		QStandardItem* item = getItemChild(_model->item(i), name_, splittedScope.last(), kind_);
		if(item != 0)
			return item;
	}
	return 0;
}

QStandardItem* SymbolManager::getItemChild(QStandardItem* child_, const QString& fullname_, const QString& name_, const QString& kind_){
	if((child_->data(TAG_KIND).toString() == kind_) && (child_->data(TAG_NAME).toString() == name_)){
		//todo we should check if the item scope is correct
		return child_;
	}
	int i = child_->rowCount();
	while(i--){
		QStandardItem* child = getItemChild(child_->child(i), fullname_, name_, kind_);
		if(child != 0)
			return child;
	}
	return 0;
}

QIcon SymbolManager::getSymbolIcon(const QString& kind_, const QString& access_){
	if(kind_ == "class"){
		return QIcon(":/ctags/class.png");
	}else if(kind_ == "macro"){
		return QIcon(":/ctags/macro.png");
	}else if(kind_ == "enumerator"){
		return QIcon(":/ctags/enumerator.png");
	}else if(kind_ == "function" || kind_ == "prototype"){
		if(access_ == "private")
			return QIcon(":/ctags/function_private.png");
		else if(access_ == "protected")
			return QIcon(":/ctags/function_protected.png");
		else
			return QIcon(":/ctags/function.png");
	}else if(kind_ == "enum"){
		return QIcon(":/ctags/enum.png");
	}else if(kind_ == "member" || kind_ == "local" || kind_ == "variable" || kind_ == "externvar"){
		if(access_ == "private")
			return QIcon(":/ctags/variable_private.png");
		else if(access_ == "protected")
			return QIcon(":/ctags/variable_protected.png");
		else
			return QIcon(":/ctags/variable.png");
	}else if(kind_ == "namespace"){
		return QIcon(":/ctags/namespace.png");
	}else if(kind_ == "struct" || kind_ == "union"){
		return QIcon(":/ctags/structure.png");
	}else if(kind_ == "typedef"){
		return QIcon(":/ctags/typedef.png");
	}
	
	qDebug() << "no icon defined for a " << kind_ << "(" << access_ << ") symbol !";
	return QIcon();
}

void SymbolManager::clicked(const QModelIndex& index_){
	activated(index_);
}

void SymbolManager::activated(const QModelIndex& index_){
	int line = index_.data(TAG_LINE).toInt();
	if(line){
		//emit symbolActivated(index_.data(TAG_NAME).toString(), line);
		emit symbolActivated(line);
	}
}
