# get current path
PACKAGE_PWD	= $$PWD
# package name & version 
PACKAGE_NAME	= Raptor
PACKAGE_VERSION	= 0.1.0
PACKAGE_DESCRIPTION	= "Raptor A Portable Text editOR"

PACKAGE_NAME_STRING	= "PACKAGE_NAME"
PACKAGE_VERSION_STRING	= "$$PACKAGE_VERSION"

#CONFIG += debug_and_release

# define config mode paths
CONFIG( debug, debug|release ) {
	#Debug
	message( Building in DEBUG for architecture $$QT_ARCH )
	CONFIG	+= console
	PACKAGE_BUILD_PATH	= $${PACKAGE_PWD}/build/debug
	unix:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/.obj/unix
	win32:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/.obj/win32
	mac:OBJECTS_DIR		= $${PACKAGE_BUILD_PATH}/.obj/mac
	UI_DIR	= $${PACKAGE_BUILD_PATH}/.ui
	MOC_DIR	= $${PACKAGE_BUILD_PATH}/.moc
	RCC_DIR	= $${PACKAGE_BUILD_PATH}/.rcc
	PACKAGE_DESTDIR	= $${PACKAGE_PWD}/delivery/debug
} else {
	#Release
	message( Building in RELEASE for architecture $$QT_ARCH )
	PACKAGE_BUILD_PATH	= $${PACKAGE_PWD}/build/release
	unix:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/.obj/unix
	win32:OBJECTS_DIR	= $${PACKAGE_BUILD_PATH}/.obj/win32
	mac:OBJECTS_DIR		= $${PACKAGE_BUILD_PATH}/.obj/mac
	UI_DIR	= $${PACKAGE_BUILD_PATH}/.ui
	MOC_DIR	= $${PACKAGE_BUILD_PATH}/.moc
	RCC_DIR	= $${PACKAGE_BUILD_PATH}/.rcc
	PACKAGE_DESTDIR	= $${PACKAGE_PWD}/delivery/release
}
