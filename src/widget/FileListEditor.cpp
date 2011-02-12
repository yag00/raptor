#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QListWidgetItem>

#include "FileListEditor.h"

FileListEditor::FileListEditor(QWidget* parent, const QString& title, const QString& path, const QString& filter )
	: StringListEditor(title, parent), _path( path ), _filter( filter )
{ 
	findChild<QToolBar*>()->actions().last()->setIcon(QIcon( ":/listeditor/images/listeditor/file.png" )); 
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
