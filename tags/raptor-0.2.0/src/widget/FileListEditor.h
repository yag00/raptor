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
