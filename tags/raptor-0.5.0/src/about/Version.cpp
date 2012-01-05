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

#include "Version.h"
#include <QString>
#include <QStringList>

Version::Version(const char* version_){
	QStringList version = QString(version_).split(".");
	_major = version[0].toUInt();
	_minor = version[1].toUInt();
	_patch = version[2].toUInt();
}

Version::Version(const std::string& version_){
	QStringList version = QString(version_.c_str()).split(".");
	_major = version[0].toUInt();
	_minor = version[1].toUInt();
	_patch = version[2].toUInt();
}

Version::Version(const QString& version_){
	QStringList version = version_.split(".");
	_major = version[0].toUInt();
	_minor = version[1].toUInt();
	_patch = version[2].toUInt();
}

Version::Version(uint32_t major_, uint32_t minor_, uint32_t patch_) : 
	_major(major_), _minor(minor_), _patch(patch_){
}

Version::~Version(){
	
}

QString Version::getVersion() const{
	return QString("%1.%2.%3").arg(_major).arg(_minor).arg(_patch);
}

QString Version::getVersionXY() const{
	return QString("%1.%2").arg(_major).arg(_minor);
}

uint32_t Version::getMajor() const{
	return _major;
}

uint32_t Version::getMinor() const{
	return _minor;
}

uint32_t Version::getPatch() const{
	return _patch;
}

bool Version::isNull(){
	return ((*this) == Version(0,0,0));
}

bool Version::operator== (Version const & v_){
	if (_major == v_._major && _minor == v_._minor && _patch == v_._patch)
        return true;
    else
        return false;	
}
bool Version::operator!=(const Version& v_){
	if (_major == v_._major && _minor == v_._minor && _patch == v_._patch)
        return false;
    else
        return true;
}
bool Version::operator<(const Version& v_){
	//check major
	if(_major > v_._major)
		return false;
	if(_major < v_._major)
		return true;
	//check minor
	if(_minor > v_._minor)
		return false;
	if(_minor < v_._minor)
		return true;
	//check patch
	if(_patch >= v_._patch)
		return false;
	if(_patch < v_._patch)
		return true;
	
	//never reached
	return false;
}
bool Version::operator<=(const Version& v_){
	//check major
	if(_major > v_._major)
		return false;
	if(_major < v_._major)
		return true;
	//check minor
	if(_minor > v_._minor)
		return false;
	if(_minor < v_._minor)
		return true;
	//check patch
	if(_patch > v_._patch)
		return false;
	if(_patch <= v_._patch)
		return true;
	
	//never reached
	return false;	
}
bool Version::operator>(const Version& v_){
	if((*this) <= v_)
		return false;
	return true;
}
bool Version::operator>=(const Version& v_){
	if((*this) < v_)
		return false;
	return true;	
}

QString Version::gccVersion(){
	return QString("%1.%2.%3").arg(__GNUC__).arg(__GNUC_MINOR__).arg(__GNUC_PATCHLEVEL__);
}

#ifdef Q_OS_WIN
QString Version::mingwVersion(){
	return QString("%1.%2.%3").arg(__MINGW32_MAJOR_VERSION).arg(__MINGW32_MINOR_VERSION).arg(__MINGW32_PATCHLEVEL);
}
#endif
