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