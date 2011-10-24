#include <QFileDialog>
#include <QFileSystemModel>
#include <QKeyEvent>
#include <QLineEdit>
#include <QCompleter>
#include <QClipboard>
#include "explorer.h"

Explorer::Explorer(QWidget *parent_) : QWidget(parent_) {
	setupUi(this);
	setObjectName("Explorer");

	layout()->setMargin(1);

	_model = new QFileSystemModel(this);
	_model->setRootPath("");
	_model->setNameFilterDisables(false);

	//_model->setFilter(QDir::AllDirs | QDir::Files | QDir::Drives);

	lvDirs->setModel(_model);
	lvDirs->setMouseTracking(true);
	lvDirs->installEventFilter(this);

	connect(tbSynchReq, SIGNAL(clicked()), this, SIGNAL(synchFileRequest()));
	connect(cbDir->lineEdit(), SIGNAL(returnPressed()), this, SLOT(validate()));

	QCompleter *comp = new QCompleter(this);
	comp->setCompletionMode(QCompleter::InlineCompletion);
	cbFilter->setCompleter(comp);

	QCompleter *comp2 = new QCompleter(this);
	comp2->setCompletionMode(QCompleter::InlineCompletion);
	cbDir->setCompleter(comp2);

	lvDirs->addAction(actionOpen);
	QAction *sep = new QAction(this); 
	sep->setSeparator(true);
	lvDirs->addAction(sep);
	lvDirs->addAction(actionCopyPath);
	lvDirs->addAction(actionCopyName);
	
	navigate(QDir::rootPath());
}

Explorer::~Explorer() {

}

void Explorer::navigate(const QString& name_) {
	QModelIndex index = _model->setRootPath(name_);
	lvDirs->setRootIndex(index);
	cbDir->lineEdit()->setText(name_);
}

void Explorer::synchronize(const QString& name_) {
	if(tbSynch->isChecked()){
		forceSynchronize(name_);
	}
}

void Explorer::forceSynchronize(const QString& name_) {
	QFileInfo fi(name_);
	QString path = fi.canonicalPath();
	navigate(path);
}

void Explorer::on_tbSynch_clicked() {
	if (tbSynch->isChecked())
		emit synchFileRequest();
}

void Explorer::on_lvDirs_activated(const QModelIndex& index_) {
	QString path = _model->filePath(index_);

	if (_model->isDir(index_)) {
		navigate(path);
		return;
	}

	emit fileClicked(path);
}

void Explorer::validate() {
	QString name = cbDir->currentText();
	QFileInfo fi(name);
	if (fi.exists()){
		navigate(fi.canonicalPath());
	}else{
		navigate(QDir::rootPath());
	}
}

void Explorer::on_tbUp_clicked() {
	QModelIndex old = lvDirs->rootIndex();
	QModelIndex index = _model->parent(lvDirs->rootIndex());
	if (index.isValid()) {
		navigate(_model->filePath(index));
		lvDirs->setCurrentIndex(old);
	} else {
		navigate(QDir::rootPath());
	}
}

void Explorer::on_tbRoot_clicked() {
	navigate(QDir::rootPath());
}

void Explorer::on_tbOpenDir_clicked() {
	QString name = QFileDialog::getExistingDirectory(
					   this, tr("Path to the directory to browse for:"),
					   _model->filePath(lvDirs->currentIndex()));

	if (!name.isEmpty()) {
		navigate(name);
		cbDir->lineEdit()->selectAll();
	}
}



bool Explorer::eventFilter(QObject* obj_, QEvent* event_) {
	if (event_->type() == QEvent::KeyPress) {
		if (obj_ == lvDirs) {
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event_);
			switch (keyEvent->key()) {
				case Qt::Key_Backspace:
					on_tbUp_clicked();
					return true;
				case Qt::Key_Home:
					if (keyEvent->modifiers() == Qt::ControlModifier) {
						on_tbRoot_clicked();
						return true;
					}
			}
		}
	}

	return QObject::eventFilter(obj_, event_);
}

void Explorer::on_cbFilter_textChanged(const QString& text_) {
	_model->setNameFilters(text_.split(";", QString::SkipEmptyParts));
}

void Explorer::on_actionOpen_triggered() {
	on_lvDirs_activated(lvDirs->currentIndex());
}

void Explorer::on_actionCopyPath_triggered() {
	QString name(_model->filePath(lvDirs->currentIndex()));
	if (!name.isEmpty()) {
		QApplication::clipboard()->setText(name);
	}
}

void Explorer::on_actionCopyName_triggered() {
	QString name(_model->filePath(lvDirs->currentIndex()));
	if (!name.isEmpty()) {
		QApplication::clipboard()->setText(QFileInfo(name).fileName());
	}
}
