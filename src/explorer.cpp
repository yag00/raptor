#include <QFileDialog>
#include <QFileSystemModel>
#include <QKeyEvent>
#include <QLineEdit>
#include <QCompleter>
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
					   this, tr("Choose directory:"),
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
