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
