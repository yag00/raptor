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

#include <QFileDialog>
#include <QDebug>
#include <QtGui>

#include <Qsci/qsciscintillabase.h>
#include <Qsci/qsciscintilla.h>

#include "document_manager.h"
#include "document_view.h"
#include "document_editor.h"
#include "search.h"

#define INDICATOR_SEARCH          0
#define INDICATOR_QUICK_SEARCH1   1
#define INDICATOR_QUICK_SEARCH2   2

#define ROLE_LINEFROM    Qt::UserRole + 1
#define ROLE_IDXFROM     Qt::UserRole + 2
#define ROLE_LINETO      Qt::UserRole + 3
#define ROLE_IDXTO       Qt::UserRole + 4

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

Search::Search(DocumentManager* documentMgr_, QWidget *parent_) : QWidget(parent_), _documentMgr(documentMgr_){
	//init ui
	setupUi(this);
	//conntect signals
	connect(tbFindNext, SIGNAL(pressed()), this, SLOT(next()));
    connect(tbFindPrev, SIGNAL(pressed()), this, SLOT(prev()));
    connect(tbSearchAll, SIGNAL(pressed()), this, SLOT(searchAll()));
    connect(tbReplace, SIGNAL(pressed()), this, SLOT(replace()));
    connect(tbReplaceAll, SIGNAL(pressed()), this, SLOT(replaceAll()));
	
	connect(tbDirectory, SIGNAL(clicked()), this, SLOT(setSearchDirectory()));	
	
    connect(tbCase, SIGNAL(toggled(bool)), this, SIGNAL(caseSensitiveButtonToggled(bool)));
    connect(tbWrap, SIGNAL(toggled(bool)), this, SIGNAL(wrapButtonToggled(bool)));
    connect(tbWords, SIGNAL(toggled(bool)), this, SIGNAL(wordOnlyButtonToggled(bool)));
    connect(tbRegExp, SIGNAL(toggled(bool)), this, SIGNAL(regexpButtonToggled(bool)));

	
	_domainMenu = new QMenu(this);
	_domainMenu->setObjectName("SearchDomainMenu");
    QAction* actCurrent = _domainMenu->addAction(tr("Whole Document"));
    actCurrent->setData(SD_FILE);
    QAction*  actFiles = _domainMenu->addAction(tr("Open Documents"));
    actFiles->setData(SD_OPENFILES);
    QAction* actAllFiles = _domainMenu->addAction(tr("All Files ..."));
    actAllFiles->setData(SD_ALLFILES);
	
    tbDomain->setMenu(_domainMenu);
	connect(_domainMenu, SIGNAL(triggered(QAction*)), this, SLOT(domainChanged(QAction*)));
	domainChanged(actCurrent);
	
	cbFilter->addItem("*.*");
	cbFilter->addItem("*.h,*.c");
	cbFilter->addItem("*.hpp,*.cpp");
	cbFilter->addItem("*.h,*.hpp,*.c,*.cpp,*.cxx");
	
    tbWrap->setChecked(true);

	twResults->header()->setResizeMode(0, QHeaderView::ResizeToContents);
	twResults->header()->setResizeMode(1, QHeaderView::Stretch);
    twResults->setTextElideMode(Qt::ElideLeft);
	
	//disable button
	on_leFind_textChanged("");
	
	_newSearch = true;
}

Search::~Search(){

}

void Search::setSearchDirectory(){
	QString dir = QFileDialog::getExistingDirectory(this, tr("Search Directory"), _documentMgr->getActiveDocument()->getPath());
	if(!dir.isEmpty()){
		if (cbDirectory->findText(dir) == -1){
			cbDirectory->addItem(dir);
		}
		cbDirectory->setCurrentIndex(cbDirectory->findText(dir));
	}
}

void Search::domainChanged(QAction *act_){
	tbDomain->setText(act_->text());
	_domainMenu->setDefaultAction(act_);
	if(act_->data().toInt() == SD_ALLFILES){
		lbFilter->setVisible(true);
		cbFilter->setVisible(true);
		lbDirectory->setVisible(true);
		cbDirectory->setVisible(true);
		tbDirectory->setVisible(true);
		cbRecursive->setVisible(true);
		if (cbDirectory->findText(_documentMgr->getActiveDocument()->getPath()) == -1)
			cbDirectory->addItem(_documentMgr->getActiveDocument()->getPath());
	}else{
		lbFilter->setVisible(false);
		cbFilter->setVisible(false);
		lbDirectory->setVisible(false);
		cbDirectory->setVisible(false);
		tbDirectory->setVisible(false);
		cbRecursive->setVisible(false);		
	}
}

bool Search::isCaseSensitive() const{
	return tbCase->isChecked();
}
bool Search::isWrap() const{
	return tbWrap->isChecked();
}
bool Search::isWordOnly() const{
	return tbWords->isChecked();
}
bool Search::isRegExp() const{
	return tbRegExp->isChecked();
}

void Search::showMessage(const QString &text_){
	resultLabel->setText(text_);
	leFind->setStyleSheet("");
}

void Search::showMessageNotFound(const QString &text_){
	resultLabel->setText(text_);
	leFind->setStyleSheet("QLineEdit { background-color: #ff9999; }");
}

void Search::showSearchResults(bool found_) {
	if (!found_){
		showMessageNotFound(tr("Search string not found"));
		_newSearch = true;
	}else{
		clearMessage();
		_newSearch = false;
	}
}

void Search::clearMessage(){
	resultLabel->clear();
	leFind->setStyleSheet("");
}

void Search::clearResults(){
  twResults->clear();
  _lastSearchFile = "";
  _lastSearchItem = 0;
}

void Search::on_leFind_textChanged(const QString& text_){
	_lastSearch = text_;
	bool on = text_.length();
	tbFindNext->setEnabled(on);
	tbFindPrev->setEnabled(on);
	tbSearchAll->setEnabled(on);
	tbReplace->setEnabled(on);
	tbReplaceAll->setEnabled(on);
	tbReplaceAll->setEnabled(on);
	if(on){
		_newSearch = true;
		searchIncremental();
	}else{
		leFind->setStyleSheet("");
	}
	leFind->setFocus();
}

void Search::setVisible(bool visible_){
	QWidget::setVisible(visible_);
	if(visible_){
		QString text = _documentMgr->getActiveDocument()->selectedText();
		if(text.isEmpty())
			leFind->setText(_lastSearch);
		else{
			leFind->setText(text);
		}
		leFind->selectAll();
		leFind->setFocus();
	}
}

void Search::next(){
	doSearch(true);
}
void Search::prev(){
	doSearch(false);
}
void Search::searchAll(){
	doSearchReplaceAll(false);
}
void Search::replace(){
	_documentMgr->getActiveDocument()->replace(leReplace->text());
	doDocumentSearch(true, true);
}
void Search::replaceAll(){
	doSearchReplaceAll(true);
}

void Search::searchIncremental() {
	_documentMgr->getActiveDocument()->clearIndicators(0);
	doDocumentSearch(true, false);
}

void Search::doSearch(bool fw_) {
	if (!this->isVisible()){
		doSimpleSearch(fw_);
		return;
	}

	doDocumentSearch(fw_, fw_);
}

void Search::doSimpleSearch(bool fw_) {
	int line = -1, index = -1;
	QString text = _documentMgr->getActiveDocument()->selectedText();
	// check forward search
	if (!fw_ && text.length()) {
		int pos = _documentMgr->getActiveDocument()->SendScintilla(QsciScintillaBase::SCI_GETCURRENTPOS);
		pos -= text.toUtf8().length();
		_documentMgr->getActiveDocument()->lineIndexFromPosition(pos, &line, &index);
	}

	_documentMgr->getActiveDocument()->findFirst(text, false, true, true, true, fw_, line, index);
}

void Search::doDocumentSearch(bool fw_, bool fix_) {
	int line = -1, index = -1;
	int lineFrom, lineTo, indexFrom, indexTo;
	_documentMgr->getActiveDocument()->getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);
	
	// check forward search
	if (!fix_ && leFind->text().length()) {
		int pos = _documentMgr->getActiveDocument()->SendScintilla(QsciScintillaBase::SCI_GETCURRENTPOS);
		pos -= leFind->text().toUtf8().length();
		_documentMgr->getActiveDocument()->lineIndexFromPosition(pos, &line, &index);
	}
	
	bool res = _documentMgr->getActiveDocument()->findFirst(leFind->text(), isRegExp(), isCaseSensitive(), isWordOnly(), isWrap(), fw_, line, index);

	if(_newSearch == false && res == false){
		if(fw_ == true)
			showMessageNotFound(tr("Search string not found, reach end of file"));
		else
			showMessageNotFound(tr("Search string not found, reach beginning of file"));
	}
	else{
		_newSearch = !res;
		showSearchResults(res);
	}
}

void Search::doSearchReplaceAll(bool replace_){
	//qDebug() << "doSearchReplaceAll()";

	//clear indicators
	foreach (DocumentEditor* document, _documentMgr->getDocuments()){
		document->clearIndicators(INDICATOR_SEARCH);
	}

	//clear results
	clearResults();

	if (leFind->text().isEmpty()){
		return;
	}

	int count = 0;

	// check if file search
	switch((SearchDomain)_domainMenu->defaultAction()->data().toInt()){
		case SD_OPENFILES:
			count = doOpenFilesSearch(replace_);
			break;
		case SD_ALLFILES:
			count = doAllFilesSearch(replace_);
			break;
		default:
			count = doFileSearch(_documentMgr->getActiveDocument(), replace_);
			break;
	}

	if (!count)
		showSearchResults(false);
	else {
		if (replace_)
			showMessage(tr("%1 entries replaced").arg(count));
		else
			showMessage(tr("Search finished, %1 entries found").arg(count));

		for(int i = 0; i < twResults->topLevelItemCount(); i++){
			QTreeWidgetItem* item = twResults->topLevelItem(i);
			item->setExpanded(true);
			QString text = item->text(0);
			text += tr(" (%1 hit)").arg(item->childCount());
			item->setText (0, text);
		}
		_newSearch = true;
	}
}

int Search::doFileSearch(DocumentEditor *document_, bool replace_){
	::TextToFind ttf;

	int count = 0;
	int last;
	int lineFrom, lineTo, indexFrom, indexTo;
	
	if(_newSearch){
		_lastSearchFile = "";
		_lastSearchItem = 0;
		document_->getSelection(&lineFrom, &indexFrom, &lineTo, &indexTo);
	}

	// clean markup
	document_->clearIndicators(INDICATOR_SEARCH);
	
	// check domain
	ttf.chrg.cpMin = 0;
	last = ttf.chrg.cpMax = document_->length();

	// search
	int flags = 0;
	if (isCaseSensitive()) flags |= QsciScintillaBase::SCFIND_MATCHCASE;
	if (isWordOnly()) flags |= QsciScintillaBase::SCFIND_WHOLEWORD;
	if (isRegExp()) flags |= QsciScintillaBase::SCFIND_REGEXP;

	document_->beginUndoAction();

	int res, end;
	QByteArray ba = leFind->text().toUtf8();
	QByteArray ra = leReplace->text().toUtf8();
	do {
		ttf.lpstrText = ba.data();
		res = document_->SendScintilla(QsciScintillaBase::SCI_FINDTEXT, flags, &ttf);
		if (res < 0)
			break;

		if (ttf.chrgText.cpMax > last)
			break;

		count++;

		if (replace_){
			document_->SendScintilla(QsciScintillaBase::SCI_SETTARGETSTART, ttf.chrgText.cpMin);
			document_->SendScintilla(QsciScintillaBase::SCI_SETTARGETEND, ttf.chrgText.cpMax);

			document_->SendScintilla(QsciScintillaBase::SCI_REPLACETARGET, -1, ra.data());

			// target set to new
			end = document_->SendScintilla(QsciScintillaBase::SCI_GETTARGETEND);
			ttf.chrg.cpMin = end;
			ttf.chrg.cpMax = last;  // can be shorter or longer than initial
		}else{
			ttf.chrg.cpMin = ttf.chrgText.cpMax+1;
			end = ttf.chrgText.cpMax;
		}

		// markup
		document_->applyIndicator(ttf.chrgText.cpMin, end, INDICATOR_SEARCH);

		// results
		document_->lineIndexFromPosition(ttf.chrgText.cpMin, &lineFrom, &indexFrom);
		document_->lineIndexFromPosition(end, &lineTo, &indexTo);
		addResult(document_->getFullPath(), document_->text(lineFrom), lineFrom, lineTo, indexFrom, indexTo);

	} while (res >= 0);

	document_->endUndoAction();
	
	return count;
}

int Search::doOpenFilesSearch(bool replace_){
	int count = 0;
	foreach (DocumentEditor* document, _documentMgr->getDocuments()){
		count += doFileSearch(document, replace_);
	}
	return count;
}

int Search::doAllFilesSearch(bool replace_){
	if(replace_){
		int ret = QMessageBox::warning(this, tr("Search&Replace"),
					tr("You are going to do a replace operation on all files in %1.\n"
					"This can not be undo. Do you want to continue?").arg(cbDirectory->currentText()),
					QMessageBox::Yes,
					QMessageBox::No,
					QMessageBox::Cancel | QMessageBox::Escape | QMessageBox::Default);
		if (ret != QMessageBox::Yes)
			return 0;
	}
	
	int count = 0;

	QStringList files = find();
	
	QProgressDialog progressDialog(this);
	progressDialog.setCancelButtonText(tr("&Cancel"));
	progressDialog.setRange(0, files.size());
	if(replace_)
		progressDialog.setWindowTitle(tr("Replace In Files ..."));
	else
		progressDialog.setWindowTitle(tr("Search In Files ..."));
	
	for(int i = 0; i < files.count(); i++){
		progressDialog.setValue(i);
		progressDialog.setLabelText(tr("Searching file number %1 of %2...\n%3").arg(i).arg(files.size()).arg(files[i]));
		qApp->processEvents();

		if (progressDialog.wasCanceled())
			break;
			
		bool deleteAfter = false;
		DocumentEditor* document = _documentMgr->getDocument(files[i]);
		if(document == 0){
			document = new DocumentEditor(_documentMgr->getFileSystemWatcher());
			document->load(files[i]);
			deleteAfter = true;
		}
			
		count += doFileSearch(document, replace_);
		
		if(deleteAfter){
			if(document->isModified()){
				document->save();
			}
			delete document;
		}
	}
	
	return count;
}

void Search::updateComboBox(QComboBox *comboBox_) {
	if (comboBox_->findText(comboBox_->currentText()) == -1)
		comboBox_->addItem(comboBox_->currentText());
}

QStringList Search::find() {
	QString filter = cbFilter->currentText();
	QString path = cbDirectory->currentText();

	updateComboBox(cbFilter);
	updateComboBox(cbDirectory);

	bool recursive = cbRecursive->isChecked();
	return findFiles(path, recursive);
}

QStringList Search::findFiles(QString directory_, bool recursive_){
	QStringList files;
	QStringList filter = cbFilter->currentText().split(",");
	
	QDir currentDir = QDir(directory_);
	QDir::Filters flags;
	if(recursive_)
		flags = QDir::AllDirs | QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot;
	else
		flags = QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot;
		
	QFileInfoList list = currentDir.entryInfoList(filter, flags);
	for (int i = 0; i< list.count(); i++){
		QFileInfo info = list[i];
		if (info.isDir()){
			files += findFiles(info.filePath(), recursive_);
		}else{
			files += info.filePath();
		}
	}	
	return files;
}

void Search::addResult(const QString &file_, const QString &text_, int lineFrom_, int lineTo_, int indexFrom_, int indexTo_){
	if(file_ != _lastSearchFile || _lastSearchItem == 0){
		_lastSearchFile = file_;
		_lastSearchItem = new QTreeWidgetItem(twResults);
		_lastSearchItem->setText(0, file_);
		_lastSearchItem->setData(0, Qt::UserRole + 1,  file_);
		_lastSearchItem->setBackgroundColor(0, QColor(213,255,213));
		//twResults->setFirstItemColumnSpanned(_lastSearchItem, true);
		//_lastSearchItem->setIcon(0, QIcon(":/images/ledblue.png"));
		_lastSearchItem->setFirstColumnSpanned(true);
		twResults->addTopLevelItem(_lastSearchItem);
	}
	
	QTreeWidgetItem *item = new QTreeWidgetItem(_lastSearchItem);
	item->setText(0, QString("%1").arg(lineFrom_+1));
	item->setData(0, ROLE_IDXFROM,  indexFrom_);
	item->setData(0, ROLE_IDXTO, indexTo_);
	item->setData(0, ROLE_LINEFROM, lineFrom_);
	item->setData(0, ROLE_LINETO, lineTo_);
	QString line(text_);
	line.remove("\n");
	line.remove("\r");
	item->setText(1, line);	
	_lastSearchItem->addChild(item);
}

void Search::on_twResults_activated(const QModelIndex & index_){
	if (!index_.isValid())
		return;
	if(index_.parent().model() == 0)
		return;
	
	QTreeWidgetItem *item = (QTreeWidgetItem*)index_.internalPointer();
	
	int lineFrom, lineTo, indexFrom, indexTo;
	lineFrom = item->data(0, ROLE_LINEFROM).toInt();
	indexFrom = item->data(0, ROLE_IDXFROM).toInt();
	lineTo = item->data(0, ROLE_LINETO).toInt();
	indexTo = item->data(0, ROLE_IDXTO).toInt();
	
	QString fileName =  ((QTreeWidgetItem*)(index_.parent().internalPointer()))->data(0, Qt::UserRole + 1).toString();
	// check if such a file opened already
	DocumentEditor* document = _documentMgr->getDocument(fileName);
	
	if(document != 0){
		_documentMgr->setActiveDocument(document);
	}else{
		// open the file, it will switch automatically
		_documentMgr->open(fileName);
		document = _documentMgr->getActiveDocument();
	}

	if (lineTo >= document->lines())
		return;

	document->setCursorPosition(lineFrom, indexFrom);
	document->setSelection(lineFrom, indexFrom, lineTo, indexTo);
	document->ensureLineVisible(lineFrom);
	document->ensureCursorVisible();
}

void Search::on_tbCollapseResult_pressed(){
	if(twResults->topLevelItemCount() == 0)
		return;
	bool isExpended = twResults->topLevelItem(0)->isExpanded();
	for(int i = 0; i < twResults->topLevelItemCount(); i++){
		QTreeWidgetItem* item = twResults->topLevelItem(i);
		item->setExpanded(!isExpended);
	}
}

bool Search::event(QEvent *event_){
	if(event_->type() == QEvent::ShortcutOverride){
		if( ((QKeyEvent*)event_)->key() == Qt::Key_Tab){
			QWidget* old = this->focusWidget();
			focusNextChild();
			if(old == this->focusWidget()){
				leFind->setFocus();
			}
		}else if( ((QKeyEvent*)event_)->key() == Qt::Key_Backtab){
			QWidget* old = this->focusWidget();
			focusPreviousChild();
			if(old == this->focusWidget()){
				twResults->setFocus();
			}
		}
		return true;
	}
	return QWidget::event(event_);
}

void Search::contextMenuEvent(QContextMenuEvent *event_){
	if(twResults->x() <= event_->x() && twResults->y() <= event_->y()){
		QMenu contextMenu;
		QAction *clear = contextMenu.addAction(tr("Clear Results"));
		clear->setIcon(QIcon(":/images/trashcan_empty.png"));
		connect(clear, SIGNAL(triggered()), this, SLOT(clearResults()));
		QAction *collapseExpand = contextMenu.addAction(tr("Collapse/Expand Results"));
		collapseExpand->setIcon(QIcon(":/images/closepage.png"));
		connect(collapseExpand, SIGNAL(triggered()), this, SLOT(on_tbCollapseResult_pressed()));	
		contextMenu.exec(QCursor::pos());
	}
}

#if WAF
#include "search.moc"
#endif

