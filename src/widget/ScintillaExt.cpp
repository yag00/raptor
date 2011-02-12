#include <QtGui>

#include "ScintillaExt.h"

// The default fold margin width.
static const int defaultFoldMarginWidth = 14;

static const int MARK_HIDELINESBEGIN = 23;
static const int MARK_HIDELINESEND = 22;

ScintillaExt::ScintillaExt(QWidget* parent_) : QsciScintilla(parent_){
	connect(this, SIGNAL(linesChanged()), this, SLOT(updateLineNumberWidth()));
}

ScintillaExt::~ScintillaExt(){
}

void ScintillaExt::increaseIndentation(){
	SendScintilla(SCI_TAB);
}

void ScintillaExt::decreaseIndentation(){
	SendScintilla(SCI_BACKTAB);
}

void ScintillaExt::doTrimTrailing(){
	QString eol = getEol();
	QString buffer;

	int nbLines = lines();
	for (int line = 0; line < nbLines; line++){
		QString str = text(line);
		int i = str.size() - 1;

		if(i < 0)
			continue;
		if(!str[i].isSpace())
			continue;

		i--;
		while( i >= 0 && str[i].isSpace())
			i--;

		str.truncate(i+1);
		str.append(eol);
		buffer.append(str);
	}
	//replace text
	//note : we do not use setText to keep the undo action available
	int start = SendScintilla(SCI_POSITIONFROMLINE, 0);
	int end = SendScintilla(SCI_GETLINEENDPOSITION, nbLines - 1);
	SendScintilla(SCI_SETTARGETSTART, start);
	SendScintilla(SCI_SETTARGETEND, end);
	SendScintilla(SCI_REPLACETARGET, -1, buffer.toStdString().c_str());
}

void ScintillaExt::doSimplified(){
	QString eol = getEol();
	QString buffer;

	int nbLines = lines();
	for (int line = 0; line < nbLines; line++){
		QString str = text(line);
		str = str.simplified();
		str.append(eol);
		buffer.append(str);
	}
	//replace text
	//note : we do not use setText to keep the undo action available
	int start = SendScintilla(SCI_POSITIONFROMLINE, 0);
	int end = SendScintilla(SCI_GETLINEENDPOSITION, nbLines - 1);
	SendScintilla(SCI_SETTARGETSTART, start);
	SendScintilla(SCI_SETTARGETEND, end);
	SendScintilla(SCI_REPLACETARGET, -1, buffer.toStdString().c_str());
}

void ScintillaExt::convertSelectedTextToUpperCase(){
	if(!hasSelectedText())
		return;
	QString text = selectedText();
	text = text.toUpper();

	SendScintilla(SCI_TARGETFROMSELECTION);
	SendScintilla(SCI_REPLACETARGET, -1, text.toStdString().c_str());
}

void ScintillaExt::convertSelectedTextToLowerCase(){
	if(!hasSelectedText())
		return;
	QString text = selectedText();
	text = text.toLower();

	SendScintilla(SCI_TARGETFROMSELECTION);
	SendScintilla(SCI_REPLACETARGET, -1, text.toStdString().c_str());
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
void ScintillaExt::deleteCurrentWord(){
	beginUndoAction();
	SendScintilla(SCI_DELWORDLEFT);
	SendScintilla(SCI_DELWORDRIGHTEND);
	endUndoAction();
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
			pixmap.load(":/images/linux.png");
			break;
		case EolWindows:
			pixmap.load(":/images/windows.png");
			break;
		case EolMac:
			pixmap.load(":/images/mac.png");
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
	SendScintilla(SCI_SETSELFORE, 0, QColor::QColor());
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

bool ScintillaExt::isQualifiedWord(const QString& str_) const {
	for (int i = 0 ; i < (int)str_.size(); i++){
		//if(!str_[i].isLetterOrNumber())
		if (!isWordChar(str_[i].toAscii()))
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
