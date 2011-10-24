#ifndef __EXPLORER_H__
#define __EXPLORER_H__

#include <QWidget>
#include "ui_explorer.h"

class QFileSystemModel;

class Explorer : public QWidget, private Ui::FileExplorer {
	
	Q_OBJECT
	
	public:
		Explorer(QWidget *parent_ = 0);
		~Explorer();

		void navigate(const QString& name_);
		void synchronize(const QString& name_);
		void forceSynchronize(const QString& name_);

	signals:
		void fileClicked(const QString& name_);
		void synchFileRequest();

	protected slots:
		void validate();
		
		void on_lvDirs_activated(const QModelIndex& index_);
		void on_tbUp_clicked();
		void on_tbRoot_clicked();
		void on_tbOpenDir_clicked();
		void on_cbFilter_textChanged(const QString& text_);
		void on_tbSynch_clicked();
		
		void on_actionOpen_triggered();
		void on_actionCopyPath_triggered();
		void on_actionCopyName_triggered();

	protected:
		bool eventFilter(QObject* obj_, QEvent* event_);

	private:
		QFileSystemModel* _model;
};

#endif // __EXPLORER_H__
