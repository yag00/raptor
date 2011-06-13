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
#include <QApplication>
#include <QDir>
#include "ApplicationPath.h"

QString ApplicationPath::binaryPath(){
#ifdef Q_OS_WIN
	return ApplicationPath::applicationPath();
#else
	return QString(PACKAGE_BIN);
#endif
}

QString ApplicationPath::applicationPath(){
	return qApp->applicationDirPath();
}

QString ApplicationPath::libraryPath(){
#ifdef Q_OS_WIN
	return ApplicationPath::applicationPath();
#else
	if(ApplicationPath::isInstalled())
		return QString(PACKAGE_LIB);
	else
		return ApplicationPath::applicationPath();
#endif
}

QString ApplicationPath::translationPath(){
#ifdef Q_OS_WIN
	return ApplicationPath::applicationPath() + QDir::separator() + "translation" + QDir::separator();
#else
	if(ApplicationPath::isInstalled())
		return QString(PACKAGE_DATA) + QDir::separator() + "translation" + QDir::separator();
	else
		return ApplicationPath::applicationPath() + QDir::separator() + "translation" + QDir::separator();
#endif
}

QString ApplicationPath::documentationPath(){
#ifdef Q_OS_WIN
	return ApplicationPath::applicationPath() + QDir::separator() + "doc" + QDir::separator();
#else
	if(ApplicationPath::isInstalled())
		return QString(PACKAGE_DATA) + QDir::separator() + "doc" + QDir::separator();
	else
		return ApplicationPath::applicationPath() + QDir::separator() + "doc" + QDir::separator();
#endif
}

bool ApplicationPath::isLocal(){
	return ~ApplicationPath::isInstalled();
}

bool ApplicationPath::isInstalled(){
	if(ApplicationPath::binaryPath() == ApplicationPath::applicationPath()){
		return true;
	}
	return false;
}

QString ApplicationPath::applicationLanguage(){
	return "en";
}
