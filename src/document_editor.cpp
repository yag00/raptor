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


#define MARKER_BOOK       0
#define MARKER_BOOK_MASK  (1 << MARKER_BOOK)


DocumentEditor::DocumentEditor(QWidget* parent_) : ScintillaExt(parent_){
	//codec
	setUtf8(true);
	_codec = "";
	_bomMode = BomLeaveAsIs;
	_hasBom = false;
	_charsetAutoDetect = true;

	_autoDetectEol = false;
	_autoDetectIndent = false;

	_isNew = true;
	_isCloned = false;
	_clone = 0;
	_type = "Normal Text file";
	_fullPath = "";

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
	connect(this, SIGNAL(marginClicked(int,int, Qt::KeyboardModifiers)), this, SLOT(toggleBookmark(int,int, Qt::KeyboardModifiers)));
}
DocumentEditor::DocumentEditor(DocumentEditor* document_, QWidget *parent_) : ScintillaExt(parent_){
	//codec
	setUtf8(true);
	_codec = document_->_codec;
	_bomMode = document_->_bomMode;
	_hasBom = document_->_hasBom;
	_charsetAutoDetect = document_->_charsetAutoDetect;

	//document info
	_autoDetectEol = document_->_autoDetectEol;
	_autoDetectIndent = document_->_autoDetectIndent;
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
	if (!isModified())
		return true;
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
                             tr("Cannot save file %1:\n%2.")
                             .arg(fileName_)
                             .arg(file.errorString()));
        return false;
    }

	QTextCodec* codec = QTextCodec::codecForName(_codec.toUtf8());
	if(codec == 0){
		QMessageBox::critical(this, tr("Application"),
					 tr("Cannot save file %1:\nUnsupported charset %2 !!")
					 .arg(fileName_).arg(_codec));
		return false;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);
	//file.resize(0);
	//file.write(codec->fromUnicode(text()));
	QTextStream out(&file);
	out.setCodec(codec);
	out.setGenerateByteOrderMark(needBOM());
	out << text();

	_fullPath = fileName_;
	setModified(false);
	_isNew = false;

    QApplication::restoreOverrideCursor();
    return true;
}
bool DocumentEditor::saveCopy(const QString &fileName_){
	QFile file(fileName_);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot save file %1:\n%2.")
                             .arg(fileName_)
                             .arg(file.errorString()));
        return false;
    }

	QTextCodec* codec = QTextCodec::codecForName(_codec.toUtf8());
	if(codec == 0){
		QMessageBox::critical(this, tr("Application"),
					 tr("Cannot save file %1:\nUnsupported charset %2 !!")
					 .arg(fileName_).arg(_codec));
		return false;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);
	//file.resize(0);
	//file.write(codec->fromUnicode(text()));
	QTextStream out(&file);
	out.setCodec(codec);
	out.setGenerateByteOrderMark(needBOM());
	out << text();
    QApplication::restoreOverrideCursor();
    return true;
}
bool DocumentEditor::saveWithCharset(const QString& codec_){
	_codec = codec_;
    if (_fullPath.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(_fullPath);
    }
}
bool DocumentEditor::saveWithCharsetAs(const QString& codec_){
	_codec = codec_;
	return saveAs();
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

	///@todo charset detection and bom information
	
	QTextCodec* codec = QTextCodec::codecForName(_codec.toUtf8());
	if(codec == 0){
		QMessageBox::critical(this, tr("Application"),
					 tr("Cannot load file %1:\nUnsupported charset %2 !!")
					 .arg(fileName_).arg(_codec));
		return false;
	}

    QApplication::setOverrideCursor(Qt::WaitCursor);

	QString data = codec->toUnicode(file.readAll());
	setText(data);
    	
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
	
	QApplication::restoreOverrideCursor();
	
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


QString DocumentEditor::getCodec() const{
	return _codec;
}

bool DocumentEditor::setCodec(const QString& codec_){
	if(_codec != codec_){
		///@todo it would be nice to change the charset on the fly without reloading the file
		if (isModified()) {
			int ret = QMessageBox::question(this ,
							tr("Reload ?"),
							tr("%1\nThis file has been modified.\nDo you want to reload it with %2 charset\nand loose the change?").arg(getFullPath()).arg(codec_),
							QMessageBox::Yes | QMessageBox::No);
			if (ret == QMessageBox::No)
				return false;
		}

		_codec = codec_;
		if(isNew())
			return false;
		return load(getFullPath());
	}
	return false;
}

void DocumentEditor::setDefaultCodec(const QString& codec_){
	if(_codec.isEmpty()){
		_codec = codec_;
	}
}

void DocumentEditor::setUnicodeBomUseMode(DocumentEditor::UnicodeBomUseMode bomMode_){
	_bomMode = bomMode_;
}

void DocumentEditor::setCharsetAutoDetection(bool autoDetect_){
	_charsetAutoDetect = autoDetect_;
}

bool DocumentEditor::needBOM(){
	switch (_bomMode){
		case BomLeaveAsIs:
			return _hasBom;
		case BomLeaveAsIsAddToNewFile:
			if(isNew())
				return true;
			return _hasBom;
		case BomAlwaysAdd:
			return true;
		case BomAlwaysRemove:
			return false;
		default:
			return false;
	}
}
