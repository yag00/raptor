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

#include <QtGui>
#include <QtDebug>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>
#include <Qsci/qscilexerpython.h>

#include "../astyle/AStyleIndenter.h"
#include "../lexer/RLexerCpp.h"
#include "../widget/ScintillaExt.h"
#include "settings.h"
#include "settings_dialog.h"

SettingsDialog::SettingsDialog(MainWindow& mainWindow_, QWidget* parent) : QDialog(parent), _mainWindow(mainWindow_) {
	_settings = new Settings(this);
	//init ui
	setupUi(this);

	//init astyle intentation page
	QFile file(":/astyle/rc/indent-sample.cpp");
	file.open(QFile::ReadOnly);
	tePreview->setText(QString::fromUtf8(file.readAll()));
	tePreview->setLexer(new RLexerCPP(tePreview));
	tePreview->setWhitespaceVisibility(QsciScintilla::WsVisible);
	file.close();

	// connect radio button
	_bgASStyle = new QButtonGroup(gbStyles);
	connect(_bgASStyle, SIGNAL(buttonClicked(int)), this, SLOT(onStyleChanged()));
	_bgASStyle->addButton(rbANSI, astyle::STYLE_ALLMAN);
	_bgASStyle->addButton(rbKR, astyle::STYLE_KR);
	_bgASStyle->addButton(rbLinux, astyle::STYLE_LINUX);
	_bgASStyle->addButton(rbGNU, astyle::STYLE_GNU);
	_bgASStyle->addButton(rbJava, astyle::STYLE_JAVA);
	_bgASStyle->addButton(rbStroustrup, astyle::STYLE_STROUSTRUP);
	_bgASStyle->addButton(rbWhitesmith, astyle::STYLE_WHITESMITH);
	_bgASStyle->addButton(rbBanner, astyle::STYLE_BANNER);
	_bgASStyle->addButton(rbHorstmann, astyle::STYLE_HORSTMANN);
	_bgASStyle->addButton(rb1TBS, astyle::STYLE_1TBS);
	_bgASStyle->addButton(rbCustom, astyle::STYLE_NONE);

	_bgASIndent = new QButtonGroup(gbStyles);
	connect(_bgASIndent, SIGNAL(buttonClicked(int)), this, SLOT(onStyleChanged()));
	_bgASIndent->addButton(rbIndentWithSpaces, AS_INDENT_WITH_SPACES);
	_bgASIndent->addButton(rbIndentWithTabs, AS_INDENT_WITH_TABS);
	_bgASIndent->addButton(rbForceIndentWithTabs, AS_FORCE_INDENT_WITH_TABS);

	_bgASBracket = new QButtonGroup(gbStyles);
	connect(_bgASBracket, SIGNAL(buttonClicked(int)), this, SLOT(onStyleChanged()));
	_bgASBracket->addButton(rbBracketNoChange, astyle::NONE_MODE);
	_bgASBracket->addButton(rbBracketAttach, astyle::ATTACH_MODE);
	_bgASBracket->addButton(rbBracketBreak, astyle::BREAK_MODE);
	_bgASBracket->addButton(rbBracketLinux, astyle::LINUX_MODE);
	_bgASBracket->addButton(rbBracketStroustrup, astyle::STROUSTRUP_MODE);
	_bgASBracket->addButton(rbBracketHorstmann, astyle::HORSTMANN_MODE);

	connect(chkIndentClasses, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkIndentSwitches, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkIndentCase, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkIndentBrackets, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkIndentBlocks, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkIndentNamespaces, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkIndentLabels, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkIndentPreprocessor, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkIndentColumnOneComment, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));

	connect(chkBreakHeaderBlocks, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkBreakAllBlocks, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkPadOperators, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkPadParenthesisOutside, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkPadParenthesisInside, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkPadHeader, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkUnpadParenthesis, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkDeleteEmptyLines, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkFillEmptyLines, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));

	connect(chkBreakClosingBrackets, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkBreakIfElseStatements, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkAddBrackets, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkAddOneLineBrackets, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkKeepOneLineBlocks, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkKeepOneLineStatements, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(chkCOnvertTabs, SIGNAL(stateChanged(int)), this, SLOT(onStyleChanged()));
	connect(cbPointerAlign, SIGNAL(currentIndexChanged(int)), this, SLOT(onStyleChanged()));

	//init menu
	twMenu->topLevelItem(3)->setExpanded(true);
	twMenu->setCurrentItem(twMenu->topLevelItem(0));

	foreach (ColorPushButton* b, findChildren<ColorPushButton*>()){
		b->setColorNameHidden(true);
		//b->setIconSize( QSize( 32, 16 ) );
	}

	//create lexers
	foreach (QString s, Settings::availableLanguages())
		_lexers[s] = LexerManager::lexerFactory(s);

	// Translations
	cbTranslation->addItems(Settings::availableTranslations());

	// loads text codecs
	cbDefaultCodec->addItems(Settings::availableTextCodecs());
	_bgBomEncoding = new QButtonGroup(gbBomBehaviour);
	_bgBomEncoding->addButton(rbDoNotAddOrRemoveBom, 0);
	_bgBomEncoding->addButton(rbAlwaysAddBom, 2);
	_bgBomEncoding->addButton(rbAlwaysRemoveBom, 3);

	// fill lexers combo
	cbSourceAPIsLanguages->addItems(Settings::availableLanguages());
	cbLexersAssociationsLanguages->addItems(Settings::availableLanguages());
	cbLexersHighlightingLanguages->addItems(Settings::availableLanguages());

	// toolbar icon
	_bgToolBarIconSize = new QButtonGroup(gbToolBar);
	_bgToolBarIconSize->addButton(rbBigToolBarIcon, 32);
	_bgToolBarIconSize->addButton(rbMediumToolBarIcon, 22);
	_bgToolBarIconSize->addButton(rbSmallToolBarIcon, 16);

	// auto completion
	_bgAutoCompletionSource = new QButtonGroup(gbAutoCompletionSource);
	_bgAutoCompletionSource->addButton(rbAcsDocument, QsciScintilla::AcsDocument);
	_bgAutoCompletionSource->addButton(rbAcsAPIs, QsciScintilla::AcsAPIs);
	_bgAutoCompletionSource->addButton(rbAcsAll, QsciScintilla::AcsAll);

	// calltips style
	_bgCallTipsStyle = new QButtonGroup(gbCalltipsEnabled);
	_bgCallTipsStyle->addButton(rbCallTipsNoContext, QsciScintilla::CallTipsNoContext);
	_bgCallTipsStyle->addButton(rbCallTipsNoAutoCompletionContext, QsciScintilla::CallTipsNoAutoCompletionContext);
	_bgCallTipsStyle->addButton(rbCallTipsContext, QsciScintilla::CallTipsContext);

	// brace match
	_bgBraceMatch = new QButtonGroup(gbBraceMatchingEnabled);
	_bgBraceMatch->addButton(rbStrictBraceMatch, QsciScintilla::StrictBraceMatch);
	_bgBraceMatch->addButton(rbSloppyBraceMatch, QsciScintilla::SloppyBraceMatch);

	// edge mode
	_bgEdgeMode = new QButtonGroup(gbEdgeModeEnabled);
	_bgEdgeMode->addButton(rbEdgeLine, QsciScintilla::EdgeLine);
	_bgEdgeMode->addButton(rbEdgeBackground, QsciScintilla::EdgeBackground);

	// fold style
	_bgFoldStyle = new QButtonGroup(gbFoldMarginEnabled);
	_bgFoldStyle->addButton(rbPlainFoldStyle, ScintillaExt::PlainFoldStyle);
	_bgFoldStyle->addButton(rbCircledTreeFoldStyle, ScintillaExt::CircledTreeFoldStyle);
	_bgFoldStyle->addButton(rbCircledFoldStyle, ScintillaExt::CircledFoldStyle);
	_bgFoldStyle->addButton(rbBoxedFoldStyle, ScintillaExt::BoxedFoldStyle);
	_bgFoldStyle->addButton(rbBoxedTreeFoldStyle, ScintillaExt::BoxedTreeFoldStyle);
	_bgFoldStyle->addButton(rbArrowedFoldStyle, ScintillaExt::ArrowFoldStyle);
	_bgFoldStyle->addButton(rbArrowedTreeFoldStyle, ScintillaExt::ArrowTreeFoldStyle);

	// eol mode
	_bgEolMode = new QButtonGroup(gbEolMode);
	_bgEolMode->addButton(rbEolUnix, QsciScintilla::EolUnix);
	_bgEolMode->addButton(rbEolMac, QsciScintilla::EolMac);
	_bgEolMode->addButton(rbEolWindows, QsciScintilla::EolWindows);

	// whitespace visibility
	_bgWhitespaceVisibility = new QButtonGroup(gbWhitespaceVisibilityEnabled);
	_bgWhitespaceVisibility->addButton(rbWsVisible, QsciScintilla::WsVisible);
	_bgWhitespaceVisibility->addButton(rbWsVisibleAfterIndent, QsciScintilla::WsVisibleAfterIndent );

	// wrap mode
	_bgWrapMode = new QButtonGroup(gbWrapModeEnabled);
	_bgWrapMode->addButton(rbWrapWord, QsciScintilla::WrapWord);
	_bgWrapMode->addButton(rbWrapCharacter, QsciScintilla::WrapCharacter);

	// wrap visual flag
	_bgStartWrapVisualFlag = new QButtonGroup(wStartWrapVisualFlags);
	_bgStartWrapVisualFlag->addButton(rbStartWrapFlagByText, QsciScintilla::WrapFlagByText);
	_bgStartWrapVisualFlag->addButton(rbStartWrapFlagByBorder, QsciScintilla::WrapFlagByBorder);
	_bgEndWrapVisualFlag = new QButtonGroup(wEndWrapVisualFlags);
	_bgEndWrapVisualFlag->addButton(rbEndWrapFlagByText, QsciScintilla::WrapFlagByText);
	_bgEndWrapVisualFlag->addButton(rbEndWrapFlagByBorder, QsciScintilla::WrapFlagByBorder);

	//  Lexers Highlighting
	// python indentation warning
	cbLexersHighlightingIndentationWarning->addItem( tr( "No warning" ), QsciLexerPython::NoWarning );
	cbLexersHighlightingIndentationWarning->addItem( tr( "Inconsistent" ), QsciLexerPython::Inconsistent );
	cbLexersHighlightingIndentationWarning->addItem( tr( "Tabs after spaces" ), QsciLexerPython::TabsAfterSpaces );
	cbLexersHighlightingIndentationWarning->addItem( tr( "Spaces" ), QsciLexerPython::Spaces );
	cbLexersHighlightingIndentationWarning->addItem( tr( "Tabs" ), QsciLexerPython::Tabs );

	// loads application settings
	loadSettingsDialog();

	// connect button
	connect(dbbButtons->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(apply()));
	connect(cbUseForeground, SIGNAL(stateChanged(int)), tbSelectionForeground, SLOT(setState(int)));
	connect(tbDocumentFont, SIGNAL(clicked()), this, SLOT(tbDocumentFonts_clicked())); // document font
	connect(tbMarginsFont, SIGNAL(clicked()), this, SLOT(tbMarginFonts_clicked())); // margin font

	// lexer elements highlighting
	connect(pbLexersHighlightingForeground, SIGNAL(clicked()), this, SLOT(lexersHighlightingColour_clicked()));
	connect(pbLexersHighlightingBackground, SIGNAL(clicked()), this, SLOT(lexersHighlightingColour_clicked()));
	connect(pbLexersHighlightingFont, SIGNAL(clicked()), this, SLOT(lexersHighlightingFont_clicked()));
	connect(pbLexersHighlightingAllForeground, SIGNAL(clicked()), this, SLOT(lexersHighlightingColour_clicked()));
	connect(pbLexersHighlightingAllBackground, SIGNAL(clicked()), this, SLOT(lexersHighlightingColour_clicked()));
	connect(pbLexersHighlightingAllFont, SIGNAL(clicked()), this, SLOT(lexersHighlightingFont_clicked()));
	foreach (QCheckBox* cb, gbLexersHighlightingElements->findChildren<QCheckBox*>()){
		if ( cb != cbLexersHighlightingFillEol )
			connect( cb, SIGNAL( clicked( bool ) ), this, SLOT( cbLexersHighlightingProperties_clicked( bool ) ) );
	}
	
	// resize to minimum size
	resize(minimumSizeHint());
}

SettingsDialog::~SettingsDialog(){
	//delete lexers
	foreach (QString s, Settings::availableLanguages()){
		delete _lexers[s];
		_lexers.remove(s);
	}
}

void SettingsDialog::accept(){
	apply();
	QDialog::accept();
}

void SettingsDialog::apply(){
	saveSettingsDialog();
	_settings->apply(_mainWindow);
}

void SettingsDialog::loadSettingsDialog(){
	// General
	// Paths

	/** Interface **/
	//	- ToolBar
	if (_bgToolBarIconSize->button(_settings->getToolBarIconSize()))
		_bgToolBarIconSize->button(_settings->getToolBarIconSize())->setChecked(true);

	cbToolBarFile->setChecked(_settings->getShowToolBar("File"));
	cbToolBarEdit->setChecked(_settings->getShowToolBar("Edit"));
	cbToolBarView->setChecked(_settings->getShowToolBar("View"));
	cbToolBarMacro->setChecked(_settings->getShowToolBar("Macro"));
	cbToolBarSymbol->setChecked(_settings->getShowToolBar("Symbol"));
	cbDockSymbol->setChecked(_settings->getShowToolBar("DockSymbol", false));
	
	// - Status Bar
	if(_settings->getShowStatusBar())
		cbStatusBar->setCheckState(Qt::Checked);
	else
		cbStatusBar->setCheckState(Qt::Unchecked);

	/** Editor **/
	//  - General
	cbAutoTrim->setChecked(_settings->autoTrimOnSave());
	cbAddNewLine->setChecked(_settings->addNewLineOnSave());
	
	cbUseForeground->setChecked(_settings->useSelectionForegroundColor());
	tbSelectionForeground->setEnabled(_settings->useSelectionForegroundColor());
	tbSelectionBackground->setColor(_settings->getSelectionBackgroundColor());
	tbSelectionForeground->setColor(_settings->getSelectionForegroundColor());

	tbDocumentPen->setColor(_settings->getDocumentPen());
	tbDocumentPaper->setColor(_settings->getDocumentPaper());
	lDocumentFontName->setFont(_settings->getDocumentFont());
	lDocumentFontName->setText(_settings->getDocumentFont().family());

	//  - Encoding
	cbDefaultCodec->setCurrentIndex(cbDefaultCodec->findText(_settings->getDefaultCodec()));
	if(_settings->getUnicodeBomUseMode() == 1){
		_bgBomEncoding->button(0)->setChecked(true);
		cbAddBomToNewFile->setCheckState(Qt::Checked);
	}else{
		_bgBomEncoding->button(_settings->getUnicodeBomUseMode())->setChecked(true);
		cbAddBomToNewFile->setCheckState(Qt::Unchecked);
	}

	if(_settings->getTryAutoDetectCodec())
		cbTryCharsetDetection->setCheckState(Qt::Checked);
	else
		cbTryCharsetDetection->setCheckState(Qt::Unchecked);


	//  - Completion
	gbAutoCompletionEnabled->setChecked(_settings->getAutoCompletionSource() != QsciScintilla::AcsNone );
	cbAutoCompletionCaseSensitivity->setChecked(_settings->getAutoCompletionCaseSensitivity());
	cbAutoCompletionReplaceWord->setChecked(_settings->getAutoCompletionReplaceWord());
	cbAutoCompletionShowSingle->setChecked(_settings->getAutoCompletionShowSingle());
	sAutoCompletionThreshold->setValue(_settings->getAutoCompletionThreshold());
	if (_bgAutoCompletionSource->button(_settings->getAutoCompletionSource()))
		_bgAutoCompletionSource->button(_settings->getAutoCompletionSource())->setChecked( true );
	//  - Call Tips
	gbCalltipsEnabled->setChecked(_settings->getCallTipsStyle() != QsciScintilla::CallTipsNone);
	sCallTipsVisible->setValue(_settings->getCallTipsVisible());
	if (_bgCallTipsStyle->button(_settings->getCallTipsStyle()))
		_bgCallTipsStyle->button(_settings->getCallTipsStyle())->setChecked(true);
	tbCalltipsBackground->setColor(_settings->getCallTipsBackgroundColor());
	tbCalltipsForeground->setColor(_settings->getCallTipsForegroundColor());
	tbCalltipsHighlight->setColor(_settings->getCallTipsHighlightColor());

	//  - Indentation
	cbAutoIndent->setChecked(_settings->getAutoIndent());
	cbBackspaceUnindents->setChecked(_settings->getBackspaceUnindents());
	cbIndentationGuides->setChecked(_settings->getIndentationGuides());
	cbIndentationUseTabs->setChecked(_settings->getIndentationsUseTabs());
	cbAutodetectIndent->setChecked(_settings->getAutoDetectIndent());
	cbTabIndents->setChecked(_settings->getTabIndents());
	sIndentationTabWidth->setValue(_settings->getTabWidth());
	sIndentationWidth->setValue(_settings->getIndentationWidth());
	tbIndentationGuidesBackground->setColor(_settings->getIndentationGuidesBackgroundColor());
	tbIndentationGuidesForeground->setColor(_settings->getIndentationGuidesForegroundColor());

	//	AStyle Indenter
	_settings->beginGroup("AStyle");
	_bgASStyle->button(_settings->value("style", astyle::STYLE_NONE).toInt())->setChecked(true);
	_bgASIndent->button(_settings->value("indentation_type", AS_FORCE_INDENT_WITH_TABS).toInt())->setChecked(true);
	spbSpacePerTab->setValue(_settings->value("indentation_spt", 4 ).toInt());
	_bgASBracket->button(_settings->value("bracket", astyle::NONE_MODE).toInt())->setChecked(true);

	chkIndentClasses->setChecked(_settings->value("indent_classes", false ).toBool());
	chkIndentSwitches->setChecked(_settings->value("indent_switches", false ).toBool());
	chkIndentCase->setChecked(_settings->value("indent_case", false ).toBool());
	chkIndentBrackets->setChecked(_settings->value("indent_brackets", false ).toBool());
	chkIndentBlocks->setChecked(_settings->value("indent_blocks", false ).toBool());
	chkIndentNamespaces->setChecked(_settings->value("indent_namespaces", false ).toBool());
	chkIndentLabels->setChecked(_settings->value("indent_labels", false ).toBool());
	chkIndentPreprocessor->setChecked(_settings->value("indent_preprocessor", false ).toBool());
	chkIndentColumnOneComment->setChecked(_settings->value("indent_column1comment", false ).toBool());

	chkBreakHeaderBlocks->setChecked(_settings->value("break_header_block", false ).toBool());
	chkBreakAllBlocks->setChecked(_settings->value("break_all_block", false ).toBool());
	chkPadOperators->setChecked(_settings->value("pad_operators", false ).toBool());
	chkPadParenthesisOutside->setChecked(_settings->value("pad_parenthesis_outside", false ).toBool());
	chkPadParenthesisInside->setChecked(_settings->value("pad_parenthesis_inside", false ).toBool());
	chkPadHeader->setChecked(_settings->value("pad_header", false ).toBool());
	chkUnpadParenthesis->setChecked(_settings->value("unpad_parenthesis", false ).toBool());
	chkDeleteEmptyLines->setChecked(_settings->value("delete_empty_lines", false ).toBool());
	chkFillEmptyLines->setChecked(_settings->value("fill_empty_lines", false ).toBool());

	chkBreakClosingBrackets->setChecked(_settings->value("break_closing_brackets", false ).toBool());
	chkBreakIfElseStatements->setChecked(_settings->value("break_elseifs", false ).toBool());
	chkAddBrackets->setChecked(_settings->value("add_brackets", false ).toBool());
	chkAddOneLineBrackets->setChecked(_settings->value("add_one_line_brackets", false ).toBool());
	chkKeepOneLineBlocks->setChecked(_settings->value("keep_one_line_blocks", false ).toBool());
	chkKeepOneLineStatements->setChecked(_settings->value("keep_one_line_statements", false ).toBool());
	chkCOnvertTabs->setChecked(_settings->value("convert_tabs", false ).toBool());
	cbPointerAlign->setCurrentIndex(_settings->value("pointer_align", 0).toInt());
	_settings->endGroup();
	onStyleChanged();

	//  - Brace Matching
	gbBraceMatchingEnabled->setChecked(_settings->getBraceMatching() != QsciScintilla::NoBraceMatch);
	if (_bgBraceMatch->button(_settings->getBraceMatching()))
		_bgBraceMatch->button(_settings->getBraceMatching())->setChecked(true);
	tbMatchedBraceForeground->setColor(_settings->getMatchedBraceForegroundColor());
	tbMatchedBraceBackground->setColor(_settings->getMatchedBraceBackgroundColor());
	tbUnmatchedBraceBackground->setColor(_settings->getUnmatchedBraceBackgroundColor());
	tbUnmatchedBraceForeground->setColor(_settings->getUnmatchedBraceForegroundColor());

	//  - Edge Mode
	gbEdgeModeEnabled->setChecked(_settings->getEdgeMode() != QsciScintilla::EdgeNone );
	if (_bgEdgeMode->button(_settings->getEdgeMode()))
		_bgEdgeMode->button(_settings->getEdgeMode())->setChecked(true);
	sEdgeColumnNumber->setValue(_settings->getEdgeColumn());
	tbEdgeColor->setColor(_settings->getEdgeColor());

	//  - Caret
	gbCaretLineVisible->setChecked(_settings->getCaretLineVisible());
	tbCaretLineBackground->setColor(_settings->getCaretLineBackgroundColor());
	tbCaretForeground->setColor(_settings->getCaretForegroundColor());
	sCaretWidth->setValue(_settings->getCaretWidth());

	//  - Margins
	lMarginFontName->setText(_settings->getMarginFont().family());
	lMarginFontName->setFont(_settings->getMarginFont());
	gbMarginEnabled->setChecked(_settings->getMarginEnabled());
	tbMarginsForeground->setColor(_settings->getMarginForegroundColor());
	tbMarginsBackground->setColor(_settings->getMarginBackgroundColor());

	gbMarginBookmarkEnabled->setChecked(_settings->getMarginBookmarkEnabled());

	gbFoldMarginEnabled->setChecked(_settings->getFolding() != QsciScintilla::NoFoldStyle);
	if (_bgFoldStyle->button(_settings->getFolding()))
		_bgFoldStyle->button(_settings->getFolding())->setChecked(true);
	tbFoldMarginForeground->setColor(_settings->getFoldMarginForegroundColor());
	tbFoldMarginBackground->setColor(_settings->getFoldMarginBackgroundColor());
	tbFoldSymbolForeground->setColor(_settings->getFoldSymbolForegroundColor());
	tbFoldSymbolBackground->setColor(_settings->getFoldSymbolBackgroundColor());

	//  - Special Characters
	_bgEolMode->button(_settings->getEolMode())->setChecked(true);
	cbEolVisibility->setChecked(_settings->getEolVisibility());
	cbAutoDetectEol->setChecked(_settings->getAutoDetectEol());
	gbWhitespaceVisibilityEnabled->setChecked(_settings->getWhitespaceVisibility() != QsciScintilla::WsInvisible);
	if (_bgWhitespaceVisibility->button(_settings->getWhitespaceVisibility()))
		_bgWhitespaceVisibility->button(_settings->getWhitespaceVisibility())->setChecked(true);
	gbWrapModeEnabled->setChecked(_settings->getWrapMode() != QsciScintilla::WrapNone);
	if (_bgWrapMode->button(_settings->getWrapMode()))
		_bgWrapMode->button(_settings->getWrapMode())->setChecked(true);
	gbWrapVisualFlagsEnabled->setChecked(_settings->getWrapVisualFlagsEnabled());
	if (_bgStartWrapVisualFlag->button(_settings->getStartWrapVisualFlag()))
		_bgStartWrapVisualFlag->button(_settings->getStartWrapVisualFlag())->setChecked(true);
	if (_bgEndWrapVisualFlag->button(_settings->getEndWrapVisualFlag()))
		_bgEndWrapVisualFlag->button(_settings->getEndWrapVisualFlag())->setChecked(true);
	sWrappedLineIndentWidth->setValue(_settings->getWrappedLineIndentWidth());

	// APIs
	for (int i = 0; i < cbSourceAPIsLanguages->count(); i++)
		cbSourceAPIsLanguages->setItemData(i, _settings->value("API/" + cbSourceAPIsLanguages->itemText(i)).toStringList());
	if (cbSourceAPIsLanguages->count() > 0)
		cbSourceAPIsLanguages->setCurrentIndex(0);

	//  Lexers Associations
	QMap<QString, QStringList> l = _settings->getAssociations();
	foreach (QString k, l.keys())	{
		foreach ( QString e, l.value( k ) )	{
			QTreeWidgetItem* it = new QTreeWidgetItem( twLexersAssociations );
			it->setText(0, k);
			it->setText(1, e);
		}
	}

	//  Lexers Highlighting
	foreach (QsciLexer* lex, _lexers)
		lex->readSettings(*_settings);

	if ( cbLexersHighlightingLanguages->count() )
		on_cbLexersHighlightingLanguages_currentIndexChanged( cbLexersHighlightingLanguages->itemText(0));
	
	// Show/Hide lexer	
	QStringList invisibleLexers = _settings->getInvisibleLexers();
	foreach(QString lexerName, _settings->availableLanguages()){
		if(invisibleLexers.contains(lexerName)){
			lvHideLexer->addItem(lexerName);
		}else{
			lvShowLexer->addItem(lexerName);
		}
	}
	
	//Switcher
	cbSwitcherNotify->setCheckState((Qt::CheckState)_settings->getSwitcherNotification());
	QMap<QString, QStringList> switcherList = _settings->getSwitcherValues();
	foreach (QString k, switcherList.keys())	{
		QString value;
		foreach ( QString e, switcherList.value( k ) )	{
			value += e;
			value += ";";
		}
		QTreeWidgetItem* it = new QTreeWidgetItem(twSwitcher);
		it->setText(0, k);
		it->setText(1, value);
	}
}

void SettingsDialog::saveSettingsDialog(){
	// General
	// Interface
	{
		//	- ToolBar
		_settings->setToolBarIconSize(_bgToolBarIconSize->checkedId());
		_settings->setShowToolBar("File", cbToolBarFile->checkState());
		_settings->setShowToolBar("Edit", cbToolBarEdit->checkState());
		_settings->setShowToolBar("View", cbToolBarView->checkState());
		_settings->setShowToolBar("Macro", cbToolBarMacro->checkState());
		_settings->setShowToolBar("Symbol", cbToolBarSymbol->checkState());
		_settings->setShowToolBar("DockSymbol", cbDockSymbol->checkState());
		// - Status Bar
		_settings->setShowStatusBar(cbStatusBar->checkState());
	}
	// Editor

	//  General
	_settings->setAutoTrimOnSave(cbAutoTrim->checkState());
	_settings->setAddNewLineOnSave(cbAddNewLine->checkState());
	
	_settings->setDefaultCodec(cbDefaultCodec->currentText());

	_settings->useSelectionForegroundColor(cbUseForeground->isChecked());
	_settings->setSelectionBackgroundColor(tbSelectionBackground->color());
	_settings->setSelectionForegroundColor(tbSelectionForeground->color());

	_settings->setDocumentPen(tbDocumentPen->color());
	_settings->setDocumentPaper(tbDocumentPaper->color());
	_settings->setDocumentFont(lDocumentFontName->font());

	//  Encoding
	_settings->setDefaultCodec(cbDefaultCodec->currentText());
	if(_bgBomEncoding->checkedId() == 0){
		if(cbAddBomToNewFile->checkState())
			_settings->setUnicodeBomUseMode(1);
		else
			_settings->setUnicodeBomUseMode(0);
	}else{
		_settings->setUnicodeBomUseMode(_bgBomEncoding->checkedId());
	}
	_settings->setTryAutoDetectCodec((bool)cbTryCharsetDetection->checkState());

	//  AutoCompletion
	if (gbAutoCompletionEnabled->isChecked())
		_settings->setAutoCompletionSource((QsciScintilla::AutoCompletionSource)_bgAutoCompletionSource->checkedId());
	else
		_settings->setAutoCompletionSource(QsciScintilla::AcsNone);
	_settings->setAutoCompletionCaseSensitivity(cbAutoCompletionCaseSensitivity->isChecked());
	_settings->setAutoCompletionReplaceWord(cbAutoCompletionReplaceWord->isChecked());
	_settings->setAutoCompletionShowSingle(cbAutoCompletionShowSingle->isChecked());
	_settings->setAutoCompletionThreshold(sAutoCompletionThreshold->value());

	//  Call Tips
	_settings->setCallTipsStyle(QsciScintilla::CallTipsNone);
	if (gbCalltipsEnabled->isChecked())
		_settings->setCallTipsStyle((QsciScintilla::CallTipsStyle)_bgCallTipsStyle->checkedId());
	_settings->setCallTipsVisible(sCallTipsVisible->value());
	_settings->setCallTipsBackgroundColor(tbCalltipsBackground->color());
	_settings->setCallTipsForegroundColor(tbCalltipsForeground->color());
	_settings->setCallTipsHighlightColor(tbCalltipsHighlight->color());

	//  Indentation
	_settings->setAutoIndent(cbAutoIndent->isChecked());
	_settings->setBackspaceUnindents(cbBackspaceUnindents->isChecked());
	_settings->setIndentationGuides(cbIndentationGuides->isChecked());
	_settings->setIndentationsUseTabs(cbIndentationUseTabs->isChecked());
	_settings->setTabIndents(cbTabIndents->isChecked());
	_settings->setAutoDetectIndent(cbAutodetectIndent->isChecked());
	_settings->setTabWidth(sIndentationTabWidth->value());
	_settings->setIndentationWidth(sIndentationWidth->value());
	_settings->setIndentationGuidesBackgroundColor(tbIndentationGuidesBackground->color());
	_settings->setIndentationGuidesForegroundColor(tbIndentationGuidesForeground->color());

	//	AStyle Indenter
	_settings->beginGroup("AStyle");
	_settings->setValue("style", _bgASStyle->checkedId());
	_settings->setValue("indentation_type", _bgASIndent->checkedId());
	_settings->setValue("indentation_spt", spbSpacePerTab->value());
	_settings->setValue("bracket", _bgASBracket->checkedId());

	_settings->setValue("indent_classes", chkIndentClasses->isChecked());
	_settings->setValue("indent_switches", chkIndentSwitches->isChecked());
	_settings->setValue("indent_case", chkIndentCase->isChecked());
	_settings->setValue("indent_brackets", chkIndentBrackets->isChecked());
	_settings->setValue("indent_blocks", chkIndentBlocks->isChecked());
	_settings->setValue("indent_namespaces", chkIndentNamespaces->isChecked());
	_settings->setValue("indent_labels", chkIndentLabels->isChecked());
	_settings->setValue("indent_preprocessor", chkIndentPreprocessor->isChecked());
	_settings->setValue("indent_column1comment", chkIndentColumnOneComment->isChecked());

	_settings->setValue("break_header_block",  chkBreakHeaderBlocks->isChecked());
	_settings->setValue("break_all_block",  chkBreakAllBlocks->isChecked());
	_settings->setValue("pad_operators",  chkPadOperators->isChecked());
	_settings->setValue("pad_parenthesis_outside",  chkPadParenthesisOutside->isChecked());
	_settings->setValue("pad_parenthesis_inside",  chkPadParenthesisInside->isChecked());
	_settings->setValue("pad_header",  chkPadHeader->isChecked());
	_settings->setValue("unpad_parenthesis",  chkUnpadParenthesis->isChecked());
	_settings->setValue("delete_empty_lines",  chkDeleteEmptyLines->isChecked());
	_settings->setValue("fill_empty_lines", chkFillEmptyLines->isChecked());

	_settings->setValue("break_closing_brackets", chkBreakClosingBrackets->isChecked());
	_settings->setValue("break_elseifs", chkBreakIfElseStatements->isChecked());
	_settings->setValue("add_brackets", chkAddBrackets->isChecked());
	_settings->setValue("add_one_line_brackets", chkAddOneLineBrackets->isChecked());
	_settings->setValue("keep_one_line_blocks", chkKeepOneLineBlocks->isChecked());
	_settings->setValue("keep_one_line_statements", chkKeepOneLineStatements->isChecked());
	_settings->setValue("convert_tabs", chkCOnvertTabs->isChecked());
	_settings->setValue("pointer_align", cbPointerAlign->currentIndex());
	_settings->endGroup();


	//  Brace Matching
	_settings->setBraceMatching(QsciScintilla::NoBraceMatch);
	if (gbBraceMatchingEnabled->isChecked())
		_settings->setBraceMatching((QsciScintilla::BraceMatch)_bgBraceMatch->checkedId());
	_settings->setMatchedBraceBackgroundColor(tbMatchedBraceBackground->color());
	_settings->setMatchedBraceForegroundColor(tbMatchedBraceForeground->color());
	_settings->setUnmatchedBraceBackgroundColor(tbUnmatchedBraceBackground->color());
	_settings->setUnmatchedBraceForegroundColor(tbUnmatchedBraceForeground->color());

	//  Edge Mode
	_settings->setEdgeMode(QsciScintilla::EdgeNone);
	if (gbEdgeModeEnabled->isChecked())
		_settings->setEdgeMode((QsciScintilla::EdgeMode)_bgEdgeMode->checkedId());
	_settings->setEdgeColumn(sEdgeColumnNumber->value());
	_settings->setEdgeColor(tbEdgeColor->color());

	//  Caret
	_settings->setCaretLineVisible(gbCaretLineVisible->isChecked());
	_settings->setCaretLineBackgroundColor(tbCaretLineBackground->color());
	_settings->setCaretForegroundColor(tbCaretForeground->color());
	_settings->setCaretWidth(sCaretWidth->value());

	//  Margins
	_settings->setMarginEnabled(gbMarginEnabled->isChecked());
	_settings->setMarginForegroundColor(tbMarginsForeground->color());
	_settings->setMarginBackgroundColor(tbMarginsBackground->color());
	_settings->setMarginFont(lMarginFontName->font());

	_settings->setMarginBookmarkEnabled(gbMarginBookmarkEnabled->isChecked());

	_settings->setFoldMarginEnabled(gbFoldMarginEnabled->isChecked());
	_settings->setFolding(QsciScintilla::NoFoldStyle);
	if (gbFoldMarginEnabled->isChecked())
		_settings->setFolding((QsciScintilla::FoldStyle)_bgFoldStyle->checkedId());
	_settings->setFoldMarginForegroundColor(tbFoldMarginForeground->color());
	_settings->setFoldMarginBackgroundColor(tbFoldMarginBackground->color());
	_settings->setFoldSymbolForegroundColor(tbFoldSymbolForeground->color());
	_settings->setFoldSymbolBackgroundColor(tbFoldSymbolBackground->color());

	//  Special Characters
	_settings->setEolMode((QsciScintilla::EolMode)_bgEolMode->checkedId());
	_settings->setEolVisibility(cbEolVisibility->isChecked());
	_settings->setAutoDetectEol(cbAutoDetectEol->isChecked());
	_settings->setWhitespaceVisibility(QsciScintilla::WsInvisible);
	if (gbWhitespaceVisibilityEnabled->isChecked())
		_settings->setWhitespaceVisibility((QsciScintilla::WhitespaceVisibility)_bgWhitespaceVisibility->checkedId());
	_settings->setWrapMode(QsciScintilla::WrapNone);
	if (gbWrapModeEnabled->isChecked())
		_settings->setWrapMode((QsciScintilla::WrapMode)_bgWrapMode->checkedId());
	_settings->setWrapVisualFlagsEnabled(gbWrapVisualFlagsEnabled->isChecked());
	_settings->setStartWrapVisualFlag(QsciScintilla::WrapFlagNone);
	if (gbWrapVisualFlagsEnabled->isChecked())
		_settings->setStartWrapVisualFlag((QsciScintilla::WrapVisualFlag)_bgStartWrapVisualFlag->checkedId());
	_settings->setEndWrapVisualFlag(QsciScintilla::WrapFlagNone);
	if (gbWrapVisualFlagsEnabled->isChecked())
		_settings->setEndWrapVisualFlag((QsciScintilla::WrapVisualFlag)_bgEndWrapVisualFlag->checkedId());
	_settings->setWrappedLineIndentWidth(sWrappedLineIndentWidth->value());

	// Source APIs
	for (int i = 0; i < cbSourceAPIsLanguages->count(); i++)
		_settings->setValue("API/" + cbSourceAPIsLanguages->itemText(i), cbSourceAPIsLanguages->itemData(i).toStringList());

	// Lexers Associations
	QMap<QString, QStringList> lexerAssociation;
	for ( int i = 0; i < twLexersAssociations->topLevelItemCount(); i++ ){
		QTreeWidgetItem* it = twLexersAssociations->topLevelItem(i);
		lexerAssociation[ it->text(0) ] << it->text(1);
	}
	_settings->setAssociations(lexerAssociation);

	//  Lexers Highlighting
	foreach (QsciLexer* lex, _lexers){
		lex->setDefaultPaper(_settings->getDocumentPaper());
		lex->setDefaultColor(_settings->getDocumentPen());
		lex->writeSettings(*_settings);
	}
	
	// Show/Hide lexer
	QStringList invisibleLexers;
	for(int i = 0; i < lvHideLexer->count(); i++){
		invisibleLexers << lvHideLexer->item(i)->text();
	}
	_settings->setInvisibleLexers(invisibleLexers);
	
	//Switcher
	_settings->setSwitcherNotification((bool)cbSwitcherNotify->checkState());
	QMap<QString, QStringList> switcherValue;
	for ( int i = 0; i < twSwitcher->topLevelItemCount(); i++ ){
		QTreeWidgetItem* it = twSwitcher->topLevelItem(i);
		switcherValue[ it->text(0) ] << it->text(1).split(";");
	}
	_settings->setSwitcherValues(switcherValue);
}

void SettingsDialog::on_twMenu_itemSelectionChanged(){
	// get item
	QTreeWidgetItem* it = twMenu->selectedItems().value(0);

	if(it){
		lInformations->setText(it->text(0));
		int i = twMenu->indexOfTopLevelItem(it);
		if (!it->parent()){
			switch(i){
				case 0:
				case 1:
				case 2:
				case 3:
					swPages->setCurrentIndex(i);
					break;
				default:
					swPages->setCurrentIndex(i + 13);
					break;
			}
		}
		else{
			int indexOfParent = twMenu->indexOfTopLevelItem(it->parent());
			int indexOfChild = it->parent()->indexOfChild(it);
			swPages->setCurrentIndex(indexOfParent + indexOfChild);
		}
	}
}

void SettingsDialog::tbMarginFonts_clicked(){
	bool b;
	QFont f = QFontDialog::getFont(&b, lMarginFontName->font(), window());
	if (b){
		lMarginFontName->setText(f.family());
		lMarginFontName->setFont(f);
	}
}
void SettingsDialog::tbDocumentFonts_clicked(){
	bool b;
	QFont f = QFontDialog::getFont(&b, lDocumentFontName->font(), window());
	if (b){
		lDocumentFontName->setText(f.family());
		lDocumentFontName->setFont(f);
	}
}

void SettingsDialog::cbSourceAPIsLanguages_beforeChanged(int i){
	if (i == cbSourceAPIsLanguages->currentIndex()){
		QStringList l;
		for (int j = 0; j < lwSourceAPIs->count(); j++)
			l << lwSourceAPIs->item( j )->text();
		cbSourceAPIsLanguages->setItemData(i, l);
	}
}
void SettingsDialog::on_cbSourceAPIsLanguages_currentIndexChanged(int i){
	lwSourceAPIs->clear();
	lwSourceAPIs->addItems(cbSourceAPIsLanguages->itemData(i).toStringList());
}
void SettingsDialog::on_pbSourceAPIsDelete_clicked(){
	// get selected item
	QListWidgetItem* it = lwSourceAPIs->selectedItems().value(0);
	if (it)	{
		delete it;
		cbSourceAPIsLanguages_beforeChanged(cbSourceAPIsLanguages->currentIndex());
	}
}
void SettingsDialog::on_pbSourceAPIsAdd_clicked(){
	// get files
	QStringList files = leSourceAPIs->text().split(";", QString::SkipEmptyParts);
	// add them recursively
	foreach (const QString& fn, files){
		if (lwSourceAPIs->findItems( fn, Qt::MatchFixedString ).count() == 0)
			lwSourceAPIs->addItem(fn);
	}
	// clear input
	leSourceAPIs->clear();
	// save datas
	cbSourceAPIsLanguages_beforeChanged(cbSourceAPIsLanguages->currentIndex());
}
void SettingsDialog::on_pbSourceAPIsBrowse_clicked(){
	QStringList files = QFileDialog::getOpenFileNames(window(), tr("Select API files"), QString::null, tr( "API Files (*.api);;All Files (*)"));
	if (!files.isEmpty())
		leSourceAPIs->setText(files.join(";"));
}

void SettingsDialog::on_twLexersAssociations_itemSelectionChanged(){
	QTreeWidgetItem* it = twLexersAssociations->selectedItems().value(0);
	if (it)	{
		cbLexersAssociationsLanguages->setCurrentIndex(cbLexersAssociationsLanguages->findText(it->text(0)));
		leLexersAssociationsFilenamePattern->setText(it->text(1));
	}
}
void SettingsDialog::on_pbLexersAssociationsAddChange_clicked(){
	QString f = leLexersAssociationsFilenamePattern->text();
	QString l = cbLexersAssociationsLanguages->currentText();
	if (f.isEmpty() || l.isEmpty())
		return;
	QTreeWidgetItem* it = twLexersAssociations->selectedItems().value(0);
	if ( !it || it->text(1) != f ) {
		// check if item with same parameters already exists
		QList<QTreeWidgetItem*> l = twLexersAssociations->findItems(f, Qt::MatchFixedString);
		if (l.count())
			it = l.at(0);
		else
			it = new QTreeWidgetItem(twLexersAssociations);
	}
	it->setText(0, l);
	it->setText(1, f);
	twLexersAssociations->setCurrentItem(0);
	twLexersAssociations->selectionModel()->clear();
	leLexersAssociationsFilenamePattern->clear();
	cbLexersAssociationsLanguages->setCurrentIndex(-1);
}
void SettingsDialog::on_pbLexersAssociationsDelete_clicked(){
	QTreeWidgetItem* it = twLexersAssociations->selectedItems().value(0);
	if (it){
		delete it;
		twLexersAssociations->setCurrentItem(0);
		twLexersAssociations->selectionModel()->clear();
		leLexersAssociationsFilenamePattern->clear();
		cbLexersAssociationsLanguages->setCurrentIndex(-1);
	}
}

void SettingsDialog::on_twSwitcher_itemSelectionChanged(){
	QTreeWidgetItem* it = twSwitcher->selectedItems().value(0);
	if (it)	{
		leSwitcherFilenamePattern->setText(it->text(0));
		leSwitchTo->setText(it->text(1));
	}
}
void SettingsDialog::on_pbSwitcherAddChange_clicked(){
	QString f = leSwitcherFilenamePattern->text();
	QString l = leSwitchTo->text();
	if (f.isEmpty() || l.isEmpty())
		return;
	QTreeWidgetItem* it = twSwitcher->selectedItems().value(0);
	if (it == 0) {
		QList<QTreeWidgetItem*> itList = twSwitcher->findItems(f, Qt::MatchFixedString);
		if (itList.count()){
			it = itList.at(0);
			QString s = it->text(1);
			s.append(QString(";"));
			s.append(l);
			it->setText(1, s);
		}else{
			it = new QTreeWidgetItem(twSwitcher);
			it->setText(0, f);
			it->setText(1, l);
		}
	}
	twSwitcher->setCurrentItem(0);
	twSwitcher->selectionModel()->clear();
	leSwitcherFilenamePattern->clear();
	leSwitchTo->clear();
}
void SettingsDialog::on_pbSwitcherDelete_clicked(){
	QTreeWidgetItem* it = twSwitcher->selectedItems().value(0);
	if (it){
		delete it;
		twSwitcher->setCurrentItem(0);
		twSwitcher->selectionModel()->clear();
		leSwitcherFilenamePattern->clear();
		leSwitchTo->clear();
	}
}

void SettingsDialog::on_cbLexersHighlightingLanguages_currentIndexChanged(const QString& s){
	QsciLexer* lexer = _lexers.value(s);
		
	//clear the widget list
	lwLexersHighlightingElements->clear();
	for ( int i = 0; i < 128; i++ )	{
		QString desc = lexer->description(i);
		if (!desc.isEmpty()) {
			QListWidgetItem* it = new QListWidgetItem(lwLexersHighlightingElements);
			it->setText(desc);
			it->setForeground(lexer->color(i));
			it->setBackground(lexer->paper(i));
			it->setFont(lexer->font(i));
			it->setData(Qt::UserRole, i);
		}
	}

	// value
	QVariant v;

	// fold comments
	v = LexerManager::lexerProperty( "foldComments", lexer );
	cbLexersHighlightingFoldComments->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldComments->setChecked( v.toBool() );

	// fold compact
	v = LexerManager::lexerProperty( "foldCompact", lexer );
	cbLexersHighlightingFoldCompact->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldCompact->setChecked( v.toBool() );

	// fold quotes
	v = LexerManager::lexerProperty( "foldQuotes", lexer );
	cbLexersHighlightingFoldQuotes->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldQuotes->setChecked( v.toBool() );

	// fold directives
	v = LexerManager::lexerProperty( "foldDirectives", lexer );
	cbLexersHighlightingFoldDirectives->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldDirectives->setChecked( v.toBool() );

	// fold at begin
	v = LexerManager::lexerProperty( "foldAtBegin", lexer );
	cbLexersHighlightingFoldAtBegin->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldAtBegin->setChecked( v.toBool() );

	// fold at parenthesis
	v = LexerManager::lexerProperty( "foldAtParenthesis", lexer );
	cbLexersHighlightingFoldAtParenthesis->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldAtParenthesis->setChecked( v.toBool() );

	// fold at else
	v = LexerManager::lexerProperty( "foldAtElse", lexer );
	cbLexersHighlightingFoldAtElse->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldAtElse->setChecked( v.toBool() );

	// fold at module
	v = LexerManager::lexerProperty( "foldAtModule", lexer );
	cbLexersHighlightingFoldAtModule->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldAtModule->setChecked( v.toBool() );

	// fold preprocessor
	v = LexerManager::lexerProperty( "foldPreprocessor", lexer );
	cbLexersHighlightingFoldPreprocessor->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingFoldPreprocessor->setChecked( v.toBool() );

	// style preprocessor
	v = LexerManager::lexerProperty( "stylePreprocessor", lexer );
	cbLexersHighlightingStylePreprocessor->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingStylePreprocessor->setChecked( v.toBool() );

	// indent opening brace
	cbLexersHighlightingIndentOpeningBrace->setChecked( lexer->autoIndentStyle() & QsciScintilla::AiOpening );
	// indent closing brace
	cbLexersHighlightingIndentClosingBrace->setChecked( lexer->autoIndentStyle() & QsciScintilla::AiClosing );

	// case sensitive tags
	v = LexerManager::lexerProperty( "caseSensitiveTags", lexer );
	cbLexersHighlightingCaseSensitiveTags->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingCaseSensitiveTags->setChecked( v.toBool() );

	// backslash escapes
	v = LexerManager::lexerProperty( "backslashEscapes", lexer );
	cbLexersHighlightingBackslashEscapes->setVisible( v.isValid() );
	if ( v.isValid() )
		cbLexersHighlightingBackslashEscapes->setChecked( v.toBool() );

	// indentation warning
	v = LexerManager::lexerProperty( "indentationWarning", lexer );
	lLexersHighlightingIndentationWarning->setVisible( v.isValid() );
	cbLexersHighlightingIndentationWarning->setVisible( lLexersHighlightingIndentationWarning->isVisible() );
	if ( v.isValid() )
		cbLexersHighlightingIndentationWarning->setCurrentIndex( cbLexersHighlightingIndentationWarning->findData( v.toInt() ) );

}
void SettingsDialog::on_lwLexersHighlightingElements_itemSelectionChanged(){
	QListWidgetItem* it = lwLexersHighlightingElements->selectedItems().value(0);
	if (it)
		cbLexersHighlightingFillEol->setChecked(_lexers.value(cbLexersHighlightingLanguages->currentText())->eolFill(it->data(Qt::UserRole).toInt()));
}
void SettingsDialog::lexersHighlightingColour_clicked(){
	// get sender
	QObject* obj = sender();
	// color
	QColor color;
	// element colour
	if (obj == pbLexersHighlightingForeground || obj == pbLexersHighlightingBackground) {
		// get item
		QListWidgetItem* it = lwLexersHighlightingElements->selectedItems().value(0);
		// cancel if no item
		if (!it)
			return;

		// get color
		color = QColorDialog::getColor(obj == pbLexersHighlightingForeground ? it->foreground().color() : it->background().color(), window());
		// apply color
		if (color.isValid() ) {
			if (obj == pbLexersHighlightingForeground) {
				it->setForeground(color);
				_lexers.value(cbLexersHighlightingLanguages->currentText())->setColor(color, it->data( Qt::UserRole ).toInt());
			}
			else if (obj == pbLexersHighlightingBackground) {
				it->setBackground(color);
				_lexers.value(cbLexersHighlightingLanguages->currentText())->setPaper(color, it->data(Qt::UserRole).toInt());
			}
		}
	}
	// gobal color
	else if (obj == pbLexersHighlightingAllForeground || obj == pbLexersHighlightingAllBackground) {
		// get lexer
		QsciLexer* l = _lexers.value( cbLexersHighlightingLanguages->currentText() );
		// get color
		color = QColorDialog::getColor( obj == pbLexersHighlightingAllForeground ? l->color( -1 ) : l->paper( -1 ), window() );
		// apply
		if ( color.isValid() )
		{
			if ( obj == pbLexersHighlightingAllForeground )
				l->setColor( color, -1 );
			else if ( obj == pbLexersHighlightingAllBackground )
				l->setPaper( color, -1 );
			// refresh
			on_cbLexersHighlightingLanguages_currentIndexChanged( l->language() );
		}
	}
}
void SettingsDialog::lexersHighlightingFont_clicked(){
	// get sender
	QObject* obj = sender();
	// values
	bool b;
	QFont f;
	// element font
	if ( obj == pbLexersHighlightingFont ) {
		// get item
		QListWidgetItem* it = lwLexersHighlightingElements->selectedItems().value( 0 );
		// cancel if no item
		if ( !it )
			return;
		// get font
		f = QFontDialog::getFont( &b, it->font(), window() );
		// apply
		if (b) {
			it->setFont( f );
			_lexers.value( cbLexersHighlightingLanguages->currentText() )->setFont( f, it->data( Qt::UserRole ).toInt() );
		}
	}
	// global font
	else if ( obj == pbLexersHighlightingAllFont ) {
		// get lexer
		QsciLexer* l = _lexers.value( cbLexersHighlightingLanguages->currentText() );
		// get font
		f = QFontDialog::getFont( &b, l->font( -1 ), window() );
		// apply
		if (b) {
			l->setFont( f, -1 );
			on_cbLexersHighlightingLanguages_currentIndexChanged( l->language() );
		}
	}
}
void SettingsDialog::on_cbLexersHighlightingFillEol_clicked(bool b){
	QListWidgetItem* it = lwLexersHighlightingElements->selectedItems().value(0);
	if (it)
		_lexers.value(cbLexersHighlightingLanguages->currentText() )->setEolFill( b, it->data( Qt::UserRole ).toInt());
}
void SettingsDialog::cbLexersHighlightingProperties_clicked(bool b){
	// get check box
	QCheckBox* cb = qobject_cast<QCheckBox*>(sender());
	if ( !cb )
		return;
	// get lexer
	QsciLexer* l = _lexers.value( cbLexersHighlightingLanguages->currentText() );
	// set lexer properties
	///@todo this is not working properly
	l->setAutoIndentStyle(0);
	/*if ( cb == cbLexersHighlightingIndentOpeningBrace || cb == cbLexersHighlightingIndentClosingBrace ) {
		if ( cbLexersHighlightingIndentOpeningBrace->isChecked() && cbLexersHighlightingIndentClosingBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiOpening | QsciScintilla::AiClosing );
		else if ( cbLexersHighlightingIndentOpeningBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiOpening );
		else if ( cbLexersHighlightingIndentClosingBrace->isChecked() )
			l->setAutoIndentStyle( QsciScintilla::AiClosing );
		else
			l->setAutoIndentStyle( QsciScintilla::AiMaintain );
	}
	else*/
		LexerManager::setLexerProperty(cb->statusTip(), l, b);
}
void SettingsDialog::on_cbLexersHighlightingIndentationWarning_currentIndexChanged(int i){
	// get lexer
	QsciLexer* l = _lexers.value(cbLexersHighlightingLanguages->currentText());
	// set lexer properties
	LexerManager::setLexerProperty(cbLexersHighlightingIndentationWarning->statusTip(), l, cbLexersHighlightingIndentationWarning->itemData(i));
}
void SettingsDialog::on_pbLexersHighlightingReset_clicked(){
	// get lexer
	QsciLexer* l = _lexers.value( cbLexersHighlightingLanguages->currentText() );
	// reset and refresh
	if (l){
		// remove lexer entry
		_settings->remove(QString( "Scintilla/%2" ).arg(l->language()));
		// reset lexer
		LexerManager::resetLexer(l);
		// re read properties
		l->readSettings(*_settings);
		//update gui
		on_cbLexersHighlightingLanguages_currentIndexChanged(l->language());
	}
}

void SettingsDialog::onStyleChanged(){
	AStyleIndenter asi;
	asi.setStyle((astyle::FormatStyle)_bgASStyle->checkedId());
	//add extra option
	//indentation
	switch(_bgASIndent->checkedId()){
		case AS_INDENT_WITH_SPACES:
			asi.formatter().setSpaceIndentation(spbSpacePerTab->value());
			break;
		case AS_INDENT_WITH_TABS:
			asi.formatter().setTabIndentation(spbSpacePerTab->value(), false);
			break;
		case AS_FORCE_INDENT_WITH_TABS:
			asi.formatter().setTabIndentation(spbSpacePerTab->value(), true);
			break;
	}
	//bracket
	asi.formatter().setBracketFormatMode((astyle::BracketMode)_bgASBracket->checkedId());
	//indentation
	asi.formatter().setClassIndent((bool)chkIndentClasses->checkState());
	asi.formatter().setSwitchIndent((bool)chkIndentSwitches->checkState());
	asi.formatter().setCaseIndent((bool)chkIndentCase->checkState());
	asi.formatter().setBracketIndent((bool)chkIndentBrackets->checkState());
	asi.formatter().setBlockIndent((bool)chkIndentBlocks->checkState());
	asi.formatter().setNamespaceIndent((bool)chkIndentNamespaces->checkState());
	asi.formatter().setLabelIndent((bool)chkIndentLabels->checkState());
	asi.formatter().setPreprocessorIndent((bool)chkIndentPreprocessor->checkState());
	asi.formatter().setIndentCol1CommentsMode((bool)chkIndentColumnOneComment->checkState());
	//padding
	if((bool)chkBreakHeaderBlocks->checkState()){
		asi.formatter().setBreakBlocksMode(true);
	}
	if((bool)chkBreakAllBlocks->checkState()){
		asi.formatter().setBreakBlocksMode(true);
		asi.formatter().setBreakClosingHeaderBlocksMode(true);
	}
	asi.formatter().setOperatorPaddingMode((bool)chkPadOperators->checkState());
	asi.formatter().setParensOutsidePaddingMode((bool)chkPadParenthesisOutside->checkState());
	asi.formatter().setParensInsidePaddingMode((bool)chkPadParenthesisInside->checkState());
	asi.formatter().setParensHeaderPaddingMode((bool)chkPadHeader->checkState());
	asi.formatter().setParensUnPaddingMode((bool)chkUnpadParenthesis->checkState());
	asi.formatter().setDeleteEmptyLinesMode((bool)chkDeleteEmptyLines->checkState());
	asi.formatter().setEmptyLineFill((bool)chkFillEmptyLines->checkState());

	asi.formatter().setBreakClosingHeaderBracketsMode((bool)chkBreakClosingBrackets->checkState());
	asi.formatter().setBreakElseIfsMode((bool)chkBreakIfElseStatements->checkState());
	asi.formatter().setAddBracketsMode((bool)chkAddBrackets->checkState());
	asi.formatter().setAddOneLineBracketsMode((bool)chkAddOneLineBrackets->checkState());
	asi.formatter().setBreakOneLineBlocksMode((bool)chkKeepOneLineBlocks->checkState());
	asi.formatter().setSingleStatementsMode((bool)chkKeepOneLineStatements->checkState());
	asi.formatter().setTabSpaceConversionMode((bool)chkCOnvertTabs->checkState());

	if(cbPointerAlign->currentText() == "Attach to type")
		asi.formatter().setPointerAlignment(astyle::ALIGN_TYPE);
	else if(cbPointerAlign->currentText() == "Attach in middle")
		asi.formatter().setPointerAlignment(astyle::ALIGN_MIDDLE);
	else if(cbPointerAlign->currentText() == "Attach to name")
		asi.formatter().setPointerAlignment(astyle::ALIGN_NAME);

	QString text = asi.format(tePreview->text());
	tePreview->clear();
	tePreview->insert(text);
}

void SettingsDialog::on_tbHideLexer_clicked(){
	QList<QListWidgetItem *> items = lvShowLexer->selectedItems();
	foreach(QListWidgetItem* item, items){
		lvHideLexer->addItem(item->text());
		delete item;
	}
}

void SettingsDialog::on_tbShowLexer_clicked(){
	QList<QListWidgetItem *> items = lvHideLexer->selectedItems();
	foreach(QListWidgetItem* item, items){
		lvShowLexer->addItem(item->text());
		delete item;
	}
}
