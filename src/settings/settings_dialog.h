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

#ifndef __SETTINGS_DIALOG_H__
#define __SETTINGS_DIALOG_H__

#include <QDialog>

#include "ui_settings.h"
#include "settings.h"

class QButtonGroup;
class QScintilla;

class SettingsDialog : public QDialog, private Ui::UISettings{
	
	Q_OBJECT

	public:
		SettingsDialog(MainWindow& mainWindow_, Settings* settings_, QWidget* parent_ = 0);
		~SettingsDialog();
	
	public slots:
		void accept();
		void apply();
	
	protected:
		void loadSettingsDialog();
		void saveSettingsDialog();
		
	private slots:
		/** connect menu to stack widget */
		void on_twMenu_itemSelectionChanged();
		/** select font buttons @todo find to way to use only one slot */
		void tbMarginFonts_clicked();
		void tbDocumentFonts_clicked();
		/** api page */
		void cbSourceAPIsLanguages_beforeChanged(int);
		void on_cbSourceAPIsLanguages_currentIndexChanged(int);
		void on_pbSourceAPIsDelete_clicked();
		void on_pbSourceAPIsAdd_clicked();
		void on_pbSourceAPIsBrowse_clicked();
		/** lexer association */
		void on_twLexersAssociations_itemSelectionChanged();
		void on_pbLexersAssociationsAddChange_clicked();
		void on_pbLexersAssociationsDelete_clicked();
		/** lexer highlighting */
		void on_cbLexersHighlightingLanguages_currentIndexChanged(const QString&);
		void on_lwLexersHighlightingElements_itemSelectionChanged();
		void lexersHighlightingColour_clicked();
		void lexersHighlightingFont_clicked();
		void on_cbLexersHighlightingFillEol_clicked(bool);
		void cbLexersHighlightingProperties_clicked(bool);
		void on_cbLexersHighlightingIndentationWarning_currentIndexChanged(int);
		void on_pbLexersHighlightingReset_clicked();
		
		/** astyle */
		void onStyleChanged();
		
	private:	
		QButtonGroup* _bgToolBarIconSize;
		QButtonGroup* _bgCallTipsStyle;
		QButtonGroup* _bgBraceMatch;
		QButtonGroup* _bgEdgeMode;
		QButtonGroup* _bgFoldStyle;
		QButtonGroup* _bgEolMode;
		QButtonGroup* _bgWhitespaceVisibility;
		QButtonGroup* _bgWrapMode;
		QButtonGroup* _bgStartWrapVisualFlag;
		QButtonGroup* _bgEndWrapVisualFlag;
		QButtonGroup* _bgAutoCompletionSource;
		QButtonGroup* _bgBomEncoding;
		
		QHash<QString,QsciLexer*> _lexers;

		QsciScintilla* _astyleExample;
		QButtonGroup* _bgASStyle;
		QButtonGroup* _bgASIndent;
		QButtonGroup* _bgASBracket;
		
		MainWindow& _mainWindow;		
		Settings* _settings;
};

#endif // __SETTINGS_DIALOG_H__
