# include config file
include( ../../config.pri )

include( QScintilla.pri )

# include original project
include( $$QSCINTILLAVERSION/Qt4/qscintilla.pro )

# include path path
INCLUDEPATH	*= $$QSCINTILLAVERSION/include
INCLUDEPATH	*= $$QSCINTILLAVERSION/src
INCLUDEPATH	*= $$QSCINTILLAVERSION/Qt4
# depend path
DEPENDPATH	*= $$QSCINTILLAVERSION/Qt4

# overwrite some values to made no lib
CONFIG	-= dll
win32{
	CONFIG 	-= release
}else {
	CONFIG 	+= debug_and_release
}

CONFIG	+= staticlib

# overrride install script of qscintilla
INSTALLS	-= header trans qsci target

header.path = $${PACKAGE_BUILD_PATH}/qscintilla/include/Qsci
header.files = $${QSCINTILLA_PWD}/$$QSCINTILLAVERSION/Qt4/Qsci/*.h

INSTALLS += header

DESTDIR	= $${PACKAGE_BUILD_PATH}/lib

