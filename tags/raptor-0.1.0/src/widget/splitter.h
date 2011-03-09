#ifndef __CUSTOM_SPLITTER_H__
#define __CUSTOM_SPLITTER_H__

#include <QSplitter>

class Splitter : public QSplitter
{
    Q_OBJECT
	public:	
		Splitter(QWidget * parent = 0);
		Splitter(Qt::Orientation orientation, QWidget * parent = 0);
	
	private slots:
		void rotate();
		void swap();
		
	protected:
		void contextMenuEvent(QContextMenuEvent *event);
	

};

#endif /* __CUSTOM_SPLITTER_H__ */
