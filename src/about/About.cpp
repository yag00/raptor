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

#include "About.h"

#include <QFile>
#include <QDesktopServices>
#include <QTabBar>
#include "Version.h"

namespace {
	struct Datas {
		Datas( const QString& n, const QString& l, const QString& p, const QString& e, const QString& c )
			: name(n), login(l), country(p), email(e), comment(c)
		{}

		QString name;
		QString login;
		QString country;
		QString email;
		QString comment;
	};

	const QString mInformations = "<strong>%1 version %2</strong><br/>(Build for %3)<br/><a href=\"%4\">Raptor's Home page</a>";
	//<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">

	const QString mDatasMask =
		"<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\" border=\"0\" style=\"font-size:small;\">"
		"	<tr>"
		"		<td>"
		"			<b>%1</b><br />"
		"			%2 %3"
		"			%4 (<a href=\"%5\">%6</a>)<br />"
		"		</td>"
		"	</tr>"
		"</table>";

	const QList<Datas> mTeamates = QList<Datas>()
		<< Datas( "Christophe Duvernois", "", QObject::tr( "France" ), "christophe.duvernois@gmail.com", QObject::tr( "Creator & Principal Developer" ) );

	const QList<Datas> mLinks = QList<Datas>()
		<< Datas( "", "", QObject::tr( "Location" ), "http://code.google.com/p/raptor/", QObject::tr("Raptor's homepage."))
		<< Datas( "Scintilla is a free source code editing component.", "<br>", QObject::tr( "Location" ), "http://www.scintilla.org/", QObject::tr("Scintilla's homepage."))
		<< Datas( "QScintilla is a port to Qt of Scintilla C++ editor control. ", "<br>", QObject::tr( "Location" ), "http://www.riverbankcomputing.co.uk/software/qscintilla/intro", QObject::tr("QScintilla's homepage."))
		<< Datas( "A Free, Fast and Small Automatic Formatter for C, C++, C#, and Java Source Code", "<br>", QObject::tr( "Location" ), "http://astyle.sourceforge.net/", QObject::tr("Artistic Style's homepage."))
		<< Datas( "Diff Template Libray", "<br>", QObject::tr( "Location" ), "http://code.google.com/p/dtl-cpp/", QObject::tr("dtl's homepage."));

};

AboutDlg::AboutDlg( QWidget * parent, Qt::WindowFlags f) : QDialog(parent, f) {
	setupUi(this);

	// window title
	setWindowTitle( windowTitle().arg( PACKAGE_NAME ) );

	// mouse cursor
	twAbout->findChild<QTabBar*>()->setCursor( Qt::PointingHandCursor );

	// change label color
	QPalette lp( lInformations->palette() );
	lp.setColor( lInformations->backgroundRole(), Qt::white );
	lInformations->setPalette( lp );

	// show informations table
	lInformations->setTextInteractionFlags( Qt::TextBrowserInteraction | Qt::TextSelectableByKeyboard );
	lInformations->setText( mInformations.arg( PACKAGE_NAME ).arg( RaptorVersion.getVersion() ).arg(PACKAGE_OS).arg("http://code.google.com/p/raptor/"));

	// team
	foreach ( Datas i, mTeamates )
		tbTeam->append( mDatasMask.arg( i.comment ).arg( i.name +" -" ).arg( i.login +"<br />" ).arg( i.country ).arg( QString( "mailto:" ).append( i.email ) ).arg( i.email ) );
	tbTeam->moveCursor( QTextCursor::Start );


	// license
	QFile file(":/about/LICENSE.GPL3");
	file.open( QFile::ReadOnly );
	tbLicense->setPlainText( QString::fromUtf8( file.readAll() ) );
	file.close();
	tbLicense->moveCursor( QTextCursor::Start );
	
	// links
	foreach ( Datas i, mLinks )
		tbLinks->append( mDatasMask.arg( i.comment ).arg( i.name ).arg( i.login ).arg( i.country ).arg( i.email ).arg( i.email ) );
	tbLinks->moveCursor( QTextCursor::Start );

	// connections
	foreach ( QTextBrowser* b, twAbout->findChildren<QTextBrowser*>() )
		connect( b, SIGNAL( anchorClicked( const QUrl& ) ), this, SLOT( anchorClicked( const QUrl& ) ) );
}

void AboutDlg::anchorClicked(const QUrl& u) {
	QTextBrowser* b = qobject_cast<QTextBrowser*>( sender() );
	if ( !b )
		return;
	b->setHtml( b->toHtml() );
	QDesktopServices::openUrl( u );
}

#if WAF
#include "About.moc"
#endif

