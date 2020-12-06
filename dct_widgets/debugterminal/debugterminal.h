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
 * @file    debugterminal.h
 *
 * @brief   Class definition of debug terminal to display data which is
 *          send / received over a com port. It can also be used to send debug
 *          commands.
 *
 *****************************************************************************/

#ifndef DEBUGTERMINAL_H
#define DEBUGTERMINAL_H

#include <QWidget>
#include <QApplication>
#include <QEvent>
#include <QList>
#include <QString>

namespace Ui {
class DebugTerminal;
}

class DebugTerminal : public QWidget
{
    Q_OBJECT

public:
    explicit DebugTerminal(QWidget *parent = nullptr);
    ~DebugTerminal() override;

signals:
    void sendData( QString data, int );

public slots:
    void onDataReceived( QString data );

protected:
    void showEvent( QShowEvent* event ) override;
    void resizeEvent( QResizeEvent *event ) override;
    bool eventFilter(QObject* obj, QEvent *event) override;

private:
    Ui::DebugTerminal *ui;
    QList<QString> commandHistory;
    int currentHistoryIndex;

    void setWaitCursor()
    {
        QApplication::setOverrideCursor( Qt::WaitCursor );
    }

    void setNormalCursor()
    {
        QApplication::setOverrideCursor( Qt::ArrowCursor );
    }

private slots:
    void onSendCommand();
    void onTextEdited( QString text );
    void onShowHelpClicked();
    void onSaveLogClicked();
    void onClearTerminalClicked();
    void onShowCommandsClicked();
};

#endif // DEBUGTERMINAL_H
