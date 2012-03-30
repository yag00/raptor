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

#include <QToolButton>
#include <QStyle>
#include "ClearLineEdit.h"

ClearLineEdit::ClearLineEdit(QWidget *parent_) : QLineEdit(parent_) {
    clearButton = new QToolButton(this);
    QPixmap pixmap(":/images/clear-left.png");
    clearButton->setIcon(QIcon(pixmap));
    clearButton->setIconSize(pixmap.size());
    clearButton->setCursor(Qt::ArrowCursor);
    clearButton->setToolTip(tr("Clear text"));
    clearButton->setStyleSheet("QToolButton { border: none; padding: 0px; }");
    clearButton->setAttribute(Qt::WA_NoBackground);
    clearButton->hide();
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clear()));
    connect(this, SIGNAL(textChanged(const QString&)), this, SLOT(updateCloseButton(const QString&)));
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth, 0, this);
    
    setStyleSheet(QString("QLineEdit { padding-right: %1px; } ").arg(clearButton->sizeHint().width() + frameWidth));
    QSize msz = minimumSizeHint();
    setMinimumSize(qMax(msz.width(), clearButton->sizeHint().height() + frameWidth * 2 + 2),
                   qMax(msz.height(), clearButton->sizeHint().height() + frameWidth * 2 + 2));
}

void ClearLineEdit::resizeEvent(QResizeEvent *) {
    QSize sz = clearButton->sizeHint();
    int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
    clearButton->move(rect().right() - frameWidth - sz.width(),
                      (rect().bottom() + 1 - sz.height())/2);
}

void ClearLineEdit::updateCloseButton(const QString& text_) {
    clearButton->setVisible(!text_.isEmpty());
}



#if WAF
#include "ClearLineEdit.moc"
#endif

