#ifndef UIABOUT_H
#define UIABOUT_H

#include <QDialog>
#include "ui_about.h"

struct pDatas
{
	pDatas( const QString& n, const QString& l, const QString& p, const QString& e, const QString& c )
		: Name( n ), Login( l ), Pays( p ), Email( e ), Comment( c )
	{}

	QString Name;
	QString Login;
	QString Pays;
	QString Email;
	QString Comment;
};

class AboutDlg : public QDialog, public Ui::UIAbout
{
Q_OBJECT
public:
	AboutDlg( QWidget * parent = 0, Qt::WFlags f = 0 );

public slots:
	void anchorClicked( const QUrl& );

};
#endif // UIABOUT_H
