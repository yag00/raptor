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

#include <PythonQt.h>
#include "ui_pluginabout.h"
#include "PyPlugin.h"
#include "PluginSettings.h"
#include "PluginElement.h"

PluginElement::PluginElement(PyPlugin& plugin_, QWidget* parent_) : 
	QWidget(parent_), _plugin(plugin_) {
	//init ui
	setupUi(this);
		
	//init element
	const QString info1 = "<strong>%1</strong> - version %2";
	linfoLine1->setText(info1.arg(_plugin.getName()).arg(_plugin.getVersion()));
	linfoLine2->setText(_plugin.getShortDescription());
	
	bool enable = _plugin.isEnabled();
	cbEnabled->setChecked(enable);
	lIcon->setEnabled(enable);
	linfoLine1->setEnabled(enable);
	linfoLine2->setEnabled(enable);
		
	//connection
	connect(pbAbout, SIGNAL(clicked()), this, SLOT(about()));
	connect(cbEnabled, SIGNAL(toggled(bool)), this, SLOT(enable(bool)));
}

PluginElement::~PluginElement(){

}

void PluginElement::about(){
	QDialog dlg(this);
	Ui::PluginAbout ui;
	ui.setupUi(&dlg);
	ui.lName->setText(QString("<strong>%1</strong>").arg(_plugin.getName()));
	ui.lVersion->setText("version : " + _plugin.getVersion());
	ui.lAuthor->setText(_plugin.getAuthor());
	ui.lLicense->setText(_plugin.getLicense());
	ui.description->setText(_plugin.getDescription());
	dlg.exec();
}

void PluginElement::enable(bool checked_){
	lIcon->setEnabled(checked_);
	linfoLine1->setEnabled(checked_);
	linfoLine2->setEnabled(checked_);
	_plugin.setEnabled(checked_);
}

#if WAF
#include "PluginElement.moc"
#endif
