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

#include "PathListEditor.h"

PathListEditor::PathListEditor( QWidget* parent, const QString& title, const QString& path ) :
	FileListEditor( parent, title, path, QString() ) 
{	
	findChild<QToolBar*>()->actions().last()->setIcon(QIcon( ":/listeditor/images/listeditor/folder.png")); 
}

void PathListEditor::onAddItem() {
	// get directory
	QString s = QFileDialog::getExistingDirectory(this, tr( "Choose directory" ), _path );
	
	if ( !s.isEmpty() )
	{
		QListWidgetItem* it = new QListWidgetItem( s, _list );
		it->setFlags( it->flags() | Qt::ItemIsEditable );
		_list->setCurrentItem( it );
		_list->scrollToItem( it );
		emit edited();
	}
}

void PathListEditor::onEditItem() {
	if ( QListWidgetItem* it = _list->selectedItems().value( 0 ) )
	{
		QString s= QFileDialog::getExistingDirectory(this, tr( "Choose directory" ), _path );
		if ( !s.isEmpty() )
		{
			it->setText( s );
			emit edited();
		}
	}
}

#if WAF
#include "PathListEditor.moc"
#endif

