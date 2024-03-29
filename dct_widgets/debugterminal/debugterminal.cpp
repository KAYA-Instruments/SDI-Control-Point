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

#include <QScrollBar>
#include <QClipboard>
#include <QMessageBox>
#include <QKeyEvent>
#include <QFileDialog>
#include <QTextStream>

#include "debugterminal.h"
#include "ui_debugterminal.h"
#include <provideo_protocol/provideo_protocol_common.h>

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

    // Limit number of lines in the text browser to 10000
    ui->tbTerminal->document()->setMaximumBlockCount( 10000 );
    ui->sbxWaitTime->setValue(DEFAULT_CMD_TIMEOUT);

    // Install event filter on line edit to catch arrow up, down events
    ui->letInput->installEventFilter(this);

    // Connect internal signals
    connect( ui->letInput, SIGNAL(returnPressed()), this, SLOT(onSendCommand()) );
    connect( ui->letInput, SIGNAL(textEdited(QString)), this, SLOT(onTextEdited(QString)) );
    connect( ui->btnShowHelp, SIGNAL(clicked()), this, SLOT(onShowHelpClicked()) );
    connect( ui->btnSaveLog, SIGNAL(clicked()), this, SLOT(onSaveLogClicked()) );
    connect( ui->btnClearTerminal, SIGNAL(clicked()), this, SLOT(onClearTerminalClicked()) );
    connect( ui->btnShowCommands, SIGNAL(clicked()), this, SLOT(onShowCommandsClicked()) );
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
 * DebugTerminal::resizeEvent
 *****************************************************************************/
void DebugTerminal::resizeEvent( QResizeEvent *event )
{
    // Call parents resize event
    QWidget::resizeEvent( event );

    // Make sure the terminal text browser is scrolled down
    QScrollBar *scrollBar = ui->tbTerminal->verticalScrollBar();
    scrollBar->setValue( scrollBar->maximum() );
}

/******************************************************************************
 * DebugTerminal::eventFilter
 *****************************************************************************/
bool DebugTerminal::eventFilter(QObject* obj, QEvent *event)
{
    // Call parents event filter function
    QWidget::eventFilter( obj, event );

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
    // Check if data got pasted from clipboard and contains more than one line
    if ( text.contains("\n") && text == QApplication::clipboard()->text() )
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

    // Check if the current command is not the last command in the history and is not empty
    if ( (commandHistory.isEmpty() || command != commandHistory.first()) && (command.length() > 0) )
    {
        // Add string to the start of the history
        commandHistory.prepend( ui->letInput->text() );

        // If history is bigger than the maximum, delete the last entry
        if ( commandHistory.count() > 100 )
        {
            commandHistory.removeLast();
        }
    }

    // Set current history index to -1
    currentHistoryIndex = -1;

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
                              "The Debug Terminal will show all commands which were sent by the KAYA GUI. It stores "
                              "up to 10000 lines, if more lines are added the oldest lines will be removed.\n\n"
                              "You can enter your own commands in the 'Command' field or copy and paste a list of "
                              "commands from the clip board to that field.\n\n"
                              "The Debug Console has a command history which you can access by pressing the Arrow Up "
                              "or Down keys on your keyboard while the 'Command' field is selected. The command history "
                              "can contain up to 100 commands, if more commands are sent the oldest ones are removed from "
                              "the history.\n\n"
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
 * DebugTerminal::onSaveLogClicked
 *****************************************************************************/
void DebugTerminal::onSaveLogClicked()
{

    QString directory = QDir::currentPath();

    // NOTE: It can fail on gtk-systems when an empty filename is given
    //       in the native dialog-box, because GTK sends a SIGSEGV-signal
    //       to process and this is not handled by Qt.
    QFileDialog dialog( this );
    dialog.setDefaultSuffix( "txt" );
    QString filename = dialog.getSaveFileName(
        this, tr("Save Debug Terminal Log File"),
        directory,
        "Select Text files (*.txt);;All files (*.*)"
    );

    if ( nullptr != filename )
    {
        QFileInfo fileInfo( filename );
        if ( fileInfo.suffix().isEmpty() )
        {
            filename += ".txt";
        }

        QFile file( filename );
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning( this,
                                  "Can not open file for writing.",
                                  QString("The file %1 can not opened for writing. Do you have write access for the "
                                          "selected folder?").arg(filename) );
        }
        else
        {
            QTextStream out(&file);
            out << ui->tbTerminal->document()->toPlainText();
            file.close();
        }
    }
}

/******************************************************************************
 * DebugTerminal::onClearTerminalClicked
 *****************************************************************************/
void DebugTerminal::onClearTerminalClicked()
{
    // Clear content of terminal
    ui->tbTerminal->clear();
}

/******************************************************************************
 * DebugTerminal::onShowCommandsClicked
 *****************************************************************************/
void DebugTerminal::onShowCommandsClicked()
{
    // Display all available commands in terminal
    QString command = "help";
    emit sendData( command.append('\n'), ui->sbxWaitTime->value() );

}
