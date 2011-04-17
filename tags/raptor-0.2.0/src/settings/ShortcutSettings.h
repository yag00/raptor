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

#ifndef __SHORTCUT_SETTINGS_H__
#define __SHORTCUT_SETTINGS_H__

#include <QSettings>

class QAction;

/** handle all shortcut settings of the application */
class ShortcutSettings : public QSettings{

	Q_OBJECT

	public:
		ShortcutSettings(QObject* parent_ = 0);
		~ShortcutSettings();

		bool hasDefaultShortcut();
		void restoreDefaultShortcut(QAction* action_ = 0);
	
		void saveDefaultShortcut(QAction* action_);
		void saveUserShortcut(QAction* action_);
	
		QKeySequence getDefaultShortcut(QAction* action_); 
		QKeySequence getUserShortcut(QAction* action_);
	
		void updateActionWithDefaultShortcut(QAction* action_);
		void updateActionWithUserShortcut(QAction* action_);

};

#endif // __SHORTCUT_SETTINGS_H__
