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
 * @file    debugterminal.cpp
 *
 * @brief   Class implementation of debug terminal to display data which is
 *          send / received over a com port. It can also be used to send debug
 *          commands.
 *
 *****************************************************************************/

#include <QClipboard>
#include <QMessageBox>

#include "debugterminal.h"
#include "ui_debugterminal.h"

/******************************************************************************
 * DebugTerminal::DebugTerminal
 *****************************************************************************/
DebugTerminal::DebugTerminal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DebugTerminal)
{
    ui->setupUi(this);

    // Connect internal signals
    connect( ui->letInput, SIGNAL(returnPressed()), this, SLOT(onSendData()) );
    connect( ui->letInput, SIGNAL(textEdited(QString)), this, SLOT(onTextEdited(QString)) );
    connect( ui->btnShowHelp, SIGNAL(clicked()), this, SLOT(onShowHelpClicked()) );
    connect( ui->btnClearTerminal, SIGNAL(clicked()), this, SLOT(onClearTerminalClicked()) );
}

/******************************************************************************
 * DebugTerminal::~DebugTerminal
 *****************************************************************************/
DebugTerminal::~DebugTerminal()
{
    delete ui;
}

/******************************************************************************
 * DebugTerminal::onDataReceived
 *****************************************************************************/
void DebugTerminal::onDataReceived( QString data )
{
    // Insert string witout a leading or trailing new line
    ui->tbTerminal->moveCursor( QTextCursor::End );
    ui->tbTerminal->insertPlainText( data.remove(QRegExp("[\\r]")) );
    ui->tbTerminal->moveCursor( QTextCursor::End );
}

/******************************************************************************
 * DebugTerminal::onTextEdited
 *****************************************************************************/
void DebugTerminal::onTextEdited( QString text )
{
    // Check if data got pasted from clipboard
    if ( text == QApplication::clipboard()->text() )
    {
        // Clear line edit
        ui->letInput->clear();

        // Get lines from clipboard text
        QStringList lines = text.split( QRegularExpression("[\\n]"), QString::SkipEmptyParts );

        // Transmitt each line as a single command
        foreach(QString line, lines)
        {
             emit sendData( line.append('\n'), ui->sbxWaitTime->value() );
        }
    }
}

/******************************************************************************
 * DebugTerminal::onSendData
 *****************************************************************************/
void DebugTerminal::onSendData()
{
    // Send string currently stored in the line edit
    setWaitCursor();
    emit sendData( ui->letInput->text().append('\n'), ui->sbxWaitTime->value() );
    setNormalCursor();

    // Clear the line edit
    ui->letInput->clear();
}

/******************************************************************************
 * DebugTerminal::onShowHelpClicked
 *****************************************************************************/
void DebugTerminal::onShowHelpClicked()
{
    // Show help text in a message box
    QMessageBox::information( this,
                              "How to Use the Debug Terminal",
                              "The Debug Terminal will show all commands which were sent by the ProVideo GUI.\n\n"
                              "You can also enter your own commands in the field above or copy and paste a list of "
                              "commands from the clip board to that field.\n\n"
                              "If you want to execute long commands (e.g. defect pixel calibration) increase the "
                              "response wait time, otherwise the command output might not be shown." );
}

/******************************************************************************
 * DebugTerminal::onClearTerminalClicked
 *****************************************************************************/
void DebugTerminal::onClearTerminalClicked()
{
    // Clear content of terminal
    ui->tbTerminal->clear();
}
