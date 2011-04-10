#
# This file is part of the raptor project.
#
# Copyright (C) 2011 Christophe Duvernois <christophe.duvernois@gmail.com>
#
# Raptor is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Raptor is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
#


# get current path
PACKAGE_PWD	= $$PWD

# package name & version 
PACKAGE_NAME	= raptor
PACKAGE_VERSION	= 0.1.0
PACKAGE_DESCRIPTION	= "Raptor A Portable Text editOR"
PACKAGE_NAME_CAPITALIZE	= Raptor

PACKAGE_NAME_STRING	= "PACKAGE_NAME"
PACKAGE_VERSION_STRING	= "$$PACKAGE_VERSION"

QMAKE_CXXFLAGS += -Werror

# define config mode paths
CONFIG( debug, debug|release ) {
	#Debug
	CONFIG	+= console
	PACKAGE_BUILD_PATH	= $${PACKAGE_PWD}/build/debug
	PACKAGE_DESTDIR	= $${PACKAGE_PWD}/delivery/debug
} else {
	#Release
	PACKAGE_BUILD_PATH	= $${PACKAGE_PWD}/build/release
	PACKAGE_DESTDIR	= $${PACKAGE_PWD}/delivery/release
}

OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/obj
UI_DIR	= $${PACKAGE_BUILD_PATH}/ui
MOC_DIR	= $${PACKAGE_BUILD_PATH}/moc
RCC_DIR	= $${PACKAGE_BUILD_PATH}/rcc