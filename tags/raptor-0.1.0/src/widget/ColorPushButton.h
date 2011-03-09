#ifndef __COLOR_BUTTON_H__
#define __COLOR_BUTTON_H__

#include <QPushButton>

class ColorPushButton : public QPushButton {
	Q_OBJECT
	Q_PROPERTY( bool colorNameHidden READ isColorNameHidden WRITE setColorNameHidden )
	Q_PROPERTY( QColor color READ color WRITE setColor )
	
	public:
		ColorPushButton(QWidget* parent = 0);
		ColorPushButton(const QColor& color, QWidget* parent = 0);
		virtual ~ColorPushButton();
		
		bool isColorNameHidden() const;
		const QColor& color() const;
	
	public slots:
		void setState(int state_);
		void setColorNameHidden(bool hidden);
		void setColor(const QColor& color);
	
	protected:
		void init(const QColor& color);
		void updateColorName();
	
	protected slots:
		void onClicked();
		
	private:
		bool _isColorNameHidden;
		QColor _color;
};

#endif // __COLOR_BUTTON_H__
