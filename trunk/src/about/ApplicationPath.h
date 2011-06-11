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

#ifndef __APPLICATION_PATH_H__
#define __APPLICATION_PATH_H__

#include <QString>

class ApplicationPath {
	public:
		/** @return the current application language (ie en, fr ...) */
		static QString applicationLanguage();
		/** @return path from where application is executed*/
		static QString applicationPath();
		/** @return binary install path */
		static QString binaryPath();
		/** @return library install path */
		static QString libraryPath();
		/** @return translation install path */
		static QString translationPath();
		/** @return documentation install path */
		static QString documentationPath();
	
		/** @return true is the application is local ie not installed */
		static bool isLocal();
		/** @return true is the application is installed in PREFIX on the system */
		static bool isInstalled();
};


#endif /* __APPLICATION_PATH_H__ */
