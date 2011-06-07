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
	return QString(PACKAGE_BIN);
}

QString ApplicationPath::applicationPath(){
	return qApp->applicationDirPath();
}

QString ApplicationPath::libraryPath(){
	if(ApplicationPath::isInstalled())
		return QString(PACKAGE_LIB);
	else
		return ApplicationPath::applicationPath();
}

QString ApplicationPath::translationPath(){
	if(ApplicationPath::isInstalled())
		return QString(PACKAGE_DATA) + QDir::separator() + "translation";
	else
		return ApplicationPath::applicationPath() + QDir::separator() + "translation";
}

QString ApplicationPath::documentationPath(){
	if(ApplicationPath::isInstalled())
		return QString(PACKAGE_DATA) + QDir::separator() + "doc";
	else
		return ApplicationPath::applicationPath() + QDir::separator() + "doc";
}

bool ApplicationPath::isLocal(){
	return ~ApplicationPath::isInstalled();
}

bool ApplicationPath::isInstalled(){
#ifdef Q_OS_WIN
	return true;
#else
	if(ApplicationPath::binaryPath() == ApplicationPath::applicationPath()){
		return true;
	}
#endif
	return false;
}

QString ApplicationPath::applicationLanguage(){
	return "fr";
}

QString ApplicationPath::documentationPathLanguage(){
	return ApplicationPath::documentationPath() + QDir::separator() + ApplicationPath::applicationLanguage();
}
