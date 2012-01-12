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

#include <QRegExp>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QDomDocument>

#include "../settings/Settings.h"
#include "Version.h"
#include "UpdateChecker.h"

const QString UpdateChecker::_downloadsUrl = "http://code.google.com/feeds/p/raptor/downloads/basic";

UpdateChecker::UpdateChecker(QObject* parent_) : QObject(parent_){
	setObjectName("UpdateChecker");
    QNetworkAccessManager* networkManager = new QNetworkAccessManager(this);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
    
	//Proxy Settings
	Settings settings;
	if(settings.getProxyEnable()){
		QNetworkProxy proxy;
		proxy.setType(QNetworkProxy::HttpProxy);
		proxy.setHostName(settings.getProxyAddress());
		proxy.setPort(settings.getProxyPort());
		if(settings.getProxyAuthentification()){
			proxy.setUser(settings.getProxyUser());
			proxy.setPassword(settings.getProxyPassword());
		}
		networkManager->setProxy(proxy);
	}
	
	//qDebug() << networkManager->proxy().hostName();
	//qDebug() << networkManager->proxy().port();
	
	networkManager->get(QNetworkRequest(QUrl(_downloadsUrl)));
}

UpdateChecker::~UpdateChecker(){

}

void UpdateChecker::finished(QNetworkReply* reply_){
    if(reply_->error() != QNetworkReply::NoError){
		emit error(reply_->errorString());
    }else{
		Version last = RaptorVersion;
        QDomDocument document;
        if(document.setContent(reply_->readAll())){
            const QDomNodeList entries = document.elementsByTagName("entry");
            
            for(int i = 0; i < entries.count(); i++) {
                const QDomElement element = entries.at(i).toElement();
				QDomNodeList nodes = element.elementsByTagName("title");
   
				for (int i = 0; i < nodes.count(); i++) {
					const QDomElement elem = nodes.at(i).toElement();
					QString filename = elem.firstChild().toText().data().trimmed();
					QRegExp rx("raptor-.*([\\d\\.\\d\\.\\d]{1,}).*");
					if(rx.exactMatch(filename)){
						Version found(rx.cap(1));
						if(last < found)
							last = found;
					}
				}
			}
		}
		emit lastVersion(last.getVersion());
	}
}

