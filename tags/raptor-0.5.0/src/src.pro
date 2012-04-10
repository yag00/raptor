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

# include config file
include( ../config.pri )

# include lib framework
include( ../ext/QScintilla/QScintilla.pri )
include( ../ext/AStyle/AStyle.pri )
include( ../ext/qt-solutions/qtsingleapplication/src/qtsingleapplication.pri )
include( ../ext/ctags/ctags.pri )
include( ../ext/dtl/dtl.pri )

TEMPLATE	= app
TARGET		= $$PACKAGE_NAME
DESTDIR		= $$PACKAGE_DESTDIR

CONFIG  += debug_and_release
CONFIG  += warn_on
CONFIG  += help

# install
raptor_bin.path	= $${PACKAGE_BIN}
raptor_bin.files = $${PACKAGE_DESTDIR}/$${PACKAGE_NAME}
raptor_bin.CONFIG *= no_check_exist
INSTALLS += raptor_bin	

INCLUDEPATH += 	$$PWD

DEPENDPATH    *= $$PWD \
				 $$PWD/astyle \
				 $$PWD/ctags \
				 $$PWD/ui \
				 $$PWD/widget \
				 $$PWD/lexer \
				 $$PWD/settings \
				 $$PWD/diff \
				 $$PWD/export \
				 $$PWD/about

HEADERS       *= mainwindow.h \
				SettingsDialog.h \
				SessionSettings.h \
				SessionManager.h \
				SessionDialog.h \				
				search.h \
				explorer.h \
				Settings.h \
				ShortcutEditor.h \
				ShortcutSettings.h \
				splitter.h \
				ScintillaExt.h \
				ColorPushButton.h \
				tabbar.h \
				singleton.h \
				lexer_manager.h \
				RLexerCpp.h \
				RLexerJava.h \
				RLexerHTML.h \
				RLexerXML.h \
				RLexerTxt2Tags.h \
				RLexerMatlab.h \
				RLexerOctave.h \
				document_view.h \
				document_manager.h \
				document_editor.h \
				MenuLabel.h \
				KeySequenceInput.h \
				PathListEditor.h \
				FileListEditor.h \
				StringListEditor.h \
				AStyleIndenter.h \
				SourceIterator.h \
				Exporter.h \
				ExporterHTML.h \
				SymbolManager.h \
				SymbolTreeComboBox.h \
				SymbolComboBoxAction.h \
				HelpBrowser.h \
				UnidiffDialog.h \
				Unidiff.h \
				CompareDialog.h \
				Version.h \
				ApplicationPath.h \
				About.h
				
SOURCES       *= main.cpp \
				mainwindow.cpp \
				SettingsDialog.cpp \
				SessionSettings.cpp \
				SessionManager.cpp \
				SessionDialog.cpp \
				search.cpp \
				explorer.cpp \
				Settings.cpp \
				ShortcutEditor.cpp \
				ShortcutSettings.cpp \
				splitter.cpp \
				ScintillaExt.cpp \
				ColorPushButton.cpp \
				tabbar.cpp \
				lexer_manager.cpp \
				RLexerCpp.cpp \
				RLexerJava.cpp \
				RLexerHTML.cpp \
				RLexerXML.cpp \
				RLexerTxt2Tags.cpp \
				RLexerMatlab.cpp \
				RLexerOctave.cpp \
				document_view.cpp \
				document_manager.cpp \
				document_editor.cpp \
				MenuLabel.cpp \
				KeySequenceInput.cpp \
				PathListEditor.cpp \
				FileListEditor.cpp \
				StringListEditor.cpp \
				AStyleIndenter.cpp \
				ExporterHTML.cpp \
				SymbolManager.cpp \
				SymbolTreeComboBox.cpp \
				SymbolComboBoxAction.cpp \
				HelpBrowser.cpp \
				UnidiffDialog.cpp \
				Unidiff.cpp \
				CompareDialog.cpp \				
				Version.cpp \
				ApplicationPath.cpp \
				About.cpp

FORMS *= mainwindow.ui \
		 settings.ui \
		 shortcut.ui \
		 sessions.ui \
		 search.ui \
		 explorer.ui \
		 macro.ui \
		 compare.ui \
		 unidiff.ui \
		 about.ui
	
RESOURCES     *= ressources.qrc
win32:RC_FILE += rc/ressources.rc

TRANSLATIONS = translations/raptor_fr.ts

LIBS *= -L$${PACKAGE_BUILD_PATH}/lib -lastyle -lqscintilla2 -lctags
# -ldiff


# define variable
DEFINES	*= "PACKAGE_NAME=\"\\\"$${PACKAGE_NAME_CAPITALIZE}\\\"\"" \
		   "PACKAGE_VERSION=\"\\\"$${PACKAGE_VERSION}\\\"\"" \
		   "PACKAGE_DESCRIPTION=\"\\\"$${PACKAGE_DESCRIPTION}\\\"\""

unix{
	# define package install paths
	DEFINES	*= "PACKAGE_BIN=\"\\\"$${PACKAGE_BIN}\\\"\"" \
			   "PACKAGE_LIB=\"\\\"$${PACKAGE_LIB}\\\"\"" \
			   "PACKAGE_DATA=\"\\\"$${PACKAGE_DATA}\\\"\""
}