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

#ifndef __SYMBOL_COMBO_BOX_ACTION_H__
#define __SYMBOL_COMBO_BOX_ACTION_H__

#include <QWidgetAction>

class QAbstractItemModel;

class SymbolComboBoxAction : public QWidgetAction {
	Q_OBJECT

	public:
		SymbolComboBoxAction(QAbstractItemModel* model_, QObject* parent_ = 0);
		virtual ~SymbolComboBoxAction();

	signals:
		//void symbolActivated(const QString& name_, int line_);
		void symbolActivated(int line_);
		
	protected:
		virtual QWidget* createWidget(QWidget* parent_);
	
	private:
		QAbstractItemModel* _model;

};


#endif /* __SYMBOL_COMBO_BOX_ACTION_H__ */
