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

#include <QByteArray>
#include <QDir>
#include <QLibraryInfo>
#include <QProcess>

#include <QHelpEngine>
#include <QHelpContentModel>
#include <QHelpContentWidget>
#include <QHelpIndexModel>
#include <QHelpIndexWidget>
#include <QTextBrowser>
#include <QSplitter>

#include <QMessageBox>
#include <QHBoxLayout>
#include <QDebug>

#include "Version.h"
#include "ApplicationPath.h"
#include "HelpBrowser.h"

#ifdef NO_ASSISTANT
namespace {
    class qthelpBrowser : public QTextBrowser {
        public:
            qthelpBrowser(QHelpEngine *helpEngine_, QWidget *parent_ = 0);
            QVariant loadResource(int type, const QUrl &url);

        private:
            QHelpEngine *_helpEngine;
    };

    qthelpBrowser::qthelpBrowser(QHelpEngine *helpEngine_, QWidget *parent_) : QTextBrowser(parent_), _helpEngine(helpEngine_){
    }

    QVariant qthelpBrowser::loadResource(int type, const QUrl &url){
        if (url.scheme() == "qthelp")
            return QVariant(_helpEngine->fileData(url));
        else
            return QTextBrowser::loadResource(type, url);
    }

    class HelpBrowserDialog : public QDialog {
        public:
            HelpBrowserDialog(QWidget *parent=0) : QDialog(parent) {
                QHBoxLayout* hLayout = new QHBoxLayout(this);
                QSplitter* splitter = new QSplitter(Qt::Horizontal, this);
                hLayout->addWidget(splitter);

                QString docPath = HelpBrowser::getDocumentationFile();

                QHelpEngine* helpEngine = new QHelpEngine(docPath, splitter);
                helpEngine->setupData();
                qthelpBrowser *helpBrowser = new qthelpBrowser(helpEngine, splitter);

                //helpEngine->contentWidget()->setExpanded(helpEngine->indexModel(), true);
                splitter->addWidget(helpEngine->contentWidget());
                splitter->addWidget(helpBrowser);
                splitter->setStretchFactor(1, 1);

                helpBrowser->setSource(HelpBrowser::getDocumentationNamespace() + "index.html");

                connect(helpEngine->contentWidget(), SIGNAL(linkActivated(const QUrl &)), helpBrowser, SLOT(setSource(const QUrl &)));
                setModal(false);
            }
    };


};
#endif

HelpBrowser::HelpBrowser(QObject * parent_) : QObject(parent_), _process(0) {
#ifdef NO_ASSISTANT
    _helpDlg = 0;
#endif
}

HelpBrowser::~HelpBrowser() {
    if (_process && _process->state() == QProcess::Running) {
        _process->terminate();
        _process->waitForFinished(3000);
        delete _process;
    }
#ifdef NO_ASSISTANT
    if(_helpDlg)
        delete _helpDlg;
#endif
}

QString HelpBrowser::getDocumentationFile(){
    return ApplicationPath::documentationPath() +  "Raptor."+ ApplicationPath::applicationLanguage() + ".qhc";
}
QString HelpBrowser::getDocumentationNamespace(){
    QString source = "qthelp://org." + ApplicationPath::applicationLanguage();
    source += ".raptor." + RaptorVersion.getVersionXY() + "/doc/";
    //qDebug() << source;
    return source;
}

void HelpBrowser::showDocumentation(const QString &page_) {
    if (!startAssistant()){
#ifdef NO_ASSISTANT
        startSimpleHelpBrowser();
#endif
        return;
    }else{
        QByteArray ba("SetSource ");
        ba.append(HelpBrowser::getDocumentationNamespace());
        _process->write(ba + page_.toLocal8Bit() + '\n');
    }
}

bool HelpBrowser::startAssistant() {
    if (!_process){
        _process = new QProcess(this);
        connect(_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(endAssistant(int, QProcess::ExitStatus)));
        connect(_process, SIGNAL(error(QProcess::ProcessError)), this, SLOT(errorAssistant(QProcess::ProcessError)));;
    }

    if (_process->state() != QProcess::Running) {
        QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath) + QDir::separator();
#if !defined(Q_OS_MAC)
        app += QLatin1String("assistant");
#else
        app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");
#endif

        QString helpCollectionFile = HelpBrowser::getDocumentationFile();
        //qDebug() << "doc : " << helpCollectionFile;
        QFileInfo file(helpCollectionFile);
        if(!file.exists())
            return false;

        QStringList args;
        args << QLatin1String("-collectionFile")
            << HelpBrowser::getDocumentationFile()
            << QLatin1String("-enableRemoteControl");

        _process->start(app, args);

        if (!_process->waitForStarted()) {
            QMessageBox::critical(0, QObject::tr("Help"), QObject::tr("Unable to launch Qt Assistant (%1)").arg(app));
            delete _process;
            _process = 0;
            return false;
        }
    }
    return true;
}

void HelpBrowser::endAssistant(int exitCode_, QProcess::ExitStatus exitStatus_){
    (void)exitCode_;
    if(exitStatus_ == QProcess::CrashExit)
        qWarning() << "Assistant crashed :(";
    delete _process;
    _process = 0;
}

void HelpBrowser::errorAssistant(QProcess::ProcessError error_){
    qDebug() << "errorAssistant " << error_;
}

#ifdef NO_ASSISTANT
void HelpBrowser::startSimpleHelpBrowser(){
    if(_helpDlg == 0){
        _helpDlg = new HelpBrowserDialog();
        connect(_helpDlg, SIGNAL(finished(int)), this, SLOT(endSimpleHelpBrowser(int)));
        _helpDlg->show();
    }
}

void HelpBrowser::endSimpleHelpBrowser(int exitCode_){
    (void)exitCode_;
    _helpDlg->deleteLater();
    _helpDlg = 0;
}
#endif
