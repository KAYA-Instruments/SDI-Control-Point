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
#include <QKeyEvent>

#include "debugterminal.h"
#include "ui_debugterminal.h"

/******************************************************************************
 * DebugTerminal::DebugTerminal
 *****************************************************************************/
DebugTerminal::DebugTerminal( QWidget *parent ) :
    QWidget( parent ),
    ui( new Ui::DebugTerminal ),
    currentHistoryIndex( -1 )
{
    // Setup UI
    ui->setupUi(this);

    // Install event filter on line edit to catch arrow up, down events
    ui->letInput->installEventFilter(this);

    // Connect internal signals
    connect( ui->letInput, SIGNAL(returnPressed()), this, SLOT(onSendCommand()) );
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
 * DebugTerminal::showEvent
 *****************************************************************************/
void DebugTerminal::showEvent( QShowEvent* event )
{
    // Call parents show event
    QWidget::showEvent( event );

    // Set focus to the line edit
    ui->letInput->setFocus();
}

/******************************************************************************
 * DebugTerminal::eventFilter
 *****************************************************************************/
bool DebugTerminal::eventFilter(QObject* obj, QEvent *event)
{
    // Filter for the input line edit
    if (obj == ui->letInput)
    {
        if (event->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

            // UP key was pressed, show older command from the history
            if (keyEvent->key() == Qt::Key_Up)
            {
                // Check if an older command is available in the history
                if ( currentHistoryIndex < commandHistory.length() - 1 )
                {
                    // Select older command
                    currentHistoryIndex++;

                    // Show command in line edit, block signal to not emit a text changed event
                    ui->letInput->blockSignals( true );
                    ui->letInput->setText( commandHistory.at( currentHistoryIndex ) );
                    ui->letInput->blockSignals( false );
                }

                // Event was processed succesfully
                return true;
            }

            // DOWN key was pressed, show newer command from the history
            else if(keyEvent->key() == Qt::Key_Down)
            {
                // Check if a newer command is available in the history
                if ( currentHistoryIndex >= 0 )
                {
                    // Select newer command
                    currentHistoryIndex--;

                    // If the index is now -1, show an empty line edit
                    if ( currentHistoryIndex <= -1 )
                    {
                        ui->letInput->blockSignals( true );
                        ui->letInput->clear();
                        ui->letInput->blockSignals( false );
                    }
                    // Else show command in line edit, block signal to not emit a text changed event
                    else
                    {
                        ui->letInput->blockSignals( true );
                        ui->letInput->setText( commandHistory.at( currentHistoryIndex ) );
                        ui->letInput->blockSignals( false );
                    }
                }

                // Event was processed succesfully
                return true;
            }
        }

        // Filter did not match the event
        return false;
    }

    // Call parent event filter
    return DebugTerminal::eventFilter(obj, event);
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

    // Check if string is empty, if so, reset current history index to -1
    if ( text.isEmpty() )
    {
        currentHistoryIndex = -1;
    }
}

/******************************************************************************
 * DebugTerminal::onSendCommand
 *****************************************************************************/
void DebugTerminal::onSendCommand()
{
    // Get command from line edit
    QString command = ui->letInput->text();

    // Check if the current command is not the last command in the history
    if ( commandHistory.isEmpty() || command != commandHistory.first() )
    {
        // Add string to the start of the history
        commandHistory.prepend( ui->letInput->text() );
    }

    // Send command with tailing new line
    setWaitCursor();
    emit sendData( command.append('\n'), ui->sbxWaitTime->value() );
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
                              "You can enter your own commands in the 'Command' field or copy and paste a list of "
                              "commands from the clip board to that field.\n\n"
                              "The Debug Console has a command history which you can access by pressing the Arrow Up "
                              "or Down keys on your keyboard while the 'Command' field is selected.\n\n"
                              "If you want to execute long commands (e.g. defect pixel calibration) increase the "
                              "response wait time, otherwise the command output will not be shown.\n\n"
                              "The Debug Terminal and the GUI have to share the Com Port, this means "
                              "while a command from the Debug Terminal is executed you should not make changes in the "
                              "GUI and vice versa.\n\n"
                              "Please note that any changes you make using debug commands will not be shown in the "
                              "GUI. E.g. changing the video mode from 1080i50 to 1080p30 with a debug command will "
                              "not change the displayed video mode in the GUI window. You will have to reconnect "
                              "to the device by using the Conenct Dialog to make such changes visible." );
}

/******************************************************************************
 * DebugTerminal::onClearTerminalClicked
 *****************************************************************************/
void DebugTerminal::onClearTerminalClicked()
{
    // Clear content of terminal
    ui->tbTerminal->clear();

    // Clear history
    commandHistory.clear();
    currentHistoryIndex = -1;
}
