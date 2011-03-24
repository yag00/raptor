#include "About.h"

#include <QFile>
#include <QDesktopServices>
#include <QTabBar>

const QString mInformations =
"<table width=\"100%\" cellpadding=\"0\" cellspacing=\"0\" border=\"0\" style=\"font-size:small;\">"
"	<tr>"
"		<td align=\"center\"><br /><img src=\":/about/images/about.png\" width=\"32\" height=\"32\"></td>"
"		<td><strong>%1<strong> version %2<br />%3<br /><br /><a href=\"%4\">Home page</a></td>"
"	</tr>"
"</table>";

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

const QList<pDatas> mTeamates = QList<pDatas>()
	<< pDatas( "Christophe Duvernois", "", QObject::tr( "France" ), "christophe.duvernois@gmail.com", QObject::tr( "Creator & Principal Developer" ) );

const QList<pDatas> mDonors = QList<pDatas>();
	
const QList<pDatas> mLinks = QList<pDatas>()
	<< pDatas( "", "", QObject::tr( "Location" ), "http://code.google.com/p/raptor/", QObject::tr("Raptor's homepage."))
	<< pDatas( "Scintilla is a free source code editing component.", "<br>", QObject::tr( "Location" ), "http://www.scintilla.org/", QObject::tr("Scintilla's homepage."))
	<< pDatas( "QScintilla is a port to Qt of Scintilla C++ editor control. ", "<br>", QObject::tr( "Location" ), "http://www.riverbankcomputing.co.uk/software/qscintilla/intro", QObject::tr("QScintilla's homepage."))
	<< pDatas( "A Free, Fast and Small Automatic Formatter for C, C++, C#, and Java Source Code", "<br>", QObject::tr( "Location" ), "http://astyle.sourceforge.net/", QObject::tr("Artistic Style's homepage."));
	
	
AboutDlg::AboutDlg( QWidget * parent, Qt::WFlags f) 
	: QDialog(parent, f)
{
	setupUi(this);
	//setAttribute( Qt::WA_DeleteOnClose );
	
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
	lInformations->setText( mInformations.arg( PACKAGE_NAME ).arg( PACKAGE_VERSION ).arg( PACKAGE_DESCRIPTION ).arg("http://code.google.com/p/raptor/"));

	// team
	foreach ( pDatas i, mTeamates )
		tbTeam->append( mDatasMask.arg( i.Comment ).arg( i.Name +" -" ).arg( i.Login +"<br />" ).arg( i.Pays ).arg( QString( "mailto:" ).append( i.Email ) ).arg( i.Email ) );
	tbTeam->moveCursor( QTextCursor::Start );

	
	// license	
	QFile file(":/about/rc/LICENSE.GPL3");
	file.open( QFile::ReadOnly );
	tbLicense->setPlainText( QString::fromUtf8( file.readAll() ) );
	file.close();
	tbLicense->moveCursor( QTextCursor::Start );
	
	// donors
	foreach ( pDatas i, mDonors )
		tbDonations->append( mDatasMask.arg( i.Comment ).arg( i.Name +" -" ).arg( i.Login +"<br />" ).arg( i.Pays ).arg( QString( "mailto:" ).append( i.Email ) ).arg( i.Email ) );
	tbDonations->moveCursor( QTextCursor::Start );

	// links
	foreach ( pDatas i, mLinks )
		tbLinks->append( mDatasMask.arg( i.Comment ).arg( i.Name ).arg( i.Login ).arg( i.Pays ).arg( i.Email ).arg( i.Email ) );
	tbLinks->moveCursor( QTextCursor::Start );

	// connections
	foreach ( QTextBrowser* b, twAbout->findChildren<QTextBrowser*>() )
		connect( b, SIGNAL( anchorClicked( const QUrl& ) ), this, SLOT( anchorClicked( const QUrl& ) ) );
}

void AboutDlg::anchorClicked( const QUrl& u )
{
	QTextBrowser* b = qobject_cast<QTextBrowser*>( sender() );
	if ( !b )
		return;
	b->setHtml( b->toHtml() );
	QDesktopServices::openUrl( u );
}
