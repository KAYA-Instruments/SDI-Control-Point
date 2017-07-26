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
 * @file    flashloader.h
 *
 * @brief   Class definition of the flash loader
 *          a wrapper class to run the flashloader application
 *
 *****************************************************************************/
#ifndef __FLASH_LOADER_H__
#define __FLASH_LOADER_H__

#include <QObject>
#include <QProcess>

/******************************************************************************
 * System Update Box Widget
 *****************************************************************************/
class FlashLoader : public QObject
{
    Q_OBJECT

public:
    explicit FlashLoader();
    ~FlashLoader();

    enum FlashLoadCommand
    {
        FLASHLOAD_CMD_INVALID       = -1,
        FLASHLOAD_CMD_APPL_VERSION  =  0,   // get application version
        FLASHLOAD_CMD_SYSTEM_INFO   =  1,   // get system information of connected camera
        FLASHLOAD_CMD_ERASE         =  2,   // erase sectors (start, count)
        FLASHLOAD_CMD_PROGRAM       =  3,   // program/flash image
        FLASHLOAD_CMD_READBACK      =  4,   // readback to image
        FLASHLOAD_CMD_REBOOT        =  5,   // reboot to command mode
        FLASHLOAD_CMD_MAX
    };

    quint32 Baudrate() const
    {
        return ( m_baudrate );
    }

    void setBaudrate( const quint32 baudrate )
    {
        m_baudrate = baudrate;
    }

    QString Portname() const
    {
        return ( m_portname );
    }

    void setPortname( const QString& portname )
    {
        m_portname = portname;
    }

    bool Verify() const
    {
        return ( m_verify );
    }

    void setVerify( const bool verify )
    {
        m_verify = verify;
    }

    bool Boot() const
    {
        return ( m_boot );
    }

    void setBoot( const bool boot )
    {
        m_boot = boot;
    }

    bool Reverse() const
    {
        return ( m_reverse );
    }

    void setReverse( const bool reverse )
    {
        m_reverse = reverse;
    }

    QString getCommand();
    QStringList getArguments( const FlashLoadCommand cmd );
    int getErrorCode( QString error );
    int parseLine( QString line );
    void parseSystemId( QString name );

    int runCommand( FlashLoadCommand cmd, quint32 start = 0u, quint32 count = 0u, QString fn = QString::null );
    int stopCommand();

signals:
    void FlashLoaderVersion( quint32 m, quint32 n );
    void SystemName( QString name );
    void SystemId( qint32 id );
    void SystemVersion( quint32 m, quint32 n, quint32 o );
    void FlashBlockNo( quint32 n );
    void FlashBlockSize( quint32 n );
    void FirstFlashBlock( quint32 id );
    void ReconfCond( quint32 mask );
    void FlashLoaderError( int no );
    
    void ReadbackProgress( quint32 progress );
    void EraseProgress( quint32 progress );
    void ProgramProgress( quint32 progress );
    void VerifyProgress( quint32 progress );
    void UpdateFinished();

public slots:
    void onVerifyChanged( int state );
    void onBootChanged( int state );

private slots:
    void readyReadStandardOutput();
    void readyReadStandardError();
    void loaderStarted();
    void loaderFinished();
    
private:
    QProcess *          m_process;      // process instance to run flash-loader application
    QString             m_toolpath;     // path to flash-loader executable
    QString             m_appl;         // application command to run
    quint32             m_baudrate;     // baudrate to use by flash-loader application
    QString             m_portname;     // port to open by flashloader application
    FlashLoadCommand    m_cmd;
    bool                m_verify;       // verify flashed image
    bool                m_boot;         // reboot after flashing
    bool                m_reverse;      // bit reversal

    QString             m_stdout;       // stdout of flash-loader application
    QString             m_stderr;       // stderr of flash-loader application

    // helper
    quint32             m_start;
    quint32             m_count;
    QString             m_filename;
};

#endif // __FLASH_LOADER_H__

