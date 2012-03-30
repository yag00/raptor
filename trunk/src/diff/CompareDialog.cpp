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
#include <QPushButton>
#include "CompareDialog.h"

CompareDialog::CompareDialog(QWidget* parent_) : QDialog(parent_) {
	//init ui
	setupUi(this);
	src1CheckLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/stop.png")));
	src2CheckLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/stop.png")));
	_src1Ok = false;
	_src2Ok = false;
	checkOk();
}

CompareDialog::~CompareDialog(){

}

QString CompareDialog::getSrcFile1() const{
	return src1LineEdit->text();
}

QString CompareDialog::getSrcFile2() const{
	return src2LineEdit->text();
}

void CompareDialog::checkOk(){
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled((_src1Ok && _src2Ok));
}

void CompareDialog::on_src1LineEdit_textChanged(const QString& text_){
	QFileInfo f(text_);
	if(f.exists() && f.isFile()){
		_src1Ok = true;
		src1CheckLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/tick.png")));
	}else{
		_src1Ok = false;
		src1CheckLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/stop.png")));
	}
	checkOk();
}

void CompareDialog::on_src2LineEdit_textChanged(const QString& text_){
	QFileInfo f(text_);
	if(f.exists() && f.isFile()){
		_src2Ok = true;
		src2CheckLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/tick.png")));
	}else{
		_src2Ok = false;
		src2CheckLabel->setPixmap(QPixmap(QString::fromUtf8(":/images/stop.png")));	
	}
	checkOk();
}

void CompareDialog::on_src1ToolButton_clicked(){
	QString name = QFileDialog::getOpenFileName(this, tr("Choose source file 1:"));
	if (!name.isEmpty()) {
		src1LineEdit->setText(name);
	}
}

void CompareDialog::on_src2ToolButton_clicked(){
	QString name = QFileDialog::getOpenFileName(this, tr("Choose source file 2:"));
	if (!name.isEmpty()) {
		src2LineEdit->setText(name);
	}	
}

#if WAF
#include "CompareDialog.moc"
#endif

