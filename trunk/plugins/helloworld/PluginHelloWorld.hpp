#ifndef __PLUGIN_HELLOWORLD_H__
#define __PLUGIN_HELLOWORLD_H__

#include <RaptorPlugin.hpp>

class QString;

class HelloWorld : public RaptorPlugin {
	Q_OBJECT
	Q_INTERFACES(RaptorPlugin)
	
	public:
		HelloWorld();
		virtual ~HelloWorld();
		
		virtual QString getName() const;
	
		virtual QMenu* getMenu(bool& preferedAreaInPluginMenu_) const;
	
	public slots:
		void hello();
	
	private:
		QMenu* _menu;
};

#endif // __PLUGIN_HELLOWORLD_H__
