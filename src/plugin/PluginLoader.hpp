#ifndef __PLUGIN_LOADER_H__
#define __PLUGIN_LOADER_H__

#include <QObject>
#include <QPluginLoader>
#include <QList>
#include <QString>
#include <QDir>

namespace PluginLoader {

    template<typename T> T* pluginByName(const QString& fileName) {
        QPluginLoader loader(fileName);
        QObject *plugin = loader.instance();
        return qobject_cast<T*>(plugin);
    }

    template<typename T> QList<T*> pluginByDir(const QString& dir) {
        QList<T*> ls;
        QDir plugDir = QDir(dir);
        foreach(QString file, plugDir.entryList(QDir::Files)) {
            if(T* plugin = PluginLoader::pluginByName<T>(plugDir.absoluteFilePath(file))){
                ls.push_back(plugin);
            }
        }
        return ls;
    }
}

#endif // __PLUGIN_LOADER_H__
