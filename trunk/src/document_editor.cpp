#include <QtGui>
#include <QPrintDialog>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscimacro.h>
#include <Qsci/qsciprinter.h>
#include <Qsci/qscilexer.h>
#include <Qsci/qscilexercpp.h>

#include "lexer/lexer_manager.h"
#include "settings/settings.h"
#include "document_editor.h"

#define INDICATOR_SEARCH          0
#define INDICATOR_QUICK_SEARCH1   1
#define INDICATOR_QUICK_SEARCH2   2

#define INDICATOR_HIGHLIGHT1      3
#define INDICATOR_HIGHLIGHT2      4
#define INDICATOR_HIGHLIGHT3      5
#define INDICATOR_HIGHLIGHT4      6
#define INDICATOR_HIGHLIGHT5      7
#define INDICATOR_HIGHLIGHT6      8

#define MARKER_BOOK       0
#define MARKER_BOOK_MASK  (1 << MARKER_BOOK)

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

DocumentEditor::DocumentEditor(QWidget* parent_) : ScintillaExt(parent_){
	_HLID1 = -1;
	_HLID2 = -1;
	_autoDetectEol = false;
	_autoDetectIndent = false;
	_isNew = true;
	_isCloned = false;
	_clone = 0;
	_type = "Normal Text file";
	_fullPath = "";
	_codec = "";

	//macro
	_macro = new QsciMacro(this);
	
	//load settings
	Settings settings;
	settings.applyToDocument(this);

	//	set the 1st margin accept markers 
	//	number 1 and 2 (binary mask 00000110 == 6)
	//setMarginMarkerMask(1, 7);

	/*markerDefine(QsciScintilla::Circle, MARKER_BOOK);
	setMarkerBackgroundColor(Qt::green, MARKER_BOOK);
	setMarkerForegroundColor(Qt::black, MARKER_BOOK);*/
	markerDefine(QPixmap(":/images/ledblue.png").scaled(40,16, Qt::KeepAspectRatio, Qt::SmoothTransformation), MARKER_BOOK);


	setModified(false);

	// define search indicator
	createIndicator(INDICATOR_SEARCH, INDIC_ROUNDBOX, Qt::green);
	// define quick search indicators
	createIndicator(INDICATOR_QUICK_SEARCH1, INDIC_ROUNDBOX, Qt::green);
	createIndicator(INDICATOR_QUICK_SEARCH2, INDIC_ROUNDBOX, Qt::blue);
	// define highlight indicators
	createIndicator(INDICATOR_HIGHLIGHT1, INDIC_ROUNDBOX, Qt::cyan);
	createIndicator(INDICATOR_HIGHLIGHT2, INDIC_ROUNDBOX, Qt::green);
	createIndicator(INDICATOR_HIGHLIGHT3, INDIC_ROUNDBOX, Qt::magenta);
	createIndicator(INDICATOR_HIGHLIGHT4, INDIC_ROUNDBOX, Qt::red);
	createIndicator(INDICATOR_HIGHLIGHT5, INDIC_ROUNDBOX, Qt::blue);
	createIndicator(INDICATOR_HIGHLIGHT6, INDIC_ROUNDBOX, Qt::yellow);
	
	//connection
	connect(this, SIGNAL(selectionChanged()), this, SLOT(selectedTextChanged()));
	connect(this, SIGNAL(linesChanged()), this, SLOT(checkHighlight()));
	connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(checkHighlight()));
	connect(this, SIGNAL(marginClicked(int,int, Qt::KeyboardModifiers)), this, SLOT(toggleBookmark(int,int, Qt::KeyboardModifiers)));
}
DocumentEditor::DocumentEditor(DocumentEditor* document_, QWidget *parent_) : ScintillaExt(parent_){
	_HLID1 = -1;
	_HLID2 = -1;
	_autoDetectEol = document_->_autoDetectEol;
	_autoDetectIndent = document_->_autoDetectIndent;
	_codec = document_->_codec;	
	_isNew = document_->isNew();
	_fullPath = document_->getFullPath();
	_type = document_->getType();
	setDocument(document_->document());
	
	_clone = document_;
	_isCloned = true;
	document_->_clone = this;
	document_->_isCloned = true;
	
	QsciLexer* l = document_->lexer();
	if(l != 0){
		QString lexLang = l->language();
		QsciLexer* newLex = LexerManager::getInstance().lexerFactory(lexLang, this);
		if(newLex == 0){
			//could not find the lexer
			newLex = LexerManager::getInstance().getAutoLexer(this);
		}
		setLexer(newLex);
		_type = LexerManager::getInstance().getFileType(lexer());
	}
	
	//macro
	_macro = new QsciMacro(this);
	
	//load settings
	Settings settings;
	settings.applyToDocument(this);

	markerDefine(QPixmap(":/images/ledblue.png").scaled(40,16, Qt::KeepAspectRatio, Qt::SmoothTransformation), MARKER_BOOK);
	
	//connection
	connect(this, SIGNAL(selectionChanged()), this, SLOT(selectedTextChanged()));
	connect(this, SIGNAL(linesChanged()), this, SLOT(checkHighlight()));
	connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(checkHighlight()));	
	connect(this, SIGNAL(marginClicked(int,int, Qt::KeyboardModifiers)), this, SLOT(toggleBookmark(int,int, Qt::KeyboardModifiers)));
}

DocumentEditor::~DocumentEditor(){
	if(isCloned()){
		_clone->detachClone();
		_clone->setLexer();
	}
}

QString DocumentEditor::getName() const{
	return QFileInfo(_fullPath).fileName();
}
QString DocumentEditor::getFullPath() const{
	return _fullPath;
}
QString DocumentEditor::getPath() const{
	return QFileInfo(_fullPath).path();
}
QString DocumentEditor::getType() const{
	return _type;
}

bool DocumentEditor::stillExist() const{
	if(isNew())
		return true;

	if(QFile(_fullPath).exists())
		return true;
	else{
		return false;
	}
}
bool DocumentEditor::isNew() const{
	return _isNew;
}

void DocumentEditor::print(bool quick_){
	// get printer
	QsciPrinter p;
	// set wrapmode
	p.setWrapMode(WrapWord);

	// if quick print
	if (quick_){
		// check if default printer is set
		if ( p.printerName().isEmpty() ){
			QMessageBox::warning(this, tr("Application"), tr( "There is no default printer, please set one before trying quick print" ));
			return;
		}
		
		// print and return
		p.printRange(this);
		return;
	}

	// printer dialog
	QPrintDialog d(&p);

	// if ok
	if (d.exec()){
		// print
		int f = -1, t = -1, i;
		if ( d.printRange() == QPrintDialog::Selection )
			getSelection( &f, &i, &t, &i );
		p.printRange( this, f, t );
	}
}
void DocumentEditor::quickPrint(){
	print(true);
}

bool DocumentEditor::save(){
    if (_fullPath.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(_fullPath);
    }
}
bool DocumentEditor::saveAs(){
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}
bool DocumentEditor::saveACopyAs(){
	QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveCopy(fileName);
}
bool DocumentEditor::maybeSave(){
    if (isModified()) {
        int ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document %1 has been modified.\n"
                        "Do you want to save your changes?").arg(getName()),
                     QMessageBox::Yes | QMessageBox::Default,
                     QMessageBox::No,
                     QMessageBox::Cancel | QMessageBox::Escape);
        if (ret == QMessageBox::Yes)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}
bool DocumentEditor::saveFile(const QString &fileName_){
    QFile file(fileName_);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName_)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << text();
    QApplication::restoreOverrideCursor();

    _fullPath = fileName_;
	setModified(false);
	_isNew = false;
    return true;
}
bool DocumentEditor::saveCopy(const QString &fileName_){
    QFile file(fileName_);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName_)
                             .arg(file.errorString()));
        return false;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    QTextStream out(&file);
    out << text();
    QApplication::restoreOverrideCursor();

    return true;
}

bool DocumentEditor::load(const QString &fileName_){
    QFile file(fileName_);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName_)
                             .arg(file.errorString()));
        return false;
    }
	
    QApplication::setOverrideCursor(Qt::WaitCursor);	
	QTextCodec* codec = QTextCodec::codecForName(_codec.toUtf8());	
	QString data = codec->toUnicode(file.readAll());
	setText(data);
    QApplication::restoreOverrideCursor();

	_fullPath = fileName_;
	_isNew = false;
	setModified(false);
	
	if(_autoDetectEol)
		autoDetectEol();
	if(_autoDetectIndent)
		autoDetectIndent();
	
	//add lexer
	QsciLexer* l = lexer();
	if(l != 0)
		delete l;
	setLexer(LexerManager::getInstance().getAutoLexer(this));
	_type = LexerManager::getInstance().getFileType(lexer());
	//reload settings for lexer
	Settings settings;
	settings.applyToDocument(this);
	return true;
}
bool DocumentEditor::reload(){
	if(isModified()){
		int ret = QMessageBox::warning(this,
					tr("Reload file"),
					 tr("File %1 has unsaved modifications.\n"
						 "Reloading will discard them.\n"
						"Do you want to proceed?").arg(getName()),
					 QMessageBox::Yes |
					 QMessageBox::Cancel);
		if (ret == QMessageBox::Cancel)
			return false;
	}
    return load(getFullPath());
}

void DocumentEditor::undo(){
	ScintillaExt::undo();
	//force view to update
	emit textChanged();
}
void DocumentEditor::redo(){
	ScintillaExt::redo();
	//force view to update
	emit textChanged();
}

void DocumentEditor::setLanguage(const QString &language_){
	QsciLexer* l = lexer();
	if(l != 0)
		delete l;
	/*clear();
	undo();*/
	l = LexerManager::getInstance().lexerFactory(language_, this);
	setLexer(l);
	_type = LexerManager::getInstance().getFileType(lexer());
	//reload settings for lexer
	Settings settings;
	settings.applyToDocument(this);	
}

void DocumentEditor::setAutoDetectEol(bool enable_){
	_autoDetectEol = enable_;
	if(_autoDetectEol)
		autoDetectEol();
}
void DocumentEditor::setAutoDetectIndent(bool enable_){
	_autoDetectIndent = enable_;
	if(_autoDetectIndent)
		autoDetectIndent();
}

void DocumentEditor::autoDetectIndent() {
	QString currText = "\n" + text(); // \n for more simple RegExp
	QRegExp tabRe = QRegExp ("\n\\t");
	int matchIntex;
	matchIntex = tabRe.indexIn (currText);
	if (matchIntex != -1) // Use tabs
	{
		setIndentationsUseTabs(true);
		return;
	}
	
	QRegExp spaceRe = QRegExp ("\n( +)");
	matchIntex = spaceRe.indexIn (currText);
	if (matchIntex != -1) // Use spaces
	{
		setIndentationsUseTabs(false);
		return;
	}
}

void DocumentEditor::autoDetectEol() {
	QString currText = text();
	if (currText.indexOf("\r\n") != -1) {
		setEolMode(QsciScintilla::EolWindows);
		return;
	}
	if (currText.indexOf("\n") != -1) {
		setEolMode(QsciScintilla::EolUnix);
		return;
	}
	if (currText.indexOf("\r") != -1) {
		setEolMode(QsciScintilla::EolMac);
		return;
	}	
}

void DocumentEditor::focusInEvent(QFocusEvent *event_){
	ScintillaExt::focusInEvent(event_);
	emit focusChanged(true);
	event_->accept();
}
void DocumentEditor::focusOutEvent(QFocusEvent *event_){
	ScintillaExt::focusOutEvent(event_);
	emit focusChanged(false);
	event_->accept();
}

void DocumentEditor::selectedTextChanged(){
	if (selectedText().size() > 1){
		// apply quick markup
		applyIndicator(selectedText(), INDICATOR_QUICK_SEARCH1, INDICATOR_QUICK_SEARCH2);
	} else{
		// remove quick markup
		clearIndicators(INDICATOR_QUICK_SEARCH1, INDICATOR_QUICK_SEARCH2);
	}
	return;
	
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
	if (isWordChar(textLine[indexFrom-1].toAscii()))
		return;
	if (isWordChar(textLine[indexTo].toAscii()))
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

QsciMacro* DocumentEditor::getMacro() const{
	return _macro;
}


bool DocumentEditor::isCloned() const{
	return _isCloned;
}
void DocumentEditor::detachClone(){
	//QsciLexer* l = lexer();
	
	//setDocument(_clone->document());
	/*if(l != 0){
		QString lexLang = l->language();
		QsciLexer* newLex = LexerManager::getInstance().lexerFactory("C", this);
		delete l;
		if(newLex == 0){
			newLex = LexerManager::getInstance().getAutoLexer(this);
		}
		setLexer(0);
		qDebug() << newLex->language();
		setLexer(newLex);
	}
	
	Settings settings;
	settings.applyToDocument(this);*/
	_clone = 0;
	_isCloned = false;
}
////////////////////////////////////////////////////////////////////////////////

void DocumentEditor::toggleBookmark() {
	int line, index;
	getCursorPosition(&line, &index);
	toggleBookmark(1, line, Qt::NoModifier);
}

void DocumentEditor::toggleBookmark(int margin_, int line_, Qt::KeyboardModifiers state_){
	(void)margin_;
	(void)state_;
	if(markersAtLine(line_) & MARKER_BOOK_MASK) {
		for (int i = 0; i < _bookmarks.size(); i++) {
			int id = _bookmarks.at(i);
			int xline = markerLine(id);
			if (xline == line_) {
				markerDeleteHandle(id);
				_bookmarks.removeAt(i);
				i--;
			}
		}
		return;
	}
	int id = markerAdd(line_, MARKER_BOOK);
	_bookmarks.append(id);
}

void DocumentEditor::removeAllBookmarks() {
	markerDeleteAll(MARKER_BOOK);
	_bookmarks.clear();
}

bool DocumentEditor::navigateBookmark(int id_) {
	int line = markerLine(id_);
	if (line < 0)
		return false;

	setCursorPosition(line, 0);
	ensureLineVisible(line);

	return true;
}

void DocumentEditor::nextBookmark() {
	int line, index;
	getCursorPosition(&line, &index);

	line = markerFindNext(line+1, MARKER_BOOK_MASK);
	if (line >= 0){
		setCursorPosition(line, 0);
		ensureLineVisible(line);
	}else{
		// check from start
		line = markerFindNext(0, MARKER_BOOK_MASK);
		if (line >= 0){
			setCursorPosition(line, 0);
			ensureLineVisible(line);
		}
	}
}

void DocumentEditor::prevBookmark() {
	int line, index;
	getCursorPosition(&line, &index);

	line = markerFindPrevious(line-1, MARKER_BOOK_MASK);
	if (line >= 0){
		setCursorPosition(line, 0);
		ensureLineVisible(line);
	}else{
		// check from end
		line = markerFindPrevious(lines(), MARKER_BOOK_MASK);
		if (line >= 0){
			setCursorPosition(line, 0);
			ensureLineVisible(line);
		}
	}
}

QStringList DocumentEditor::bookmarkedLinesToStringList() {
	QStringList list;
	for (int i = 0; i < _bookmarks.size(); i++) {
		int id = _bookmarks.at(i);
		int xline = markerLine(id);
		list.append(QString::number(xline));
	}
	return list;
}

void DocumentEditor::setBookmarks(const QStringList& lines_) {
	removeAllBookmarks();
	for (int i = 0; i < lines_.size(); i++) {
		int line = lines_.at(i).toInt();
		toggleBookmark(1, line, Qt::NoModifier);
	}
}

////////////////////////////////////////////////////////////////////////////////

void DocumentEditor::createIndicator(int id_, int type_, const QColor &color_, bool under_) {
	SendScintilla(SCI_INDICSETSTYLE, id_, type_);
	SendScintilla(SCI_INDICSETFORE, id_, color_);
	//SendScintilla(SCI_INDICSETALPHA, id_, color_.alpha());
	SendScintilla(SCI_INDICSETALPHA, id_, 100);
	SendScintilla(SCI_INDICSETUNDER, id_, under_);
}

void DocumentEditor::applyIndicator(int start_, int end_, int id_){
	SendScintilla(SCI_SETINDICATORCURRENT, id_);
	SendScintilla(SCI_INDICATORFILLRANGE, start_, end_-start_);
}

void DocumentEditor::clearIndicators(int id1_, int id2_){
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

void DocumentEditor::applyIndicator(const QString &text_, int id1_, int id2_){
	clearIndicators(id1_);
	clearIndicators(id2_);

	_HLText = text_;
	_HLID1 = id1_;
	_HLID2 = id2_;
	highlightVisible(text_, id1_, id2_);
}

void DocumentEditor::checkHighlight(){
	if (_HLID1 >= 0)
		highlightVisible(_HLText, _HLID1, _HLID2);
}

void DocumentEditor::highlightVisible(const QString &text_, int id1_, int id2_){
	if (text_.simplified().isEmpty())
		return;

	int line1 = firstVisibleLine();   // visual coords
	line1 = SendScintilla(SCI_DOCLINEFROMVISIBLE, line1);   // document coords
	if (line1 < 0) 
		line1 = 0;
	int line2 = line1 + linesVisible() + 2;

	for (int i = line1; i <= line2; i++){
		if (SendScintilla(SCI_GETLINEVISIBLE, i) == false)
			line2++;
	}

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
	char buf[text_.length() * 2 + 2];

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

	// remove selection
	pos1 = SendScintilla(SCI_GETSELECTIONSTART);
	pos2 = SendScintilla(SCI_GETSELECTIONEND);
	SendScintilla(SCI_INDICATORCLEARRANGE, pos1, pos2-pos1);
}

QString DocumentEditor::getCodec() const{
	return _codec;
}

void DocumentEditor::setCodec(const QString& codec_){
	if(_codec != codec_){
		_codec = codec_;
		QTextCodec* codec = QTextCodec::codecForName(_codec.toUtf8());
		if(!codec){
			_codec = "";
			return;
		}
		//update doc with codec
		QFile file(getFullPath());
		if (!file.open(QFile::ReadOnly)) {
			QMessageBox::warning(this, tr("Application"),
								 tr("Cannot read file %1:\n%2.")
								 .arg(getFullPath())
								 .arg(file.errorString()));
			return;
		}
		
		QApplication::setOverrideCursor(Qt::WaitCursor);	
		QString data = codec->toUnicode(file.readAll());
		setText(data);
		QApplication::restoreOverrideCursor();
	}
}

void DocumentEditor::setDefaultCodec(const QString& codec_){
	if(_codec.isEmpty()){
		_codec = codec_;
	}
}