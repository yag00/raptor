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

#ifndef __SEARCH_H__
#define __SEARCH_H__

#include <QMenu>
#include <QWidget>
#include "ui_search.h"

enum SearchDomain { SD_FILE, SD_OPENFILES, SD_ALLFILES };

class DocumentManager;
class DocumentEditor;

class Search : public QWidget, private Ui::Search{
    Q_OBJECT
	public:
		Search(DocumentManager* documentMgr_, QWidget *parent_ = 0);
		~Search();

		bool isCaseSensitive() const;
		bool isWrap() const;
		bool isWordOnly() const;
		bool isRegExp() const;
		
	public slots:
		virtual void setVisible(bool visible_);

		void next();
		void prev();
		void searchAll();		
		void replace();
		void replaceAll();
		
	signals:
		void caseSensitiveButtonToggled(bool checked);
		void wrapButtonToggled(bool checked);
		void wordOnlyButtonToggled(bool checked);
		void regexpButtonToggled(bool checked);
		
	protected slots:
		//automatic connected slots
		/** called when the find line edit changed */ 
		void on_leFind_textChanged(const QString&);
		void on_twResults_activated(const QModelIndex&);
		void on_tbCollapseResult_pressed();

		void domainChanged(QAction *act_);
		void setSearchDirectory();
		
		/** internal seach function 
		 * @param fw_ if true search forward else backward*/		
		void doSearch(bool fw_);
		void doSimpleSearch(bool fw_);
		void doDocumentSearch(bool fw_, bool fix_);
		void doSearchReplaceAll(bool replace_);
		int doFileSearch(DocumentEditor *document_, bool replace_);
		int doOpenFilesSearch(bool replace_);
		int doAllFilesSearch(bool replace_);
		void searchIncremental();		
		
		void showMessage(const QString &text_);
		void showMessageNotFound(const QString &text_);
		void showSearchResults(bool found_);
		void clearMessage();
		void clearResults();
				
		void addResult(const QString &file_, const QString &text_, int lineFrom_, int lineTo_, int indexFrom_, int indexTo_);
		
	protected:
		virtual void contextMenuEvent(QContextMenuEvent *event_);
		virtual bool event(QEvent *event_);
		
		QStringList find();
		QStringList findFiles(QString directory_, bool recursive_);
		void updateComboBox(QComboBox *comboBox_);
		
	private:
		DocumentManager* _documentMgr;
		
		QMenu *_domainMenu;
		
		QString _lastSearch;
		QString _lastSearchFile;
		QTreeWidgetItem *_lastSearchItem;
		bool _newSearch;
};

#endif // __SEARCH_H__
