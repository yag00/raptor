# include config file
include( ../config.pri )
# include lib framework
include( ../ext/QScintilla/QScintilla.pri )
include( ../ext/AStyle/AStyle.pri )
include( ../ext/qt-solutions/qtsingleapplication/src/qtsingleapplication.pri )
#include( ../ext/GoogleDiffMatchPatch/GoogleDiffMatchPatch.pri )
#include( ../ext/dtl/dtl.pri )


TEMPLATE	= app
TARGET		= $$PACKAGE_NAME
DESTDIR		= $$PACKAGE_DESTDIR


INCLUDE_PATH *= . $$PWD $$PWD/ui $$PWD/widget $$PWD/lexer $$PWD/settings $$PWD/astyle $$PWD/about

DEPENDPATH    *= $$PWD \
				 $$PWD/astyle \
				 $$PWD/ui \
				 $$PWD/widget \
				 $$PWD/lexer \
				 $$PWD/settings \
				 $$PWD/about

HEADERS       *= mainwindow.h \
				settings_dialog.h \
				search.h \
				settings.h \
				splitter.h \
				ScintillaExt.h \
				ColorPushButton.h \
				tabbar.h \
				singleton.h \
				lexer_manager.h \
				lexerCpp.h \
				document_view.h \
				document_manager.h \
				document_editor.h \
				PathListEditor.h \
				FileListEditor.h \
				StringListEditor.h \
				AStyleIndenter.h \
				SourceIterator.h \
				Diff.h \
				About.h
				
SOURCES       *= main.cpp \
				mainwindow.cpp \
				settings_dialog.cpp \
				search.cpp \
				settings.cpp \
				splitter.cpp \
				ScintillaExt.cpp \
				ColorPushButton.cpp \
				tabbar.cpp \
				lexer_manager.cpp \
				lexerCpp.cpp \
				document_view.cpp \
				document_manager.cpp \
				document_editor.cpp \
				PathListEditor.cpp \
				FileListEditor.cpp \
				StringListEditor.cpp \
				AStyleIndenter.cpp \
				Diff.cpp \
				About.cpp

FORMS *= mainwindow.ui \
		 settings.ui \
		 search.ui \
		 macro.ui \
		 about.ui

RESOURCES     *= ressources.qrc

LIBS += -L$${PACKAGE_BUILD_PATH}/lib -lastyle -lqscintilla2
# -ldiff


# define variable for source code
DEFINES	*= "PACKAGE_NAME=\"\\\"$${PACKAGE_NAME}\\\"\"" \
		   "PACKAGE_VERSION=\"\\\"$${PACKAGE_VERSION}\\\"\"" \
		   "PACKAGE_DESCRIPTION=\"\\\"$${PACKAGE_DESCRIPTION}\\\"\"" \
