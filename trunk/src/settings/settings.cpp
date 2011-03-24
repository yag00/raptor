#include <QtGui>
#include <QtDebug>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>

#include "../astyle/AStyleIndenter.h"
#include "../mainwindow.h"
#include "../document_manager.h"
#include "../document_editor.h"
#include "settings.h"

Settings::Settings(QObject * parent_) : QSettings (QSettings::IniFormat, QSettings::UserScope, "raptor", "settings-dev", parent_){

}

Settings::~Settings(){

}

QStringList Settings::availableTextCodecs(){
	static QMap<QString, QString> codecs;

	if(codecs.isEmpty()){
		foreach (const QByteArray& codec, QTextCodec::availableCodecs())
			codecs[ codec.toLower() ] = codec;
	}
	return codecs.values();
}

QStringList Settings::availableLanguages(){
	static QStringList languages;

	if (languages.isEmpty()){
		languages = QStringList() << "Bash" << "Batch" << "C#" << "C++" << "CMake" << "CSS"
			<< "D" << "Diff" << "HTML" << "IDL" << "Java" << "JavaScript" << "Lua" << "Makefile" << "Perl"
			<< "POV" << "Properties" << "Ruby" << "Python" << "SQL" << "TeX" << "VHDL"
			<< "Fortran" << "Fortran77" << "Pascal" << "PostScript" << "TCL" << "XML" << "YAML"
			<< "Verilog" << "Spice";
		languages.sort();
	}
	return languages;
}

QStringList Settings::availableTranslations(){
	static QStringList translations;

	if (translations.isEmpty()){
		translations = QStringList() << "English";
		translations.sort();
	}
	return translations;
}

void Settings::apply(MainWindow& window_){
	// ToolBar
	int iconSize = getToolBarIconSize();
	window_.toolBarFile->setIconSize(QSize(iconSize,iconSize));
	window_.toolBarEdit->setIconSize(QSize(iconSize,iconSize));
	window_.toolBarView->setIconSize(QSize(iconSize,iconSize));
	window_.toolBarMacro->setIconSize(QSize(iconSize,iconSize));

	window_.toolBarFile->setVisible(getShowToolBar("File"));
	window_.toolBarEdit->setVisible(getShowToolBar("Edit"));
	window_.toolBarView->setVisible(getShowToolBar("View"));
	window_.toolBarMacro->setVisible(getShowToolBar("Macro"));
	// StatusBar
	window_.statusbar->setVisible(getShowStatusBar());

	//Lexer association
	LexerManager::getInstance().setAssociationList(getAssociations());
	
	// apply properties to all open documents
	foreach(DocumentEditor* document, window_.getDocumentManager().getDocuments()){
		applyToDocument(document);
	}
}

void Settings::applyToDocument(DocumentEditor* document_){
	QsciLexer* lexer = document_->lexer();
	//remove lexer while applying settings (avoid bug display)
	//document_->setLexer(0);
	if(lexer != 0){
		lexer->readSettings(*this);
		lexer->refreshProperties();
	}

	// General
	if(useSelectionForegroundColor())
		document_->setSelectionForegroundColor(getSelectionForegroundColor());
	else
		document_->setNoSelectionForegroundColor();
	document_->setSelectionBackgroundColor(getSelectionBackgroundColor());

	document_->setColor(getDocumentPen());
	document_->setPaper(getDocumentPaper());
	document_->setFont(getDocumentFont());

	//Encoding
	document_->setDefaultCodec(getDefaultCodec());
	document_->setUnicodeBomUseMode((DocumentEditor::UnicodeBomUseMode)getUnicodeBomUseMode());
	document_->setCharsetAutoDetection(getTryAutoDetectCodec());
	
	// Auto Completion
	document_->setAutoCompletionCaseSensitivity(getAutoCompletionCaseSensitivity());
	document_->setAutoCompletionReplaceWord(getAutoCompletionReplaceWord());
	document_->setAutoCompletionShowSingle(getAutoCompletionShowSingle());
	document_->setAutoCompletionSource(getAutoCompletionSource());
	document_->setAutoCompletionThreshold(getAutoCompletionThreshold());

	// CallTips
	document_->setCallTipsBackgroundColor(getCallTipsBackgroundColor());
	document_->setCallTipsForegroundColor(getCallTipsForegroundColor());
	document_->setCallTipsHighlightColor(getCallTipsHighlightColor());
	document_->setCallTipsStyle(getCallTipsStyle());
	document_->setCallTipsVisible(getCallTipsVisible());

	// Indentation
	document_->setAutoIndent(getAutoIndent());
	document_->setBackspaceUnindents(getBackspaceUnindents());
	document_->setIndentationGuides(getIndentationGuides());
	document_->setIndentationsUseTabs(getIndentationsUseTabs());
	document_->setIndentationWidth(getIndentationWidth());
	document_->setTabIndents(getTabIndents());
	document_->setTabWidth(getTabWidth());
	document_->setIndentationGuidesBackgroundColor(getIndentationGuidesBackgroundColor());
	document_->setIndentationGuidesForegroundColor(getIndentationGuidesForegroundColor());
	document_->setAutoDetectIndent(getAutoDetectIndent());

	// Brace Matching
	document_->setBraceMatching(getBraceMatching());
	document_->setMatchedBraceBackgroundColor(getMatchedBraceBackgroundColor());
	document_->setMatchedBraceForegroundColor(getMatchedBraceForegroundColor());
	document_->setUnmatchedBraceBackgroundColor(getUnmatchedBraceBackgroundColor());
	document_->setUnmatchedBraceForegroundColor(getUnmatchedBraceForegroundColor());

	// Edge Mode
	document_->setEdgeMode(getEdgeMode());
	document_->setEdgeColor(getEdgeColor());
	document_->setEdgeColumn(getEdgeColumn());

	// Caret
	document_->setCaretLineVisible(getCaretLineVisible());
	document_->setCaretLineBackgroundColor(getCaretLineBackgroundColor());
	document_->setCaretForegroundColor(getCaretForegroundColor());
	document_->setCaretWidth(getCaretWidth());

	// Margins
	if (getMarginEnabled()){
		document_->setMarginsBackgroundColor(getMarginBackgroundColor());
		document_->setMarginsForegroundColor(getMarginForegroundColor());
		document_->setMarginsFont(getMarginFont());
		document_->setLineNumbersMarginEnabled(true);
	}
	else{
		document_->setLineNumbersMarginEnabled(false);
	}

	if (getMarginBookmarkEnabled()){
		document_->setMarginSensitivity(1, true);
		document_->setMarginWidth(1, 16);
	}else{
		document_->setMarginSensitivity(1, false);
		document_->setMarginWidth(1, 0);
	}
	
	if(getFoldMarginEnabled()){
		document_->setFolding((ScintillaExt::FoldStyleExt)getFolding(), getFoldSymbolForegroundColor(), getFoldSymbolBackgroundColor());
		document_->setFoldMarginColors(getFoldMarginForegroundColor(), getFoldMarginBackgroundColor());
	}else{
		document_->setFolding(ScintillaExt::NoFoldStyle, getFoldSymbolForegroundColor(), getFoldSymbolBackgroundColor());
	}

	// Special Characters
	document_->setEolMode(getEolMode());
	document_->setAutoDetectEol(getAutoDetectEol());

	document_->setEolVisibility(getEolVisibility());

	if (getWhitespaceVisibility() != QsciScintilla::WsInvisible)
		document_->setWhitespaceVisibility(getWhitespaceVisibility());
	else
		document_->setWhitespaceVisibility(QsciScintilla::WsInvisible);

	if (getWrapMode() != QsciScintilla::WrapNone)
		document_->setWrapMode(getWrapMode());
	else
		document_->setWrapMode(QsciScintilla::WrapNone);

	if (getWrapVisualFlagsEnabled())
		document_->setWrapVisualFlags(getEndWrapVisualFlag(), getStartWrapVisualFlag(), getWrappedLineIndentWidth());
	else
		document_->setWrapVisualFlags(QsciScintilla::WrapFlagNone, QsciScintilla::WrapFlagNone, getWrappedLineIndentWidth());

	//set the lexer in order to document to care of the new lexer properties if any
	//document_->setLexer(lexer);
}

/**** SETTINGS GETTER/SETTER ****/

// Interface
void Settings::setToolBarIconSize(int size_){
	setValue("Interface/toolbar/iconSize", size_);
}
int Settings::getToolBarIconSize(){
	return value("Interface/toolbar/iconSize", 22).toInt();
}

void Settings::setShowToolBar(const QString& toolbar_, bool show_){
	setValue("Interface/toolbar/show" + toolbar_, show_);
}
bool Settings::getShowToolBar(const QString& toolbar_){
	return value("Interface/toolbar/show" + toolbar_, true).toBool();
}

void Settings::setShowStatusBar(bool show_){
	setValue("Interface/statusbar/show", show_);
}
bool Settings::getShowStatusBar(){
	return value("Interface/statusbar/show", true).toBool();
}


/******	EDITOR ******/

void Settings::setRecentFiles(const QStringList& files_){
	setValue("RecentFiles/files", files_);
}
QStringList Settings::getRecentFiles(){
	return value("RecentFiles/files").toStringList();
}

// General
// void setAutoSyntaxCheck( bool check );
// bool autoSyntaxCheck();
// void setConvertTabsUponOpen( bool convert );
// bool convertTabsUponOpen();
// void setCreateBackupUponOpen( bool backup );
// bool createBackupUponOpen();
// void setAutoEolConversion( bool convert );
// bool autoEolConversion();

void Settings::useSelectionForegroundColor(bool useForegroundColor_){
	setValue("Editor/useForeGroundColor", useForegroundColor_);
}
bool Settings::useSelectionForegroundColor(){
	return value("Editor/useForeGroundColor", false).toBool();
}

void Settings::setSelectionBackgroundColor(const QColor& color_){
	setValue("Editor/SelectionBackgroundColor", color_);
}
QColor Settings::getSelectionBackgroundColor(){
	return value("Editor/SelectionBackgroundColor", QColor("#C0C0C0")).value<QColor>();
}

void Settings::setSelectionForegroundColor(const QColor& color_){
	setValue("Editor/SelectionForegroundColor", color_);
}
QColor Settings::getSelectionForegroundColor(){
	return value("Editor/SelectionForegroundColor", QColor("#000000")).value<QColor>();
}

void Settings::setDocumentPen(const QColor& color_){
	setValue("Editor/penColor", color_);
}
QColor Settings::getDocumentPen(){
	return value("Editor/penColor", QColor(Qt::black)).value<QColor>();
}

void Settings::setDocumentPaper(const QColor& color_){
	setValue("Editor/paperColor", color_);
}
QColor Settings::getDocumentPaper(){
	return value("Editor/paperColor", QColor(Qt::white)).value<QColor>();
}

void Settings::setDocumentFont(const QFont& font_){
	setValue("Editor/font", font_.toString());
}
QFont Settings::getDocumentFont(){
	QFont f;
#if defined(Q_OS_WIN)
	f = QFont("Courier New",10);
#else
	f = QFont("Bitstream Vera Sans Mono",9);
#endif
	f.fromString(value("Editor/font", f.toString()).toString());
	return f;
}

//Encoding
void Settings::setDefaultCodec(const QString& codec){
	setValue("Encoding/DefaultCodec", codec);
}
QString Settings::getDefaultCodec(){
	return value("Encoding/DefaultCodec", "UTF-8").toString();
}

void Settings::setUnicodeBomUseMode(int bomMode_){
	setValue("Encoding/BOM", bomMode_);
}
int Settings::getUnicodeBomUseMode(){
	return value("Encoding/BOM", 0).toInt();
}

void Settings::setTryAutoDetectCodec(bool auto_){
	setValue("Encoding/autoDetect", auto_);
}
bool Settings::getTryAutoDetectCodec(){
	return value("Encoding/autoDetect", true).toBool();
}

// Auto Completion
void Settings::setAutoCompletionCaseSensitivity(bool caseSensitive_){
	setValue("AutoCompletion/caseSensitivity", caseSensitive_);
}
bool Settings::getAutoCompletionCaseSensitivity(){
	return value("AutoCompletion/caseSensitivity", true).toBool();
}

void Settings::setAutoCompletionReplaceWord(bool replace_){
	setValue("AutoCompletion/replaceWord", replace_);
}
bool Settings::getAutoCompletionReplaceWord(){
	return value("AutoCompletion/replaceWord", false).toBool();
}

void Settings::setAutoCompletionShowSingle(bool showSingle_){
	setValue("AutoCompletion/showSingle", showSingle_);
}
bool Settings::getAutoCompletionShowSingle(){
	return value("AutoCompletion/showSingle", true).toBool();
}

void Settings::setAutoCompletionSource(QsciScintilla::AutoCompletionSource source_){
	setValue("AutoCompletion/source", (int)source_);
}
QsciScintilla::AutoCompletionSource Settings::getAutoCompletionSource(){
	return (QsciScintilla::AutoCompletionSource)value("AutoCompletion/source", QsciScintilla::AcsAll).toInt();
}

void Settings::setAutoCompletionThreshold(int count_){
	setValue("AutoCompletion/threshold", count_);
}
int Settings::getAutoCompletionThreshold(){
	return value("AutoCompletion/threshold", 3).toInt();
}

// CallTips
void Settings::setCallTipsBackgroundColor(const QColor& color_){
	setValue("CallTips/CallTipsBackgroundColor", color_);
}
QColor Settings::getCallTipsBackgroundColor(){
	return value("CallTips/CallTipsBackgroundColor", QColor("#ffff9b")).value<QColor>();
}

void Settings::setCallTipsForegroundColor(const QColor& color_){
	setValue("CallTips/CallTipsForegroundColor", color_);
}
QColor Settings::getCallTipsForegroundColor(){
	return value("CallTips/CallTipsForegroundColor", QColor("#000000")).value<QColor>();
}

void Settings::setCallTipsHighlightColor(const QColor& color_){
	setValue("CallTips/CallTipsHighlightColor", color_);
}
QColor Settings::getCallTipsHighlightColor(){
	return value("CallTips/CallTipsHighlightColor", QColor("#ff0000")).value<QColor>();
}

void Settings::setCallTipsStyle(QsciScintilla::CallTipsStyle style_){
	setValue("CallTips/CallTipsStyle", style_);
}
QsciScintilla::CallTipsStyle Settings::getCallTipsStyle(){
	return (QsciScintilla::CallTipsStyle)value("CallTips/CallTipsStyle", QsciScintilla::CallTipsContext).toInt();
}

void Settings::setCallTipsVisible(int count_){
	setValue("CallTips/CallTipsVisible", count_);
}
int Settings::getCallTipsVisible(){
	return value("CallTips/CallTipsVisible", -1).toInt();
}

// Indentation
void Settings::setAutoIndent(bool indent_){
	setValue("Indentation/AutoIndent", indent_);
}
bool Settings::getAutoIndent(){
	return value("Indentation/AutoIndent", true).toBool();
}

void Settings::setBackspaceUnindents(bool unindents_){
	setValue("Indentation/BackspaceUnindents", unindents_);
}
bool Settings::getBackspaceUnindents(){
	return value("Indentation/BackspaceUnindents", true).toBool();
}

void Settings::setIndentationGuides(bool guides_){
	setValue("Indentation/IndentationGuides", guides_);
}
bool Settings::getIndentationGuides(){
	return value("Indentation/IndentationGuides", true).toBool();
}

void Settings::setIndentationsUseTabs(bool tabs_){
	setValue("Indentation/IndentationsUseTabs", tabs_);
}
bool Settings::getIndentationsUseTabs(){
	return value("Indentation/IndentationsUseTabs", true).toBool();
}

void Settings::setAutoDetectIndent(bool detect_){
	setValue("Indentation/AutoDetectIndent", detect_);
}
bool Settings::getAutoDetectIndent(){
	return value("Indentation/AutoDetectIndent", true).toBool();
}

void Settings::setIndentationWidth(int width_){
	setValue("Indentation/IndentationWidth", width_);
}
int Settings::getIndentationWidth(){
	return value("Indentation/IndentationWidth", 4).toInt();
}

void Settings::setTabIndents(bool indents_){
	setValue("Indentation/TabIndents", indents_);
}
bool Settings::getTabIndents(){
	return value("Indentation/TabIndents", true).toBool();
}

void Settings::setTabWidth(int width_){
	setValue("Indentation/TabWidth", width_);
}
int Settings::getTabWidth(){
	return value("Indentation/TabWidth", 4).toInt();
}

void Settings::setIndentationGuidesBackgroundColor(const QColor& color_){
	setValue("Indentation/IndentationGuidesBackgroundColor", color_);
}
QColor Settings::getIndentationGuidesBackgroundColor(){
	return value("Indentation/IndentationGuidesBackgroundColor", QColor("#ffffff")).value<QColor>();
}

void Settings::setIndentationGuidesForegroundColor(const QColor& color_){
	setValue("Indentation/IndentationGuidesForegroundColor", color_);
}
QColor Settings::getIndentationGuidesForegroundColor(){
	return value("Indentation/IndentationGuidesForegroundColor", QColor("#c0c0c0")).value<QColor>();
}

// Brace Matching
void Settings::setBraceMatching(QsciScintilla::BraceMatch mode_){
	setValue("BraceMatching/BraceMatching", mode_);
}
QsciScintilla::BraceMatch Settings::getBraceMatching(){
	return (QsciScintilla::BraceMatch)value("BraceMatching/BraceMatching", QsciScintilla::SloppyBraceMatch).toInt();
}

void Settings::setMatchedBraceBackgroundColor(const QColor& color_){
	setValue("BraceMatching/MatchedBraceBackgroundColor", color_);
}
QColor Settings::getMatchedBraceBackgroundColor(){
	return value("BraceMatching/MatchedBraceBackgroundColor", QColor("#ffff80")).value<QColor>();
}

void Settings::setMatchedBraceForegroundColor(const QColor& color_){
	setValue("BraceMatching/MatchedBraceForegroundColor", color_);
}
QColor Settings::getMatchedBraceForegroundColor(){
	return value("BraceMatching/MatchedBraceForegroundColor", QColor("#ff0000")).value<QColor>();
}

void Settings::setUnmatchedBraceBackgroundColor(const QColor& color_){
	setValue("BraceMatching/UnmatchedBraceBackgroundColor", color_);
}
QColor Settings::getUnmatchedBraceBackgroundColor(){
	return value("BraceMatching/UnmatchedBraceBackgroundColor", QColor("#ffffff")).value<QColor>();
}

void Settings::setUnmatchedBraceForegroundColor(const QColor& color_){
	setValue("BraceMatching/UnmatchedBraceForegroundColor", color_);
}
QColor Settings::getUnmatchedBraceForegroundColor(){
	return value("BraceMatching/UnmatchedBraceForegroundColor", QColor("#800000")).value<QColor>();
}

// Edge Mode
void Settings::setEdgeMode(QsciScintilla::EdgeMode mode_){
	setValue("Edge/EdgeMode", mode_);
}
QsciScintilla::EdgeMode Settings::getEdgeMode(){
	return (QsciScintilla::EdgeMode)value("Edge/EdgeMode", QsciScintilla::EdgeNone).toInt();
}

void Settings::setEdgeColor(const QColor& color_){
	setValue("Edge/EdgeColor", color_);
}
QColor Settings::getEdgeColor(){
	return value("Edge/EdgeColor", QColor("#80FFFF")).value<QColor>();
}

void Settings::setEdgeColumn(int columns_){
	setValue("Edge/EdgeColumn", columns_);
}
int Settings::getEdgeColumn(){
	return value("Edge/EdgeColumn", 80).toInt();
}

// Caret
void Settings::setCaretLineVisible(bool visible_){
	setValue("Caret/CaretLineVisible", visible_);
}
bool Settings::getCaretLineVisible(){
	return value("Caret/CaretLineVisible", true).toBool();
}

void Settings::setCaretLineBackgroundColor(const QColor& color_){
	setValue("Caret/CaretLineBackgroundColor", color_);
}
QColor Settings::getCaretLineBackgroundColor(){
	return value("Caret/CaretLineBackgroundColor", QColor("#E8E8FF")).value<QColor>();
}

void Settings::setCaretForegroundColor( const QColor& color_){
	setValue("Caret/CaretForegroundColor", color_);
}
QColor Settings::getCaretForegroundColor(){
	return value("Caret/CaretForegroundColor", QColor("#000000")).value<QColor>();
}

void Settings::setCaretWidth(int width_){
	setValue("Caret/CaretWidth", width_);
}
int Settings::getCaretWidth(){
	return value("Caret/CaretWidth", 1).toInt();
}

// Margins
void Settings::setFoldMarginEnabled(bool enabled_){
	setValue("Margins/FoldMarginsEnabled", enabled_);
}
bool Settings::getFoldMarginEnabled(){
	return value("Margins/FoldMarginsEnabled", true).toBool();
}

void Settings::setFolding(int style_){
	setValue("Margins/Folding", style_);
}
int Settings::getFolding(){
	return value("Margins/Folding", QsciScintilla::BoxedTreeFoldStyle).toInt();
}

void Settings::setFoldMarginBackgroundColor(const QColor& color_){
	setValue("Margins/FoldMarginBackgroundColor", color_);
}
QColor Settings::getFoldMarginBackgroundColor(){
	return value("Margins/FoldMarginBackgroundColor", QColor("#e9e9e9")).value<QColor>();
}

void Settings::setFoldMarginForegroundColor(const QColor& color_){
	setValue("Margins/FoldMarginForegroundColor", color_);
}
QColor Settings::getFoldMarginForegroundColor(){
	return value("Margins/FoldMarginForegroundColor", QColor("#ffffff")).value<QColor>();
}

void Settings::setFoldSymbolBackgroundColor(const QColor& color_){
	setValue("Margins/FoldSymbolBackgroundColor", color_);
}
QColor Settings::getFoldSymbolBackgroundColor(){
	return value("Margins/FoldSymbolBackgroundColor", QColor("#808080")).value<QColor>();
}

void Settings::setFoldSymbolForegroundColor(const QColor& color_){
	setValue("Margins/FoldSymbolForegroundColor", color_);
}
QColor Settings::getFoldSymbolForegroundColor(){
	return value("Margins/FoldSymbolForegroundColor", QColor("#f3f3f3")).value<QColor>();
}

void Settings::setMarginEnabled(bool enabled_){
	setValue("Margins/MarginsEnabled", enabled_);
}
bool Settings::getMarginEnabled(){
	return value("Margins/MarginsEnabled", true).toBool();
}

void Settings::setMarginBookmarkEnabled(bool enabled_){
	setValue("Margins/MarginsBookmarkEnabled", enabled_);
}
bool Settings::getMarginBookmarkEnabled(){
	return value("Margins/MarginsBookmarkEnabled", true).toBool();
}

void Settings::setMarginBackgroundColor(const QColor& color_){
	setValue("Margins/MarginsBackgroundColor", color_);
}
QColor Settings::getMarginBackgroundColor(){
	return value("Margins/MarginsBackgroundColor", QColor("#e4e4e4")).value<QColor>();
}

void Settings::setMarginForegroundColor(const QColor& color_){
	setValue("Margins/MarginsForegroundColor", color_);
}
QColor Settings::getMarginForegroundColor(){
	return value("Margins/MarginsForegroundColor", QColor("#808080")).value<QColor>();
}

void Settings::setMarginFont(const QFont& font_){
	setValue("Margins/MarginsFont", font_.toString());
}
QFont Settings::getMarginFont(){
	QFont f;
	f.fromString(value("Margins/MarginsFont", f.toString()).toString());
	return f;
}

// Special Characters
void Settings::setEolMode(QsciScintilla::EolMode mode_){
	setValue("SpecialCharacters/EolMode", mode_);
}
QsciScintilla::EolMode Settings::getEolMode(){
#if defined( Q_WS_WIN )
	int i = QsciScintilla::EolWindows;
#elif defined( Q_WS_MAC )
	int i = QsciScintilla::EolMac;
#else
	int i = QsciScintilla::EolUnix;
#endif
	return (QsciScintilla::EolMode)value("SpecialCharacters/EolMode", i).toInt();
}

void Settings::setEolVisibility(bool visible_){
	setValue("SpecialCharacters/EolVisibility", visible_);
}
bool Settings::getEolVisibility(){
	return value("SpecialCharacters/EolVisibility", false).toBool();
}

void Settings::setAutoDetectEol(bool detect_){
	setValue("SpecialCharacters/AutoDetectEol", detect_);
}
bool Settings::getAutoDetectEol(){
	return value("SpecialCharacters/AutoDetectEol", true).toBool();
}

void Settings::setWhitespaceVisibility(QsciScintilla::WhitespaceVisibility visibility_){
	setValue("SpecialCharacters/WhitespaceVisibility", visibility_);
}
QsciScintilla::WhitespaceVisibility Settings::getWhitespaceVisibility(){
	return (QsciScintilla::WhitespaceVisibility)value("SpecialCharacters/WhitespaceVisibility", QsciScintilla::WsInvisible).toInt();
}

void Settings::setWrapMode(QsciScintilla::WrapMode mode_){
	setValue("SpecialCharacters/WrapMode", mode_);
}
QsciScintilla::WrapMode Settings::getWrapMode(){
	return (QsciScintilla::WrapMode)value("SpecialCharacters/WrapMode", QsciScintilla::WrapNone).toInt();
}

void Settings::setWrapVisualFlagsEnabled(bool enabled_){
	setValue("SpecialCharacters/WrapVisualFlagsEnabled", enabled_);
}
bool Settings::getWrapVisualFlagsEnabled(){
	return value("SpecialCharacters/WrapVisualFlagsEnabled", false ).toBool();
}

void Settings::setStartWrapVisualFlag(QsciScintilla::WrapVisualFlag flag_){
	setValue("SpecialCharacters/StartWrapVisualFlag", flag_);
}
QsciScintilla::WrapVisualFlag Settings::getStartWrapVisualFlag(){
	return (QsciScintilla::WrapVisualFlag)value("SpecialCharacters/StartWrapVisualFlag", QsciScintilla::WrapFlagNone).toInt();
}

void Settings::setEndWrapVisualFlag(QsciScintilla::WrapVisualFlag flag_){
	setValue("SpecialCharacters/EndWrapVisualFlag", flag_);
}
QsciScintilla::WrapVisualFlag Settings::getEndWrapVisualFlag(){
	return (QsciScintilla::WrapVisualFlag)value("SpecialCharacters/EndWrapVisualFlag", QsciScintilla::WrapFlagNone).toInt();
}

void Settings::setWrappedLineIndentWidth(int witdh_){
	setValue("SpecialCharacters/WrappedLineIndentWidth", witdh_);
}
int Settings::getWrappedLineIndentWidth(){
	return value("SpecialCharacters/WrappedLineIndentWidth", 0).toInt();
}

// Lexer association
QMap<QString, QStringList> Settings::getDefaultLexerAssociations(){
	QMap<QString, QStringList> lexerAssociation;
	//build default map association
	QStringList filePatternList = QStringList();
	//Bash
	filePatternList.clear();
	filePatternList << "*.sh";
	lexerAssociation.insert("Bash", filePatternList);
	//Batch
	filePatternList.clear();
	filePatternList << "*.cmd" << "*.bat";
	lexerAssociation.insert("Batch", filePatternList);
	//C#
	filePatternList.clear();
	filePatternList << "*.cmd" << "*.cs";
	lexerAssociation.insert("C#", filePatternList);
	//C++
	filePatternList.clear();
	filePatternList << "*.hxx" << "*.hpp" << "*.hh" << "*.h++" << "*.h" << "*.cxx" << "*.cpp" << "*.cc" << "*.c++" << "*.c";
	lexerAssociation.insert("C++", filePatternList);
	//CMake
	filePatternList.clear();
	filePatternList << "CMake" << "CMake.txt" << "*.cmake";
	lexerAssociation.insert("CMake", filePatternList);
	//CSS
	filePatternList.clear();
	filePatternList << "CSS" << "*.css";
	lexerAssociation.insert("CSS", filePatternList);
	//D
	filePatternList.clear();
	filePatternList << "*.d";
	lexerAssociation.insert("D", filePatternList);
	//Diff
	filePatternList.clear();
	filePatternList << "*.patch" << "*.diff";
	lexerAssociation.insert("Diff", filePatternList);
	//HTML
	filePatternList.clear();
	filePatternList << "*.shtm*" << "*.rdf" << "*.php*" << "*.phtm*" << "*.htm*" << "*.dtd" << "*.docbook" << "*.asp";
	lexerAssociation.insert("HTML", filePatternList);
	//IDL
	filePatternList.clear();
	filePatternList << "*.idl";
	lexerAssociation.insert("IDL", filePatternList);
	//Java
	filePatternList.clear();
	filePatternList << "*.java";
	lexerAssociation.insert("Java", filePatternList);
	//JavaScript
	filePatternList.clear();
	filePatternList << "*.js";
	lexerAssociation.insert("JavaScript", filePatternList);
	//Lua
	filePatternList.clear();
	filePatternList << "*.lua";
	lexerAssociation.insert("Lua", filePatternList);
	//Makefile
	filePatternList.clear();
	filePatternList << "Makefile*" << "*makefile" << "*.mak";
	lexerAssociation.insert("Makefile", filePatternList);
	//POV
	filePatternList.clear();
	filePatternList << "*.pov";
	lexerAssociation.insert("POV", filePatternList);
	//Perl
	filePatternList.clear();
	filePatternList << "*.pm" << "*.pl" << "*.ph";
	lexerAssociation.insert("Perl", filePatternList);
	//Properties
	filePatternList.clear();
	filePatternList << "*.reg" << "*.rc" << "*.properties" << "*.ini" << "*.inf" << "*.cnf" << "*.cfg";
	lexerAssociation.insert("Properties", filePatternList);
	//Python
	filePatternList.clear();
	filePatternList << "*.pyx" << "*.pyw" << "*.py" << "*.ptl";
	lexerAssociation.insert("Python", filePatternList);
	//Ruby
	filePatternList.clear();
	filePatternList << "*.rbw" << "*.rb";
	lexerAssociation.insert("Ruby", filePatternList);
	//SQL
	filePatternList.clear();
	filePatternList << "*.sql";
	lexerAssociation.insert("SQL", filePatternList);
	//TCL
	filePatternList.clear();
	filePatternList << "*.tcl";
	lexerAssociation.insert("TCL", filePatternList);
	//TeX
	filePatternList.clear();
	filePatternList << "*.toc" << "*.sty" << "*.idx" << "*.aux";
	lexerAssociation.insert("TeX", filePatternList);
	//VHDL
	filePatternList.clear();
	filePatternList << "*.vhdl";
	lexerAssociation.insert("VHDL", filePatternList);
	//XML
	filePatternList.clear();
	filePatternList << "*.xslt" << "*.xsl" << "*.xsd" << "*.xml" << "*.svg";
	lexerAssociation.insert("XML", filePatternList);
	return lexerAssociation;
}
void Settings::setAssociations(const QMap<QString, QStringList>& associations_){
	setValue("LexerAssociation/NbLexer", associations_.size());

	int i = 0;
	QMap<QString, QStringList>::const_iterator it;
	for( it = associations_.begin(); it != associations_.end(); it++){
		setValue(QString("LexerAssociation/Lexer%1").arg(i), it.key());
		setValue("LexerAssociation/" + it.key(), it.value());
		i++;
	}
}
QMap<QString, QStringList> Settings::getAssociations(){
	QMap<QString, QStringList> assocations;
	int nbLexer = value("LexerAssociation/NbLexer", 0).toInt();

	if(nbLexer == 0)
		return getDefaultLexerAssociations();

	for(int i = 0; i < nbLexer; i++){
		QString lexer = value(QString("LexerAssociation/Lexer%1").arg(i)).toString();
		QStringList filePattern = value("LexerAssociation/" + lexer).toStringList();
		assocations.insert(lexer, filePattern);
	}
	return assocations;
}

void Settings::setAstyleIndenterOptions(AStyleIndenter& asi_){	
	beginGroup("AStyle");
	asi_.setStyle((astyle::FormatStyle)value("style", astyle::STYLE_NONE).toInt());
	//add extra option	
	//indentation
	int space = value("indentation_spt", 4 ).toInt();
	switch(value("indentation_type", AS_FORCE_INDENT_WITH_TABS).toInt()){
		case AS_INDENT_WITH_SPACES:
			asi_.formatter().setSpaceIndentation(space);
			break;
		case AS_INDENT_WITH_TABS:
			asi_.formatter().setTabIndentation(space, false);
			break;
		case AS_FORCE_INDENT_WITH_TABS:
			asi_.formatter().setTabIndentation(space, true);
			break;		
	}
	//bracket
	asi_.formatter().setBracketFormatMode((astyle::BracketMode)value("bracket", astyle::NONE_MODE).toInt());
	//indentation
	asi_.formatter().setClassIndent(value("indent_classes", false).toBool());
	asi_.formatter().setSwitchIndent(value("indent_switches", false).toBool());
	asi_.formatter().setCaseIndent(value("indent_case", false).toBool());
	asi_.formatter().setBracketIndent(value("indent_brackets", false).toBool());
	asi_.formatter().setBlockIndent(value("indent_blocks", false).toBool());
	asi_.formatter().setNamespaceIndent(value("indent_namespaces", false).toBool());
	asi_.formatter().setLabelIndent(value("indent_labels", false).toBool());
	asi_.formatter().setPreprocessorIndent(value("indent_preprocessor", false).toBool());
	asi_.formatter().setIndentCol1CommentsMode(value("indent_column1comment", false).toBool());
	//padding
	if(value("break_header_block", false).toBool()){
		asi_.formatter().setBreakBlocksMode(true);
	}
	if(value("break_all_block", false).toBool()){
		asi_.formatter().setBreakBlocksMode(true);
		asi_.formatter().setBreakClosingHeaderBlocksMode(true);
	}
	
	asi_.formatter().setOperatorPaddingMode(value("pad_operators", false).toBool());
	asi_.formatter().setParensOutsidePaddingMode(value("pad_parenthesis_outside", false).toBool());
	asi_.formatter().setParensInsidePaddingMode(value("pad_parenthesis_inside", false).toBool());
	asi_.formatter().setParensHeaderPaddingMode(value("pad_header", false).toBool());
	asi_.formatter().setParensUnPaddingMode(value("unpad_parenthesis", false).toBool());
	asi_.formatter().setDeleteEmptyLinesMode(value("delete_empty_lines", false).toBool());
	asi_.formatter().setEmptyLineFill(value("fill_empty_lines", false).toBool());
	
	asi_.formatter().setBreakClosingHeaderBracketsMode(value("break_closing_brackets", false).toBool());
	asi_.formatter().setBreakElseIfsMode(value("break_elseifs", false).toBool());
	asi_.formatter().setAddBracketsMode(value("add_brackets", false).toBool());
	asi_.formatter().setAddOneLineBracketsMode(value("add_one_line_brackets", false).toBool());
	asi_.formatter().setBreakOneLineBlocksMode(value("keep_one_line_blocks", false).toBool());
	asi_.formatter().setSingleStatementsMode(value("keep_one_line_statements", false).toBool());
	asi_.formatter().setTabSpaceConversionMode(value("convert_tabs", false).toBool());
	
	if(value("pointer_align", 0).toInt() == 1) //"Attach to type"
		asi_.formatter().setPointerAlignment(astyle::ALIGN_TYPE);
	else if(value("pointer_align", 0).toInt() == 2) //"Attach in middle"
		asi_.formatter().setPointerAlignment(astyle::ALIGN_MIDDLE);
	else if(value("pointer_align", 0).toInt() == 3) //"Attach to name"
		asi_.formatter().setPointerAlignment(astyle::ALIGN_NAME);
		
	endGroup();
}
