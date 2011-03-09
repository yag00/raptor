#ifndef __FILE_LIST_EDITOR_H__
#define __FILE_LIST_EDITOR_H__

#include "StringListEditor.h"

/**	@brief a StringListEditor that handle files name. */
class FileListEditor : public StringListEditor {
	Q_OBJECT
	
	public:
		FileListEditor(QWidget* parent = 0, const QString& title = QString(), const QString& path = QString(), const QString& filter = QString());
		
		/** Set the default path used by QFileDialog
		 * @param path The path to make default */
		void setPath( const QString& path );

	protected slots:
		virtual void onAddItem();
		virtual void onEditItem();

	protected:
		QString _path;
		QString _filter;
};

#endif // __FILE_LIST_EDITOR_H__
