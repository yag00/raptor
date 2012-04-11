#ifndef __RAPTOR_PLUGIN_H__
#define __RAPTOR_PLUGIN_H__

#include <QtPlugin>
#include <QDockWidget>

class QString;
class QMenu;
class QDockWidget;

class RaptorPlugin : public QObject {
	Q_OBJECT

	public:
		virtual ~RaptorPlugin(){}
		
		/** @return the name of the plugin */
		virtual QString getName() const = 0;
		
		/** the plugin menu
		 * @param preferedAreaInPluginMenu_ prefered menu area
		 * @return the plugin menu or null */
		virtual QMenu* getMenu(bool& preferedAreaInPluginMenu_) const {
			return 0;
		}
		/** the plugin dock
		 * @param preferedArea_ prefered dock area
		 * @return the plugin dock or null */
		virtual QDockWidget* getDock(Qt::DockWidgetArea& preferedArea_) const {
			return 0;
		}

	protected:
		
};

Q_DECLARE_INTERFACE(RaptorPlugin, "raptor.RaptorPlugin")

#endif // __RAPTOR_PLUGIN_H__
