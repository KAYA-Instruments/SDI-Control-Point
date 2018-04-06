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
 * @file    textviewer.cpp
 *
 * @brief   Class implementation of a simple viewer for txt files.
 *
 *****************************************************************************/

#include <QDebug>
#include <QVBoxLayout>
#include <QFile>

#include "textviewer.h"
#include "ui_textviewer.h"

TextViewer::TextViewer(const QString &path, const QString &title, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextViewer)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowModality(Qt::WindowModal);

    // Create and set widget layout
    QVBoxLayout *mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    // Create Text Browser and add it to the layout
    textBrowser = new QTextBrowser;
    mainLayout->addWidget(textBrowser);

    // Open file
    QFile file( path );
    if ( !file.open( QIODevice::ReadOnly ) )
    {
        qDebug() << "Can not open file: " << path;
        return;
    }

    // Load contents of file to string and display it in the text browser
    QTextStream stream(&file);
    QString content = stream.readAll();
    file.close();
    textBrowser->setPlainText(content);

    // Show window title
    setWindowTitle( title );
}

TextViewer::~TextViewer()
{
    delete textBrowser;
    delete ui;
}
