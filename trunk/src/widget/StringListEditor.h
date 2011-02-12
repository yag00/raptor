#ifndef __STRING_LIST_EDITOR_H__
#define __STRING_LIST_EDITOR_H__

#include <QGroupBox>

class QVBoxLayout;
class QListWidget;

/*!
	\brief A widget that handle QStringList edition.
*/
class StringListEditor : public QGroupBox {
	Q_OBJECT
	
	public:
		StringListEditor(const QString& title = QString(), QWidget* parent = 0);
		QVBoxLayout* verticalLayout() const;

		void setValues( const QStringList& values );
		QStringList values() const;

	protected:
		QVBoxLayout* _hLayout;
		QListWidget* _list;

	protected slots:
		virtual void onEdited();
		virtual void onAddItem();
		virtual void onRemoveItem();
		virtual void onClearItem();
		virtual void onMoveUpItem();
		virtual void onMoveDownItem();
		virtual void onEditItem();

	signals:
		void edited();
};

#endif // __STRING_LIST_EDITOR_H__
