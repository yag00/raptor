#ifndef __PATH_LIST_EDITOR_H__
#define __PATH_LIST_EDITOR_H__

#include "FileListEditor.h"

/*!
	\brief a pPathListEditor that handle paths name.
*/
class PathListEditor : public FileListEditor {
	Q_OBJECT
	
	public:
		PathListEditor( QWidget* parent = 0, const QString& title = QString(), const QString& path = QString() );

	protected slots:
		virtual void onAddItem();
		virtual void onEditItem();
};

#endif // __PATH_LIST_EDITOR_H__
