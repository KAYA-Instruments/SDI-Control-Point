/******************************************************************************
 *
 * Copyright 2015, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    ComChannelRSxxx.h
 *
 * @brief   
 *
 *****************************************************************************/
#ifndef _COM_CHANNEL_RSxxx_H_
#define _COM_CHANNEL_RSxxx_H_

#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>

// generic control channel layer
#include <ctrl_channel/ctrl_channel_serial.h>

#include "ComChannel.h"

class ComChannelSerial : public ComChannel
{
public:
    explicit ComChannelSerial() : ComChannel(),
        m_port( NULL ),
        m_idx( 0 ),
        m_data( 0 ),
        m_parity( 0 ),
        m_stop( 0 ),
        m_baudrate( 0 ),
        m_reopenable( false )
    {
    }

    QSerialPort * getPort()
    {
        return ( m_port );
    }

    void setPort( QSerialPort * p )
    {
        m_port = p;
    }

    uint8_t getPortIndex() const
    {
        return ( m_idx );
    }

    void setPortIndex( uint8_t idx )
    {
        m_idx = idx;
    }

    uint8_t getNoDataBits() const
    {
        return ( m_data );
    }

    void setNoDataBits( uint8_t data )
    {
        m_data = data;
    }

    uint8_t getParity() const
    {
        return ( m_parity );
    }

    void setParity( uint8_t parity )
    {
        m_parity = parity;
    }
 
    uint8_t getNoStopBits() const
    {
        return ( m_stop );
    }

    void setNoStopBits( uint8_t stop )
    {
        m_stop = stop;
    }

    uint32_t getBaudRate() const
    {
        return ( m_baudrate );
    }

    void setBaudRate( uint32_t baudrate )
    {
        m_baudrate = baudrate;
    }

    void setReOpenAble( bool reopenable )
    {
        m_reopenable = reopenable;
    }

    int getNoPorts() const;
    int getPortName( int idx, ctrl_channel_name_t name );

    QString getSystemPortName()
    {
        if ( m_port )
        {
            return ( QSerialPortInfo(*m_port).systemLocation() );
        }

        return ( QString::null );
    }

private:
    QSerialPort *   m_port;

    uint8_t         m_idx;          /**< system port index */
    uint8_t         m_data;         /**< number of data bits */
    uint8_t         m_parity;       /**< type of parity */
    uint8_t         m_stop;         /**< number of stop bits */
    uint32_t        m_baudrate;     /**< baudrate */
    bool            m_reopenable;   /**< port can be reopened */
};

class ComChannelRS232 : public ComChannelSerial
{
public:
    explicit ComChannelRS232();
    
    void ReOpen() override;
};

class ComChannelRS4xx: public ComChannelSerial
{
public:
    explicit ComChannelRS4xx( unsigned int dev_addr = 1u );
    
    void ReOpen() override;

    void setDeviceAddress( unsigned int dev_addr )
    {
        m_dev_addr = dev_addr;
    }

    unsigned int getDeviceAddress()
    {
        return ( m_dev_addr );
    }

private:
    unsigned int m_dev_addr;
};


#endif // _COM_CHANNEL_RSxxx_H_

