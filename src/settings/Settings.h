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

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <QLocale>
#include <QSettings>
#include <Qsci/qsciscintilla.h>
#include "../about/Version.h"
#include "../lexer/lexer_manager.h"

class MainWindow;
class DocumentEditor;
class AStyleIndenter;

/** handle all settings of the application */
class Settings : public QSettings{

	Q_OBJECT

	public:
		Settings(QObject* parent_ = 0);
		~Settings();

		/** @return list of available codec */
		static QStringList availableTextCodecs();

		/** @return list of all know languages */
		static QStringList availableLanguages();

		/** @return list of available translation */
		static QList<QLocale::Language> availableTranslations();

		/** apply all settings
		* @param window_ reference on MainWindow */
		void apply(MainWindow& window_);

		/** apply settings to a document
		 * @param document_ */
		void applyToDocument(DocumentEditor* document_);

		/** @return raptor settings version */
		Version getVersion();
		/** @param version_ settings version */
		void setVersion(const Version& version_);
		
		QStringList getPluginPaths();
		void setPluginPaths(const QStringList& paths_);
		
		// Interface
		void setToolBarIconSize(int size_);
		int getToolBarIconSize();

		void setShowToolBar(const QString& toolbar_, bool show_);
		bool getShowToolBar(const QString& toolbar_, bool default_ = true);

		void setShowStatusBar(bool show_);
		bool getShowStatusBar();


		/******	EDITOR ******/
		void setRecentFiles(const QStringList& files_);
		QStringList getRecentFiles();

		// General
		/** get/set auto trim on save property */
		void setAutoTrimOnSave(bool autoTrim_);
		bool autoTrimOnSave();
		/** get/set add new line on save property */
		void setAddNewLineOnSave(bool addNewLine_);
		bool addNewLineOnSave();

		/** get/set translation */
		void setTranslation(const QLocale::Language& language_);
		QLocale::Language getTranslation();
		
		// void setAutoSyntaxCheck( bool check );
		// bool autoSyntaxCheck();
		// void setConvertTabsUponOpen( bool convert );
		// bool convertTabsUponOpen();
		// void setCreateBackupUponOpen( bool backup );
		// bool createBackupUponOpen();
		// void setAutoEolConversion( bool convert );
		// bool autoEolConversion();

		/** get/set use custom foreground selection color (if true foreground selection color will be used */
		void useSelectionForegroundColor(bool useForegroundColor_);
		bool useSelectionForegroundColor();
		/** get/set selection background color */
		void setSelectionBackgroundColor(const QColor& color_);
		QColor getSelectionBackgroundColor();
		/** get/set selection foreground color */
		void setSelectionForegroundColor(const QColor& color_);
		QColor getSelectionForegroundColor();
		/** get/set pen color */
		void setDocumentPen(const QColor& color_);
		QColor getDocumentPen();
		/** get/set paper color */
		void setDocumentPaper(const QColor& color_);
		QColor getDocumentPaper();
		/** get/set document font */
		void setDocumentFont(const QFont& font_);
		QFont getDocumentFont();

		//Encoding
		/** get/set default codec */
		void setDefaultCodec(const QString& codec);
		QString getDefaultCodec();

		void setUnicodeBomUseMode(int bomMode_);
		int getUnicodeBomUseMode();

		void setTryAutoDetectCodec(bool auto_);
		bool getTryAutoDetectCodec();

		// Auto Completion
		void setAutoCompletionCaseSensitivity(bool caseSensitive_);
		bool getAutoCompletionCaseSensitivity();

		void setAutoCompletionReplaceWord(bool replace_);
		bool getAutoCompletionReplaceWord();

		void setAutoCompletionShowSingle(bool showSingle_);
		bool getAutoCompletionShowSingle();

		void setAutoCompletionSource(QsciScintilla::AutoCompletionSource source_);
		QsciScintilla::AutoCompletionSource getAutoCompletionSource();

		void setAutoCompletionThreshold(int count_);
		int getAutoCompletionThreshold();

		// CallTips
		void setCallTipsBackgroundColor(const QColor& color_);
		QColor getCallTipsBackgroundColor();

		void setCallTipsForegroundColor(const QColor& color_);
		QColor getCallTipsForegroundColor();

		void setCallTipsHighlightColor(const QColor& color_);
		QColor getCallTipsHighlightColor();

		void setCallTipsStyle(QsciScintilla::CallTipsStyle style_);
		QsciScintilla::CallTipsStyle getCallTipsStyle();

		void setCallTipsVisible(int count_);
		int getCallTipsVisible();

		// Indentation
		void setAutoIndent(bool indent_);
		bool getAutoIndent();

		void setBackspaceUnindents(bool unindents_);
		bool getBackspaceUnindents();

		void setIndentationGuides(bool guides_);
		bool getIndentationGuides();

		void setIndentationsUseTabs(bool tabs_);
		bool getIndentationsUseTabs();

		void setAutoDetectIndent(bool detect_);
		bool getAutoDetectIndent();

		void setIndentationWidth(int width_);
		int getIndentationWidth();

		void setTabIndents(bool indents_);
		bool getTabIndents();

		void setTabWidth(int width_);
		int getTabWidth();

		void setIndentationGuidesBackgroundColor(const QColor& color_);
		QColor getIndentationGuidesBackgroundColor();

		void setIndentationGuidesForegroundColor(const QColor& color_);
		QColor getIndentationGuidesForegroundColor();

		// Brace Matching
		void setBraceMatching(QsciScintilla::BraceMatch mode_);
		QsciScintilla::BraceMatch getBraceMatching();

		void setMatchedBraceBackgroundColor(const QColor& color_);
		QColor getMatchedBraceBackgroundColor();

		void setMatchedBraceForegroundColor(const QColor& color_);
		QColor getMatchedBraceForegroundColor();

		void setUnmatchedBraceBackgroundColor(const QColor& color_);
		QColor getUnmatchedBraceBackgroundColor();

		void setUnmatchedBraceForegroundColor(const QColor& color_);
		QColor getUnmatchedBraceForegroundColor();

		// Edge Mode
		void setEdgeMode(QsciScintilla::EdgeMode mode_);
		QsciScintilla::EdgeMode getEdgeMode();

		void setEdgeColor(const QColor& color_);
		QColor getEdgeColor();

		void setEdgeColumn(int columns_);
		int getEdgeColumn();

		// Caret
		void setCaretLineVisible(bool visible_);
		bool getCaretLineVisible();

		void setCaretLineBackgroundColor(const QColor& color_);
		QColor getCaretLineBackgroundColor();

		void setCaretForegroundColor( const QColor& color_);
		QColor getCaretForegroundColor();

		void setCaretWidth(int width_);
		int getCaretWidth();

		// Margins
		void setFoldMarginEnabled(bool enabled_);
		bool getFoldMarginEnabled();

		void setFolding(int style_);
		int getFolding();

		void setFoldMarginBackgroundColor(const QColor& color_);
		QColor getFoldMarginBackgroundColor();

		void setFoldMarginForegroundColor(const QColor& color_);
		QColor getFoldMarginForegroundColor();

		void setFoldSymbolBackgroundColor(const QColor& color_);
		QColor getFoldSymbolBackgroundColor();

		void setFoldSymbolForegroundColor(const QColor& color_);
		QColor getFoldSymbolForegroundColor();

		void setMarginEnabled(bool enabled_);
		bool getMarginEnabled();

		void setMarginBookmarkEnabled(bool enabled_);
		bool getMarginBookmarkEnabled();

		void setMarginBackgroundColor(const QColor& color_);
		QColor getMarginBackgroundColor();

		void setMarginForegroundColor(const QColor& color_);
		QColor getMarginForegroundColor();

		void setMarginFont(const QFont& font_);
		QFont getMarginFont();

		// Special Characters
		void setEolMode(QsciScintilla::EolMode mode_);
		QsciScintilla::EolMode getEolMode();

		void setEolVisibility(bool visible_);
		bool getEolVisibility();

		void setAutoDetectEol(bool detect_);
		bool getAutoDetectEol();

		void setWhitespaceVisibility(QsciScintilla::WhitespaceVisibility visibility_);
		QsciScintilla::WhitespaceVisibility getWhitespaceVisibility();

		void setWrapMode(QsciScintilla::WrapMode mode_);
		QsciScintilla::WrapMode getWrapMode();

		void setWrapVisualFlagsEnabled(bool enabled_);
		bool getWrapVisualFlagsEnabled();

		void setStartWrapVisualFlag(QsciScintilla::WrapVisualFlag flag_);
		QsciScintilla::WrapVisualFlag getStartWrapVisualFlag();

		void setEndWrapVisualFlag(QsciScintilla::WrapVisualFlag flag_);
		QsciScintilla::WrapVisualFlag getEndWrapVisualFlag();

		void setWrappedLineIndentWidth(int witdh_);
		int getWrappedLineIndentWidth();

		// APIs, Lexers
		QMap<QString, QStringList> getDefaultLexerAssociations();

		void setAssociations(const QMap<QString, QStringList>& associations_);
		QMap<QString, QStringList> getAssociations();

		void setInvisibleLexers(const QStringList& lexers_);
		QStringList getInvisibleLexers();

		// AStyle
		void setAstyleIndenterOptions(AStyleIndenter& asi__);

		// Switcher
		void setSwitcherNotification(bool enable_);
		bool getSwitcherNotification();
		void setSwitcherValues(const QMap<QString, QStringList>& associations_);
		QMap<QString, QStringList> getDefaultSwitcherValues();
		QMap<QString, QStringList> getSwitcherValues();
		
		// Update
		void setCheckOnStartUp(bool check_);
		bool getCheckOnStartUp();
		void setShowPopUpOnError(bool check_);
		bool getShowPopUpOnError();		
		void setProxyEnable(bool enable_);
		bool getProxyEnable();
		void setProxyAddress(const QString& proxy_);
		QString getProxyAddress();
		void setProxyPort(int port_);
		int getProxyPort();
		void setProxyAuthentification(bool enable_);
		bool getProxyAuthentification();
		void setProxyUser(const QString& user_);
		QString getProxyUser();
		void setProxyPassword(const QString& password_);
		QString getProxyPassword();
		
};

#endif // __SETTINGS_H__
