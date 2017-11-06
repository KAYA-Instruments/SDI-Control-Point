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
 * @file    textviewer.h
 *
 * @brief   Class definition of simple widget to view the content of txt files
 *
 *****************************************************************************/

#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include <QDialog>
#include <QTextBrowser>

namespace Ui {
class TextViewer;
}

class TextViewer : public QDialog
{
    Q_OBJECT

public:
    explicit TextViewer( const QString &path, const QString &title, QWidget *parent = 0 );
    ~TextViewer();

private:
    Ui::TextViewer *ui;
    QTextBrowser *textBrowser;
};

#endif // TEXTVIEWER_H
