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
 * @file    flashloader.cpp
 *
 * @brief   Implementation of flash loader
 *
 *****************************************************************************/
#include <QFileInfo>
#include <QDir>
#include <QtDebug>

#include <cerrno>

#include "id.h"
#include "flashloader.h"

/******************************************************************************
 * definitions
 *****************************************************************************/

// regular expressions to parse error messages
#define REGEX_ERROR_TIMEOUT             ( "^Timeout" )
#define REGEX_ERROR_NO_DEV              ( "^No device found" )
#define REGEX_ERROR_REQ_VERSION_FAILED  ( "^Requesting flashloader version failed" )
#define REGEX_ERROR_VERSION_INVALID     ( "^Incompatible flashloader version" )
#define REGEX_ERROR_REQ_INFO_FAILED     ( "^Failed to request flash information" )
#define REGEX_ERROR_ERASE_FAILED        ( "^Erasing flash blocks failed" )
#define REGEX_ERROR_PROGRAM_FAILED      ( "^Flash programming failed" )
#define REGEX_ERROR_VERIFY_FAILED       ( "^Verifying failed" )
#define REGEX_ERROR_REBOOT_FAILED       ( "^Reboot failed" )

#define REGEX_ERROR_MASK_1              ( "^ERROR: (.*)[.!]+$" )
#define REGEX_ERROR_MASK_2              ( "^error (.*)+$" )

// regular expressions to parse stdout
#define REGEX_FLASHLOADER_VERSION       ( "^flashloader ([0-9]+).([0-9]+)$" )
#define REGEX_SYSTEM_NAME               ( "^System-ID: (.*)$" )
#define REGEX_SYSTEM_VERSION            ( "^Version: ([0-9]+).([0-9]+).([0-9]+)$" )
#define REGEX_FLASH_BLOCK_NO            ( "^Flash block count: ([0-9]+)$" )
#define REGEX_FLASH_BLOCK_SIZE          ( "^Flash block size: ([0-9]+) bytes$" )
#define REGEX_FIRST_FLASH_BLOCK         ( "^First writable flash block: ([0-9]+)$" )
#define REGEX_RECONF_COND               ( "^Reconf cond: ([0-9]+)$" )
#define REGEX_READBACK                  ( "^Readback: ([0-9]+)%$" )
#define REGEX_READBACK_DONE             ( "^Readback done.$" )
#define REGEX_ERASE                     ( "^Erasing ([0-9]+)/([0-9]+)...$" )
#define REGEX_ERASE_DONE                ( "^Erasing ([0-9]+)/([0-9]+)...done.$" )
#define REGEX_PROGRAM                   ( "^Programming: ([0-9]+)%$" )
#define REGEX_PROGRAM_DONE              ( "^Programming done.$" )
#define REGEX_VERIFY                    ( "^Verifying: ([0-9]+)%$" )
#define REGEX_VERIFY_DONE               ( "^Verifying done.$" )

// regular expression to parse system name
#define REGEX_ATOME_ONE                 ( "ATOM ONE (X-BOW)" )
#define REGEX_ATOME_ONE_4K              ( "ATOM ONE 4K (CONDOR)" )
#define REGEX_ATOME_ONE_4K_MINI         ( "ATOM ONE 4K MINI (CONDOR)" )

/******************************************************************************
 * fileExists
 *****************************************************************************/
static bool fileExists( QString & filename )
{
    QFileInfo check_file( filename );
    return ( check_file.exists() && check_file.isFile() && check_file.isExecutable() );
}

/******************************************************************************
 * FlashLoader::FlashLoader
 *****************************************************************************/
FlashLoader::FlashLoader()
    : QObject()
    , m_process( new QProcess( this ) )
    , m_toolpath( QDir::currentPath() + QDir::separator() + "tools" + QDir::separator() )
    , m_appl( FLASH_LOADER_APPLICATION )
    , m_baudrate( 57600u )
    , m_portname( "/dev/ttyUSB0" )
    , m_cmd( FLASHLOAD_CMD_INVALID )
    , m_verify( false )
    , m_boot( false )
{
    m_stdout.clear();
    m_stderr.clear();

    connect( m_process, SIGNAL(started())                , this, SLOT(loaderStarted()));
    connect( m_process, SIGNAL(finished(int))            , this, SLOT(loaderFinished()));
    connect( m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
    connect( m_process, SIGNAL(readyReadStandardError()) , this, SLOT(readyReadStandardError()));
}

/******************************************************************************
 * FlashLoader::~FlashLoader
 *****************************************************************************/
FlashLoader::~FlashLoader()
{
    delete m_process;
}

/******************************************************************************
 * FlashLoader::getCommand
 *****************************************************************************/
QString FlashLoader::getCommand()
{
    QFileInfo exeFileInfo( m_toolpath );
    QString path = exeFileInfo.absolutePath();
    QString cmd = QDir::fromNativeSeparators( path + QDir::separator() + m_appl );
    return ( cmd );
}

/******************************************************************************
 * FlashLoader::getArguments
 *****************************************************************************/
QStringList FlashLoader::getArguments( const FlashLoadCommand cmd )
{
    QStringList param;

    switch ( cmd )
    {
        case FLASHLOAD_CMD_SYSTEM_INFO:
            param
                << "-v"
                << "-p" << m_portname
                << "-br" << QString::number( m_baudrate );
            break;

        case FLASHLOAD_CMD_READBACK:
            param 
                << "-v"
                << "-p" << m_portname
                << "-br" << QString::number( m_baudrate )
                << "-rf" << QString::number( m_start )
                << "-rc" << QString::number( m_count )
                << "-rn" << m_filename;
            break;

        case FLASHLOAD_CMD_ERASE:
            param 
                << "-v"
                << "-p" << m_portname
                << "-br" << QString::number( m_baudrate )
                << "-ef" << QString::number( m_start )
                << "-ec" << QString::number( m_count );
            break;

        case FLASHLOAD_CMD_PROGRAM:
            param 
                << "-v"
                << "-p" << m_portname
                << "-br" << QString::number( m_baudrate )
                << "-e"
                << ((m_verify) ? "-F" : "-f") << QString::number( m_start )
                << "-i" << m_filename;
            if ( m_boot )
            {
                param << "-b" << "1";
            }
            if ( m_reverse )
            {
                param << "-R";
            }
            break;

        case FLASHLOAD_CMD_REBOOT:
            param 
                << "-v"
                << "-p" << m_portname
                << "-br" << QString::number( m_baudrate )
                << "-b" << "1";
            break;

        case FLASHLOAD_CMD_APPL_VERSION:
        default:
            /* empty list */
            break;
    }

    return ( param );
}

/******************************************************************************
 * FlashLoader::getErrorCode
 *****************************************************************************/
int FlashLoader::getErrorCode( QString error )
{
    QRegExp rx;
    int pos;

    rx = QRegExp( REGEX_ERROR_TIMEOUT );
    pos = rx.indexIn( error );
    if ( pos > -1 )
    {
        return ( -ETIMEDOUT );
    }

    rx = QRegExp( REGEX_ERROR_NO_DEV );
    pos = rx.indexIn( error );
    if ( pos > -1 )
    {
        return ( -ENODEV );
    }

    rx = QRegExp( REGEX_ERROR_REQ_VERSION_FAILED );
    pos = rx.indexIn( error );
    if ( pos > -1 )
    {
        return ( -EIO );
    }

    rx = QRegExp( REGEX_ERROR_VERSION_INVALID );
    pos = rx.indexIn( error );
    if ( pos > -1 )
    {
        return ( -EPROTO );
    }

    rx = QRegExp( REGEX_ERROR_REQ_INFO_FAILED );
    pos = rx.indexIn( error );
    if ( pos > -1 )
    {
        return ( -EIO );
    }

    rx = QRegExp( REGEX_ERROR_ERASE_FAILED );
    pos = rx.indexIn( error );
    if ( pos > -1 )
    {
        return ( -EIO );
    }

    rx = QRegExp( REGEX_ERROR_PROGRAM_FAILED );
    pos = rx.indexIn( error );
    if ( pos > -1 )
    {
        return ( -EIO );
    }

    rx = QRegExp( REGEX_ERROR_VERIFY_FAILED );
    pos = rx.indexIn( error );
    if ( pos > -1 )
    {
        return ( -EIO );
    }

    rx = QRegExp( REGEX_ERROR_REBOOT_FAILED );
    pos = rx.indexIn( error );
    if ( pos > -1 )
    {
        return ( -EIO );
    }

    return ( -EFAULT );
}

/******************************************************************************
 * FlashLoader::parseSystemId
 *****************************************************************************/
void FlashLoader::parseSystemId( QString name )
{
    if ( !name.compare( REGEX_ATOME_ONE ) )
    {
        emit SystemId( SYSTEM_ID_ATOM_ONE  );
        return;
    }

    if ( !name.compare( REGEX_ATOME_ONE_4K ) || !name.compare( REGEX_ATOME_ONE_4K_MINI ) )
    {
        emit SystemId( SYSTEM_ID_ATOM_ONE_4K  );
        return;
    }

    emit SystemId( SYSTEM_ID_INVALID  );
}
 
/******************************************************************************
 * FlashLoader::parseLine
 *****************************************************************************/
int FlashLoader::parseLine( QString line )
{
    QRegExp rx;
    int pos;

    rx = QRegExp( REGEX_FLASHLOADER_VERSION );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        emit FlashLoaderVersion( rx.cap(1).toUInt(), rx.cap(2).toUInt() );
        return ( 0 );
    }

    rx = QRegExp( REGEX_SYSTEM_NAME );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        emit SystemName( rx.cap(1) );
        parseSystemId( rx.cap(1) );
        return ( 0 );
    }

    rx = QRegExp( REGEX_SYSTEM_VERSION );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        emit SystemVersion( rx.cap(1).toUInt(), rx.cap(2).toUInt(), rx.cap(3).toUInt() );
        return ( 0 );
    }

    rx = QRegExp( REGEX_FLASH_BLOCK_NO );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        emit FlashBlockNo( rx.cap(1).toUInt() );
        return ( 0 );
    }

    rx = QRegExp( REGEX_FLASH_BLOCK_SIZE );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        emit FlashBlockSize( rx.cap(1).toUInt() );
        return ( 0 );
    }
    
    rx = QRegExp( REGEX_FIRST_FLASH_BLOCK );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        emit FirstFlashBlock( rx.cap(1).toUInt() );
        return ( 0 );
    }

    rx = QRegExp( REGEX_RECONF_COND );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        emit ReconfCond( rx.cap(1).toUInt() );
        return ( 0 );
    }

    rx = QRegExp( REGEX_READBACK );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        emit ReadbackProgress( rx.cap(1).toUInt() );
        return ( 0 );
    }

    rx = QRegExp( REGEX_READBACK_DONE );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        emit ReadbackProgress( 100u );
        return ( 0 );
    }

    rx = QRegExp( REGEX_ERASE );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        quint32 i = rx.cap(1).toUInt();
        quint32 n = rx.cap(2).toUInt();
        emit EraseProgress( (i*100u)/n );
        return ( 0 );
    }

    rx = QRegExp( REGEX_ERASE_DONE );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        emit EraseProgress( 100u );
        return ( 0 );
    }

    rx = QRegExp( REGEX_PROGRAM );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        emit ProgramProgress( rx.cap(1).toUInt() );
        return ( 0 );
    }

    rx = QRegExp( REGEX_PROGRAM_DONE );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        emit ProgramProgress( 100u );
        return ( 0 );
    }

    rx = QRegExp( REGEX_VERIFY );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        emit VerifyProgress( rx.cap(1).toUInt() );
        return ( 0 );
    }

    rx = QRegExp( REGEX_VERIFY_DONE );
    pos = rx.indexIn( line );
    if ( pos > -1 )
    {
        emit VerifyProgress( 100u );
        return ( 0 );
    }

    // no match => line not complete
    return ( -EINVAL );
}

/******************************************************************************
 * FlashLoader::runCommand
 *****************************************************************************/
int FlashLoader::runCommand( FlashLoadCommand cmd, quint32 start, quint32 count, QString fn )
{
    // check current state
    if ( FLASHLOAD_CMD_INVALID != m_cmd )
    {
        return ( -EBUSY );
    }

    // create system call 
    QString application = getCommand();
    if ( !fileExists( application ) )
    {
        return ( -ENOENT );
    }

    // create list of arguments
    m_start     = start;
    m_count     = count;
    m_filename  = fn;
    QStringList args = getArguments( cmd );

    // clear stdout and stderr
    m_stdout.clear();
    m_stderr.clear();

    // state
    m_cmd = cmd;
    
    // start system call
    m_process->start( application, args );

    return ( 0 );
}

/******************************************************************************
 * FlashLoader::stopCommand
 *****************************************************************************/
int FlashLoader::stopCommand()
{
    if ( FLASHLOAD_CMD_INVALID != m_cmd )
    {
        m_process->kill();
    }

    return ( 0 );
}

/******************************************************************************
 * FlashLoader::onVerifyChanged
 *****************************************************************************/
void FlashLoader::onVerifyChanged( int state )
{
    m_verify = ( state == Qt::Checked ) ? true : false;
}

/******************************************************************************
 * FlashLoader::onBootChanged
 *****************************************************************************/
void FlashLoader::onBootChanged( int state )
{
    m_boot = ( state == Qt::Checked ) ? true : false;
}
 
/******************************************************************************
 * FlashLoader::readyReadStandardOutput
 *****************************************************************************/
void FlashLoader::readyReadStandardOutput()
{
    // I. append 
    m_stdout.append( m_process->readAllStandardOutput() );
    
    // II. evaluate application output
    QStringList lout = m_stdout.split( QRegExp("[\r\n]+"), QString::SkipEmptyParts );
    if ( lout.count() > 0 )
    {
        // clear stdout
        m_stdout.clear();

        foreach( QString string, lout )
        {
            int res = parseLine( string );
            if ( !res )
            {
                // continue with next line
                continue;
            }

            // no match => append to stdout
            m_stdout.append( string );
        }
    }
}

/******************************************************************************
 * FlashLoader::readyReadStandardError
 *****************************************************************************/
void FlashLoader::readyReadStandardError()
{
    m_stderr.append( m_process->readAllStandardError() );

    // Force application close
    if ( m_cmd != FLASHLOAD_CMD_INVALID )
    {
        stopCommand();
    }
}

/******************************************************************************
 * FlashLoader::loaderStarted
 *****************************************************************************/
void FlashLoader::loaderStarted()
{
};

/******************************************************************************
 * FlashLoader::loaderFinished
 *****************************************************************************/
void FlashLoader::loaderFinished()
{
    // I. check for error
    QStringList lerr = m_stderr.split( QRegExp("[\r\n]+"), QString::SkipEmptyParts );
    foreach( QString string, lerr )
    {
        QRegExp rx1, rx2;
        int pos1, pos2;

        rx1 = QRegExp( REGEX_ERROR_MASK_1 );
        pos1 = rx1.indexIn( string );

        rx2 = QRegExp( REGEX_ERROR_MASK_2 );
        pos2 = rx2.indexIn( string );

        if ( pos1 > -1 )
        {
            m_cmd = FLASHLOAD_CMD_INVALID;
            emit FlashLoaderError( getErrorCode( rx1.cap(1) ));
            return;
        }
        else if ( pos2 > -1 )
        {
            m_cmd = FLASHLOAD_CMD_INVALID;
            emit FlashLoaderError( getErrorCode( rx2.cap(1) ));
            return;
        }
    }

    // II. check application output
    QStringList lout = m_stdout.split( QRegExp("[\r\n]+"), QString::SkipEmptyParts );
    if ( lout.count() > 0 )
    {
        // clear stdout
        m_stdout.clear();

        foreach( QString string, lout )
        {
            int res = parseLine( string );
            if ( !res )
            {
                // continue with next line
                continue;
            }
        }
    }

    m_cmd = FLASHLOAD_CMD_INVALID;

    emit UpdateFinished();
};



