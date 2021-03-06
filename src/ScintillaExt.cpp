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

#include <QtWidgets>

#include "../astyle/AStyleIndenter.h"
#include "../settings/Settings.h"
#include "ScintillaExt.h"

// The default fold margin width.
static const int defaultFoldMarginWidth = 14;

static const int MARK_HIDELINESBEGIN = 23;
static const int MARK_HIDELINESEND = 22;

namespace{
	struct CharacterRange {
		long cpMin;
		long cpMax;
	};

	struct TextRange {
		struct CharacterRange chrg;
		char *lpstrText;
	};

	struct TextToFind {
		struct CharacterRange chrg;
		char *lpstrText;
		struct CharacterRange chrgText;
	};
}

ScintillaExt::ScintillaExt(QWidget* parent_) : QsciScintilla(parent_){
#ifdef Q_WS_WIN
	/** the "courier new font" (default font for cpp lexer ... ) with the default
	 * value (SC_EFF_QUALITY_DEFAULT) is kind of blurred
	 * this was not usefull for previous version of qscintilla (<2.5) ??
	 * version 2.5.1 fix the regression */
	//SendScintilla(SCI_SETFONTQUALITY, SC_EFF_QUALITY_NON_ANTIALIASED);
#endif
	setAcceptDrops(true);
	_HLID1 = -1;
	_HLID2 = -1;

	connect(this, SIGNAL(selectionChanged()), this, SLOT(selectedTextChanged()));
	connect(this, SIGNAL(linesChanged()), this, SLOT(updateLineNumberWidth()));
	connect(this, SIGNAL(linesChanged()), this, SLOT(checkHighlight()));
	connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(checkHighlight()));
}

ScintillaExt::~ScintillaExt(){
}

void ScintillaExt::increaseIndentation(){
	SendScintilla(SCI_TAB);
}

void ScintillaExt::decreaseIndentation(){
	SendScintilla(SCI_BACKTAB);
}

void ScintillaExt::reindent(){
	Settings settings;
	AStyleIndenter asi;
	settings.setAstyleIndenterOptions(asi);
	QString text = asi.format(this->text());
	replaceAllText(text);
}

void ScintillaExt::replaceAllText(const QString& text_){
	//get the current line
	int currentLine = firstVisibleLine();
	int cline,cindex;
	getCursorPosition(&cline, &cindex);

	//replace text
	//note : we do not use setText to keep the undo action available
	int start = SendScintilla(SCI_POSITIONFROMLINE, 0);
	int end = SendScintilla(SCI_GETLINEENDPOSITION, lines() - 1);
	SendScintilla(SCI_SETTARGETSTART, start);
	SendScintilla(SCI_SETTARGETEND, end);
	QByteArray s = text_.toUtf8();
	SendScintilla(SCI_REPLACETARGET, -1, s.constData());

	//set the current line visible to have the same view as before
	setFirstVisibleLine(currentLine);
	setCursorPosition(cline, cindex);
}

void ScintillaExt::tabsToSpaces(){
	QString spaces;
	spaces.fill(' ', tabWidth());

	QString buffer;

	int nbLines = lines();
	for (int line = 0; line < nbLines; line++){
		QString str = text(line);
		str.replace('\t', spaces);
		buffer.append(str);
	}

	replaceAllText(buffer);
}

void ScintillaExt::spacesToTabs(){
	QString spaces;
	spaces.fill(' ', tabWidth());

	QString buffer;

	int nbLines = lines();
	for (int line = 0; line < nbLines; line++){
		QString str = text(line);
		str.replace(spaces, QString('\t'));
		buffer.append(str);
	}

	replaceAllText(buffer);
}

void ScintillaExt::trimTrailingSpaces(){
	QString eol = getEol();
	QString buffer;

	int nbLines = lines();
	for (int line = 0; line < nbLines; line++){
		QString str = text(line);
		int i = str.size() - 1;

		if(i < 0)
			continue;

		while( i >= 0 && str[i].isSpace())
			i--;

		str.truncate(i+1);
		str.append(eol);
		buffer.append(str);
	}

	replaceAllText(buffer);
}

void ScintillaExt::compressSpaces(){
	QString eol = getEol();
	QString buffer;

	int nbLines = lines();
	for (int line = 0; line < nbLines; line++){
		QString str = text(line);
		str = str.simplified();
		str.append(eol);
		buffer.append(str);
	}

	replaceAllText(buffer);
}

void ScintillaExt::convertSelectedTextToUpperCase(){
	if(!hasSelectedText())
		return;
	QString text = selectedText();
	text = text.toUpper();

	SendScintilla(SCI_TARGETFROMSELECTION);
	QByteArray s = text.toUtf8();
	SendScintilla(SCI_REPLACETARGET, -1, s.constData());
}

void ScintillaExt::convertSelectedTextToLowerCase(){
	if(!hasSelectedText())
		return;
	QString text = selectedText();
	text = text.toLower();

	SendScintilla(SCI_TARGETFROMSELECTION);
	QByteArray s = text.toUtf8();
	SendScintilla(SCI_REPLACETARGET, -1, s.constData());
}

void ScintillaExt::duplicateCurrentLine(){
	SendScintilla(SCI_LINEDUPLICATE);
}
void ScintillaExt::duplicateSelection(){
	SendScintilla(SCI_SELECTIONDUPLICATE);
}
void ScintillaExt::duplicateSelectionOrCurrentLine(){
	if (hasSelectedText())
		duplicateSelection();
	else
		duplicateCurrentLine();
}
void ScintillaExt::copyCurrentLine(){
	SendScintilla(SCI_LINECOPY);
}
void ScintillaExt::cutCurrentLine(){
	SendScintilla(SCI_LINECUT);
}
void ScintillaExt::deleteCurrentLine(){
	SendScintilla(SCI_LINEDELETE);
}
void ScintillaExt::moveCurrentLineUp(){
	if (hasSelectedText()) {
		int start, end, from, to;
		getSelection(&start, &from, &end, &to);
		int end2 = end;
		if (!to) end--;

		if (!start)
			return;

		setUpdatesEnabled(false);
		beginUndoAction();

		for (int i = start; i <= end; i++){
			setCursorPosition(i, 0);
			SendScintilla(SCI_LINETRANSPOSE);
		}

		setSelection(start-1, from, end2-1, to);

		endUndoAction();
		setUpdatesEnabled(true);
		return;
	}

	SendScintilla(SCI_LINETRANSPOSE);

	int line, index;
	getCursorPosition(&line, &index);
	setCursorPosition(line-1, index);
}
void ScintillaExt::moveCurrentLineDown(){
	if (hasSelectedText()){
		int start, end, from, to;
		getSelection(&start, &from, &end, &to);
		int end2 = end;
		if (!to) end--;

		if (end >= lines()-1)
			return;

		setUpdatesEnabled(false);
		beginUndoAction();

		for (int i = end+1; i > start; i--){
			setCursorPosition(i, 0);
			SendScintilla(SCI_LINETRANSPOSE);
		}

		setSelection(start+1, from, end2+1, to);

		endUndoAction();
		setUpdatesEnabled(true);
		return;
	}

	int line, index;
	getCursorPosition(&line, &index);
	setCursorPosition(line+1, index);
	SendScintilla(SCI_LINETRANSPOSE);
}
void ScintillaExt::transposeLine(){
	SendScintilla(SCI_LINETRANSPOSE);
}

void ScintillaExt::deleteCurrentWord(){
	beginUndoAction();
	SendScintilla(SCI_DELWORDLEFT);
	SendScintilla(SCI_DELWORDRIGHTEND);
	endUndoAction();
}

void ScintillaExt::gotoLine(int line_, int pos_){
	if (line_ > lines())
		line_ = lines();
	setCursorPosition(line_ - 1, pos_);
	ensureLineVisible(line_ - 1);
	ensureCursorVisible();
	setFocus(Qt::OtherFocusReason);
}

QString ScintillaExt::getEol() const{
	QString eol;
	switch (eolMode()){
		case EolWindows: //CRLF
			eol = "\r\n";
			break;
		case EolUnix://LF
			eol = "\n";
			break;
		case EolMac://CR
			eol = "\r";
			break;
	}
	return eol;
}
QString ScintillaExt::getFormat() const{
	QString format = "";
	switch (eolMode()){
		case EolUnix:
			format = "UNIX";
			break;
		case EolWindows:
			format = "Dos/Windows";
			break;
		case EolMac:
			format = "MAC";
			break;
	}
	return format;
}
QPixmap ScintillaExt::getFormatPixmap() const{
	QPixmap pixmap;
	switch (eolMode()){
		case EolUnix:
			pixmap.load(":/os/linux.png");
			break;
		case EolWindows:
			pixmap.load(":/os/windows.png");
			break;
		case EolMac:
			pixmap.load(":/os/mac.png");
			break;
	}
	return pixmap;
}

void ScintillaExt::setLineNumbersMarginEnabled(bool enable_){
	setMarginLineNumbers(0, enable_);
	if(enable_)
		updateLineNumberWidth();
	else
		setMarginWidth(0, 0);
}

void ScintillaExt::setNoSelectionForegroundColor(){
	QColor color;
	SendScintilla(SCI_SETSELFORE, 0, color);
}

void ScintillaExt::updateLineNumberWidth() {
	if(marginLineNumbers(0))
		setLineNumbersMarginWidth( QString::number( lines() ).length() );
}

void ScintillaExt::setLineNumbersMarginWidth(int i){
	int j = i;
	if ( i != 0 )
		j++;
	setMarginWidth( 0, QString().fill( '0', j ) );
}

void ScintillaExt::setFoldMarker(int marknr, int mark, const QColor& foreground_, const QColor& background_) {
    SendScintilla(SCI_MARKERDEFINE, marknr, mark);
    if (mark != SC_MARK_EMPTY) {
        SendScintilla(SCI_MARKERSETFORE, marknr, foreground_);
        SendScintilla(SCI_MARKERSETBACK, marknr, background_);
    }
}

void ScintillaExt::setFolding(FoldStyleExt folding_, const QColor& foreground_, const QColor& background_, int margin_) {
	///@todo fixme avoid this useless call but we need to update private member(fold,foldmargin)
	QsciScintilla::setFolding((QsciScintilla::FoldStyle)folding_, margin_);

	if (folding_ == NoFoldStyle) {
        SendScintilla(SCI_SETMARGINWIDTHN, margin_, 0L);
        return;
    }

    int mask = SendScintilla(SCI_GETMODEVENTMASK);
    SendScintilla(SCI_SETMODEVENTMASK, mask | SC_MOD_CHANGEFOLD);

    SendScintilla(SCI_SETFOLDFLAGS, SC_FOLDFLAG_LINEAFTER_CONTRACTED);

    SendScintilla(SCI_SETMARGINTYPEN, margin_, (long)SC_MARGIN_SYMBOL);
    SendScintilla(SCI_SETMARGINMASKN, margin_, SC_MASK_FOLDERS);
    SendScintilla(SCI_SETMARGINSENSITIVEN, margin_, 1);


	switch (folding_) {
		case PlainFoldStyle:
			setFoldMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDER, SC_MARK_PLUS, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, foreground_, background_);
			break;

		case CircledFoldStyle:
			setFoldMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_CIRCLEMINUS, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDER, SC_MARK_CIRCLEPLUS, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, foreground_, background_);
			break;

		case BoxedFoldStyle:
			setFoldMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, foreground_, background_);
			break;

		case CircledTreeFoldStyle:
			setFoldMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_CIRCLEMINUS, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDER, SC_MARK_CIRCLEPLUS, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNERCURVE, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDEREND, SC_MARK_CIRCLEPLUSCONNECTED, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_CIRCLEMINUSCONNECTED, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNERCURVE, foreground_, background_);
			break;

		case BoxedTreeFoldStyle:
			setFoldMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDEREND, SC_MARK_BOXPLUSCONNECTED, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER, foreground_, background_);
			break;
	case ArrowFoldStyle:
			setFoldMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_ARROWDOWN, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDER, SC_MARK_ARROW, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY, foreground_, background_);
			break;
	case ArrowTreeFoldStyle:
			setFoldMarker(SC_MARKNUM_FOLDEROPEN, SC_MARK_ARROWDOWN, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDER, SC_MARK_ARROW, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDEREND, SC_MARK_PLUS, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDEROPENMID, SC_MARK_MINUS, foreground_, background_);
			setFoldMarker(SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER, foreground_, background_);
			break;
		default:
			break;
    }

    SendScintilla(SCI_SETMARGINWIDTHN, margin_, defaultFoldMarginWidth);
}

bool ScintillaExt::isLineEmpty(int line_) const {
	int start = SendScintilla(SCI_POSITIONFROMLINE, line_);
	int end = SendScintilla(SCI_GETLINEENDPOSITION, line_);
	for (int i = start; i < end; i++) {
		char ch = SendScintilla(SCI_GETCHARAT, i);
		if (ch != ' ' && ch != '\t')
			return false;
	}
	return true;
}

bool ScintillaExt::isQualifiedWord(const QString& str_) const {
	for (int i = 0 ; i < (int)str_.size(); i++){
		//if(!str_[i].isLetterOrNumber())
		if (!isWordChar(str_[i].toLatin1()))
			return false;
	}
	return true;
};

bool ScintillaExt::isWordChar(char ch) const {
	if (ch < 0x20)
		return false;

	switch(ch)
	{
		case ' ':
		case '	':
		case '\n':
		case '\r':
		case '.':
		case ',':
		case '?':
		case ';':
		case ':':
		case '!':
		case '(':
		case ')':
		case '[':
		case ']':
		case '+':
		case '-':
		case '*':
		case '/':
		case '#':
		case '@':
		case '^':
		case '%':
		case '$':
		case '"':
		case '\'':
		case '~':
		case '&':
		case '{':
		case '}':
		case '|':
		case '=':
		case '<':
		case '>':
		case '\\':
			return false;
	}
	return true;
};

int ScintillaExt::getCurrentPosition() const {
	return SendScintilla(SCI_GETCURRENTPOS);
}

int ScintillaExt::getCurrentLine() const {
	return SendScintilla(SCI_LINEFROMPOSITION, getCurrentPosition());
}

int ScintillaExt::getSelectedTextSize() const{
	return (SendScintilla(SCI_GETSELECTIONEND) - SendScintilla(SCI_GETSELECTIONSTART));
}

QString ScintillaExt::getTextRange(int pos1_, int pos2_) {
	TextRange tr;
	tr.chrg.cpMin = pos1_;
	tr.chrg.cpMax = pos2_;
	tr.lpstrText = new char[pos2_ - pos1_ + 1];
	SendScintilla(SCI_GETTEXTRANGE, 0, &tr);
	QString str = QString::fromUtf8(tr.lpstrText, pos2_ - pos1_);
	delete[] tr.lpstrText;
	return str;
}

void ScintillaExt::foldLevel(int level_){
	for (int line = 0; line < lines(); line++) {
		int level = SendScintilla(SCI_GETFOLDLEVEL, line);
		if (level & SC_FOLDLEVELHEADERFLAG){
			level -= SC_FOLDLEVELBASE;
			if (level_ == (level & SC_FOLDLEVELNUMBERMASK)){
				if(SendScintilla(SCI_GETFOLDEXPANDED, line) == 1) //not fold
					foldLine(line);
			}
		}
	}
}

void ScintillaExt::unfoldLevel(int level_){
	for (int line = 0; line < lines(); line++) {
		int level = SendScintilla(SCI_GETFOLDLEVEL, line);
		if (level & SC_FOLDLEVELHEADERFLAG){
			level -= SC_FOLDLEVELBASE;
			if (level_ == (level & SC_FOLDLEVELNUMBERMASK)){
				if(SendScintilla(SCI_GETFOLDEXPANDED, line) == 0) //fold
					foldLine(line);
			}
		}
	}
}

void ScintillaExt::wheelEvent(QWheelEvent *event_){
	QsciScintilla::wheelEvent(event_);
	if (event_->modifiers() == Qt::ControlModifier){
		if(event_->delta() < 0)
			zoomOut();
		else
			zoomIn();
	}
}

void ScintillaExt::paintEvent(QPaintEvent *event_){
	updateLineNumberWidth();
	QsciScintilla::paintEvent(event_);
}

void ScintillaExt::dragEnterEvent(QDragEnterEvent *event_){
	event_->acceptProposedAction();
}

void ScintillaExt::dragMoveEvent(QDragMoveEvent* event){
	// if some actions should not be usable, like move, this code must be adopted
	event->acceptProposedAction();
}

void ScintillaExt::dragLeaveEvent(QDragLeaveEvent* event){
	event->accept();
}

void ScintillaExt::dropEvent(QDropEvent* event_){
	const QMimeData* mimeData = event_->mimeData();
	// check for our needed mime type, here a file or a list of files
	if (mimeData->hasUrls()) {
		QStringList pathList;
		QList<QUrl> urlList = mimeData->urls();
 
		// extract the local paths of the files
		for (int i = 0; i < urlList.size() && i < 32; ++i) {
			pathList.append(urlList.at(i).toLocalFile());
		}
 
		// emit signal
		emit fileDropped(pathList);
		event_->acceptProposedAction();
	}
}

void ScintillaExt::selectedTextChanged(){
	int selectionSize = getSelectedTextSize();
	if ((selectionSize > 1) && (selectionSize < length())){
		// there is a selected text and it's not a select all
		//apply quick markup
		applyIndicator(selectedText(), INDICATOR_QUICK_SEARCH1, INDICATOR_QUICK_SEARCH2);
	} else {
		// remove quick markup
		clearIndicators(INDICATOR_QUICK_SEARCH1, INDICATOR_QUICK_SEARCH2);
	}


	/*// save target locations for other search functions
	int originalStartPos = SendScintilla(SCI_GETTARGETSTART);
	int originalEndPos = SendScintilla(SCI_GETTARGETEND);

	//clear marker
	//SendScintilla(SCI_SETINDICATORCURRENT, SCE_UNIVERSAL_FOUND_STYLE_SMART);
	SendScintilla(SCI_INDICATORCLEARRANGE, 0, text().length());


	QString selection = selectedText();
	if(selection == "")
		return;
	if(!isQualifiedWord(selection))
		return;

	int lineFrom, lineTo, indexFrom, indexTo;
	getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);
	if(lineFrom != lineTo)
		return;
	QString textLine = text(lineFrom);
	if (isWordChar(textLine[indexFrom-1].toLatin1()))
		return;
	if (isWordChar(textLine[indexTo].toLatin1()))
		return;



	//a single word is selected, highlight the word in the whole document
	qDebug() << selection << " " << lineFrom << " " << indexFrom << " " << lineTo << " " << indexTo;
	//setSelectionBackgroundColor(QColor(0,255,0));


	int firstLine =		SendScintilla(SCI_GETFIRSTVISIBLELINE);
	int nrLines =		SendScintilla(SCI_LINESONSCREEN);
	int lastLine =		firstLine+nrLines;
	int startPos =		0;//(int)pHighlightView->execute(SCI_POSITIONFROMLINE, firstLine);
	int endPos =		0;//(int)pHighlightView->execute(SCI_POSITIONFROMLINE, lastLine);

	int currentLine = firstLine;
	int prevDocLineChecked = -1;	//invalid start

	for(; currentLine < lastLine; currentLine++) {
		int docLine = SendScintilla(SCI_DOCLINEFROMVISIBLE, currentLine);
		if (docLine == prevDocLineChecked)
			continue;	//still on same line (wordwrap)
		prevDocLineChecked = docLine;
		startPos = SendScintilla(SCI_POSITIONFROMLINE, docLine);
		endPos = SendScintilla(SCI_POSITIONFROMLINE, docLine+1);

		int targetStart = 0;
		int targetEnd = 0;

		//Initial range for searching
		SendScintilla(SCI_SETSEARCHFLAGS, SCFIND_WHOLEWORD);
		targetStart = searchInTarget(selection.toStdString().c_str(), selection.size(), startPos, endPos);
		qDebug() << currentLine << " " << startPos << " " << endPos << " " << targetStart;

		while (targetStart != -1){
			targetStart = SendScintilla(SCI_GETTARGETSTART);
			targetEnd = SendScintilla(SCI_GETTARGETEND);
			if (targetEnd > endPos) {	//we found a result but outside our range, therefore do not process it
				break;
			}
			//SendScintilla(SCI_INDICSETSTYLE, 0, SCE_UNIVERSAL_TAGMATCH);
			//SendScintilla(SCI_INDICSETSTYLE, 0, INDIC_ROUNDBOX );
			//SendScintilla(SCI_INDICSETFORE, 0, 0x00ff00);
			//SendScintilla(SCI_INDICSETALPHA, 0, 255);
			SendScintilla(SCI_INDICATORFILLRANGE,  targetStart, selection.size());

			startPos = targetStart + selection.size();
			targetStart = searchInTarget(selection.toStdString().c_str(), selection.size(), startPos, endPos);
		}
	}

	// restore the original targets to avoid conflicts with the search/replace functions
	SendScintilla(SCI_SETTARGETSTART, originalStartPos);
	SendScintilla(SCI_SETTARGETEND, originalEndPos);*/
}

void ScintillaExt::createIndicator(int id_, int type_, const QColor &color_, bool under_) {
	SendScintilla(SCI_INDICSETSTYLE, id_, type_);
	SendScintilla(SCI_INDICSETFORE, id_, color_);
	//SendScintilla(SCI_INDICSETALPHA, id_, color_.alpha());
	SendScintilla(SCI_INDICSETALPHA, id_, 100);
	SendScintilla(SCI_INDICSETUNDER, id_, under_);
}

void ScintillaExt::applyIndicator(int start_, int end_, int id_){
	SendScintilla(SCI_SETINDICATORCURRENT, id_);
	SendScintilla(SCI_INDICATORFILLRANGE, start_, end_-start_);
}

void ScintillaExt::clearIndicators(int id1_, int id2_){
	SendScintilla(SCI_SETINDICATORCURRENT, id1_);
	SendScintilla(SCI_INDICATORCLEARRANGE, 0, length());

	if (id2_ >= 0){
		SendScintilla(SCI_SETINDICATORCURRENT, id2_);
		SendScintilla(SCI_INDICATORCLEARRANGE, 0, length());
	}

	if (id1_ == _HLID1)
	_HLID1 = -1;
	if (id2_ == _HLID2)
	_HLID2 = -1;
}

void ScintillaExt::applyIndicator(const QString &text_, int id1_, int id2_){
	clearIndicators(id1_);
	clearIndicators(id2_);

	_HLText = text_;
	_HLID1 = id1_;
	_HLID2 = id2_;
	highlightVisible(text_, id1_, id2_);
}

void ScintillaExt::checkHighlight(){
	if (_HLID1 >= 0)
		highlightVisible(_HLText, _HLID1, _HLID2);
}

void ScintillaExt::highlightVisible(const QString &text_, int id1_, int id2_){
	//text is to big, highlighting is useless
	if (text_.size() >= 10000)
		return;
	if (text_.simplified().isEmpty())
		return;

	int line1 = firstVisibleLine();   // visual coords
	line1 = SendScintilla(SCI_DOCLINEFROMVISIBLE, line1);   // document coords
	if (line1 < 0)
		line1 = 0;
	int line2 = line1 + linesVisible() + 2;

	int pos1 = positionFromLineIndex(line1, 0);
	if (pos1 < 0)
		pos1 = 0;
	int pos2 = positionFromLineIndex(line2, 0);
	if (pos2 < 0)
		pos2 = length();

	///@todo make it configurable
	int flags = 0;//SCFIND_MATCHCASE | SCFIND_WHOLEWORD | SCFIND_WORDSTART;
	TextToFind ttf;
	ttf.chrg.cpMin = pos1;
	ttf.chrg.cpMax = pos2;

	TextRange tr;
	
	if((pos2 - pos1) <= 0)
		return;
	if((pos2 - pos1) > 10000)
		return;
	
	char* buf = new char[(pos2 - pos1) + 1];

	int res;
	QByteArray ba = text_.toUtf8();
	do {
		ttf.lpstrText = ba.data();
		res = SendScintilla(SCI_FINDTEXT, flags, &ttf);
		if (res < 0)
			break;

		ttf.chrg.cpMin = ttf.chrgText.cpMax;

		// markup
		if (id2_ < 0)
			SendScintilla(SCI_SETINDICATORCURRENT, id1_);
		else {
			tr.chrg = ttf.chrgText;
			tr.lpstrText = buf;
			SendScintilla(SCI_GETTEXTRANGE, 0, &tr);
			QString txt = QString::fromUtf8(tr.lpstrText);
			SendScintilla(SCI_SETINDICATORCURRENT, txt == text_ ? id1_ : id2_);
		}

		SendScintilla(SCI_INDICATORFILLRANGE, ttf.chrgText.cpMin, ttf.chrgText.cpMax-ttf.chrgText.cpMin);
	} while (res >= 0);

	delete []buf;

	// remove selection
	pos1 = SendScintilla(SCI_GETSELECTIONSTART);
	pos2 = SendScintilla(SCI_GETSELECTIONEND);
	SendScintilla(SCI_INDICATORCLEARRANGE, pos1, pos2-pos1);
}

int ScintillaExt::getStyleAt(int position_) const {
	return SendScintilla(SCI_GETSTYLEAT, position_);
}

char ScintillaExt::getCharAt(int position_)  const {
	return SendScintilla(SCI_GETCHARAT, position_);
}

#if WAF
#include "ScintillaExt.moc"
#endif

