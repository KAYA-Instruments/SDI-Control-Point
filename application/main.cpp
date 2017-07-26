/******************************************************************************
 *
 * Copyright 2016, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
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

    // If the connection can be established automatically directly show the main window
    if ( dlg.scanAndConnect() )
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

