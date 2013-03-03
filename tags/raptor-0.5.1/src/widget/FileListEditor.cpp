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

#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QListWidgetItem>

#include "FileListEditor.h"

FileListEditor::FileListEditor(QWidget* parent, const QString& title, const QString& path, const QString& filter )
	: StringListEditor(title, parent), _path( path ), _filter( filter )
{ 
	findChild<QToolBar*>()->actions().last()->setIcon(QIcon( ":/listeditor/file.png" )); 
}

void FileListEditor::onAddItem() {
	// get files
	QStringList l = QFileDialog::getOpenFileNames(this, tr("Choose file(s)"), _path, _filter );
	
	if ( !l.isEmpty() )
	{
		foreach ( QString s, l )
		{
			QListWidgetItem* it = new QListWidgetItem( s, _list );
			it->setFlags( it->flags() | Qt::ItemIsEditable );
			_list->setCurrentItem( it );
			_list->scrollToItem( it );
		}
		emit edited();
	}
}

void FileListEditor::onEditItem() {
	if ( QListWidgetItem* it = _list->selectedItems().value( 0 ) )
	{
		QString s= QFileDialog::getOpenFileName(this, tr("Choose file"), _path, _filter );
		if ( !s.isEmpty() )
		{
			it->setText( s );
			emit edited();
		}
	}
}

void FileListEditor::setPath( const QString& path ) {
	_path = path;
}

#if WAF
#include "FileListEditor.moc"
#endif

