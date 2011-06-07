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

#ifndef __HELP_VIEWER__
#define __HELP_VIEWER__

#include <QObject>
#include <QProcess>

class QString;
class QDialog;

class HelpBrowser : public QObject {

	Q_OBJECT
    
    public:
        HelpBrowser(QObject * parent_ = 0);
        ~HelpBrowser();
        
        void showDocumentation(const QString &file_);
    
    public slots:
        bool startAssistant();
        void endAssistant(int exitCode_, QProcess::ExitStatus exitStatus_);
        void errorAssistant(QProcess::ProcessError error_);
    
        void startSimpleHelpBrowser();
        void endSimpleHelpBrowser(int exitCode_);
    
    private:
        QProcess *_process;

        QDialog* _helpDlg;
};

#endif // __HELP_VIEWER__
