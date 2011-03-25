# include config file
include( ../../config.pri )

include( AStyle.pri )

TEMPLATE = lib
TARGET = astyle

CONFIG  += debug_and_release
CONFIG	+= staticlib

HEADERS	= 	$$ASTYLE_VERSION/src/astyle.h

SOURCES	= 	$$ASTYLE_VERSION/src/ASBeautifier.cpp \
			$$ASTYLE_VERSION/src/ASFormatter.cpp \
			$$ASTYLE_VERSION/src/ASEnhancer.cpp \
			$$ASTYLE_VERSION/src/ASResource.cpp

DESTDIR	= $${PACKAGE_BUILD_PATH}/lib

