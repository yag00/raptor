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

#include <Qsci/qscilexer.h>
#include <Qsci/qscilexerdiff.h>

#include "../settings/Settings.h"
#include "UnidiffDialog.h"

UnidiffDialog::UnidiffDialog(const QString& diff_, QWidget* parent_) : QDialog(parent_) {
	//init ui
	setupUi(this);
	diffEditor->setUtf8(true);
	diffEditor->setText(diff_);
	diffEditor->setReadOnly(true);
	QsciLexer* lexer = new QsciLexerDiff(diffEditor);
	diffEditor->setLexer(lexer);
	//load lexer settings
	Settings settings;
	lexer->readSettings(settings);
	lexer->refreshProperties();

	connect((QObject*)buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(ok()));
	connect((QObject*)buttonBox->button(QDialogButtonBox::Save), SIGNAL(clicked()), this, SLOT(save()));
	connect((QObject*)buttonBox->button(QDialogButtonBox::Open), SIGNAL(clicked()), this, SLOT(open()));
}

UnidiffDialog::~UnidiffDialog(){

}

void UnidiffDialog::ok(){
	setResult(UnidiffDialog::Ok);
}

void UnidiffDialog::open(){
	setResult(UnidiffDialog::Open);
}

void UnidiffDialog::save(){
	setResult(UnidiffDialog::Save);
}
