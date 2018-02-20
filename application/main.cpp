/******************************************************************************
 * Copyright (C) 2017 Dream Chip Technologies GmbH
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/
/**
 * @file    main.cpp
 *
 * @brief   Application main file
 *
 *****************************************************************************/
#include <QtDebug>
#include <QFile>
#include <QApplication>

#include "defines.h"
#include "connectdialog.h"

#include "mainwindow.h"


#include <QThread>

/******************************************************************************
 * main 
 *****************************************************************************/
int main(int argc, char *argv[])
{
    int res;

    QApplication app(argc, argv);

    // register meta operators
    qRegisterMetaTypeStreamOperators<QList<int> >("QList<int>");
    qRegisterMetaTypeStreamOperators<QVector<int> >("QVector<int>");
   
    // Use dark style from https://github.com/ColinDuquesnoy/QDarkStyleSheet
    QFile f(":/style/darkstyle.qss");
    f.open( QFile::ReadOnly );
    QTextStream ts( &f );
    app.setStyleSheet( ts.readAll() );

    // Create the connection dialog
    ConnectDialog dlg;

    // Create the main application window
    MainWindow w( &dlg );

    // Try to automatically scan an connect
    bool connected = dlg.scanAndConnect();
    app.processEvents( QEventLoop::WaitForMoreEvents );

    // If the connection can be established automatically directly show the main window
    if ( connected )
    {
        w.show();
        res = app.exec();
    }
    // If no connection was established, show the connect dialog
    else
    {
        dlg.show();
        res = app.exec();
        if ( dlg.result() == QDialog::Rejected )
        {
            // dialog closed with cancel
            return ( res );
        }
        else
        {
            // Show the (now connected) main window
            w.show();
            res = app.exec();
        }
    }

    return res;
}

