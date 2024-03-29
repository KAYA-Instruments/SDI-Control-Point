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
 * @file    ComChannelRSxxx.cpp
 *
 * @brief   
 *
 *****************************************************************************/
#include <cerrno>

#include "common.h"
#include "ComChannelRSxxx.h"

#include <QThread>
#include <QElapsedTimer>

/******************************************************************************
 * ctrl_channel_qtserial_get_no_ports
 *****************************************************************************/
static int ctrl_channel_qtserial_get_no_ports( void * const handle )
{
    (void) handle;
    // call system implementation 
    return ( QSerialPortInfo::availablePorts().count() );
}

/******************************************************************************
 * ctrl_channel_qtserial_get_port_name
 *****************************************************************************/
static int ctrl_channel_qtserial_get_port_name
(
    void * const        handle,
    int const           idx,
    ctrl_channel_name_t name
)
{
    (void) handle;
    
    // call system implementation 
    QByteArray ba = QSerialPortInfo::availablePorts().at(idx).portName().toLatin1();
    strncpy( name, ba.data(), sizeof(ctrl_channel_name_t) );

    return ( 0 );
}

/******************************************************************************
 * ctrl_channel_qtserial_lock
 *****************************************************************************/
static int ctrl_channel_qtserial_lock( void * const handle )
{
    ComChannelSerial * com = static_cast<ComChannelSerial *>(handle);

    // check parameter
    if ( !com )
    {
        return ( -EINVAL );
    }

    // call channel implementation
    com->lock();

    return 0;
}

/******************************************************************************
 * ctrl_channel_qtserial_release
 *****************************************************************************/
static int ctrl_channel_qtserial_release( void * const handle )
{
    ComChannelSerial * com = static_cast<ComChannelSerial *>(handle);

    // check parameter
    if ( !com )
    {
        return ( -EINVAL );
    }

    // call channel implementation
    com->release();

    return 0;
}

/******************************************************************************
 * ctrl_channel_qtserial_rs232_open
 *****************************************************************************/
static int ctrl_channel_qtserial_rs232_open
(
    void * const    handle,
    void * const    param,
    int const       size
)
{
    // type cast context
    ComChannelRS232 * com = static_cast<ComChannelRS232 *>(handle);

    QSerialPort * port;

    // type cast open configuration
    ctrl_channel_rs232_open_config_t * conf  = static_cast<ctrl_channel_rs232_open_config_t *>(param);

    // check parameter
    if ( !com || !conf || (size != sizeof(ctrl_channel_rs232_open_config_t)) )
    {
        return ( -EINVAL );
    }

    // close the old com port
    if ( com->getPort() != nullptr )
    {
        com->getPort()->close();
        delete com->getPort();
        com->setPort( nullptr );
    }

    // create a new serial-port instance
    port = new QSerialPort( QSerialPortInfo::availablePorts().at(conf->idx) );
    if ( port )
    {
        QSerialPort::DataBits b = QSerialPort::UnknownDataBits;
        QSerialPort::BaudRate r = QSerialPort::UnknownBaud;
        QSerialPort::Parity   p = QSerialPort::UnknownParity;
        QSerialPort::StopBits s = QSerialPort::UnknownStopBits;

        // check parameter
        switch ( conf->baudrate )
        {
            case CTRL_CHANNEL_BAUDRATE_9600:
                r = QSerialPort::Baud9600;
                break;

            case CTRL_CHANNEL_BAUDRATE_19200: 
                r = QSerialPort::Baud19200;
                break;

            case CTRL_CHANNEL_BAUDRATE_38400: 
                r = QSerialPort::Baud38400;
                break;

            case CTRL_CHANNEL_BAUDRATE_57600: 
                r = QSerialPort::Baud57600;
                break;

            case CTRL_CHANNEL_BAUDRATE_115200: 
                r = QSerialPort::Baud115200;
                break;

            default:    // unsupported 
                delete port;
                return ( -EINVAL );
        }

        switch ( conf->data )
        {
            case CTRL_CHANNEL_DATA_BITS_5:
                b = QSerialPort::Data5;
                break;

            case CTRL_CHANNEL_DATA_BITS_6:
                b = QSerialPort::Data6;
                break;

            case CTRL_CHANNEL_DATA_BITS_7:
                b = QSerialPort::Data7;
                break;

            case CTRL_CHANNEL_DATA_BITS_8:
                b = QSerialPort::Data8;
                break;

            default:    // unsupported
                delete port;
                return ( -EINVAL );
        }

        switch ( conf->parity )
        {
            case CTRL_CHANNEL_PARITY_NONE:
                p = QSerialPort::NoParity;
                break;

            case CTRL_CHANNEL_PARITY_ODD:
                p = QSerialPort::OddParity;
                break;

            case CTRL_CHANNEL_PARITY_EVEN:
                p = QSerialPort::EvenParity;
                break;

            default:    // unsupported
                delete port;
                return ( -EINVAL );
        }

        switch ( conf->stop )
        {
            case CTRL_CHANNEL_STOP_BITS_1:
                s = QSerialPort::OneStop;
                break;

            case CTRL_CHANNEL_STOP_BITS_2:
                s = QSerialPort::TwoStop;
                break;

            default:    // unsupported
                delete port;
                return ( -EINVAL );
        }
    
        // open serial port 
        if ( !port->open( QIODevice::ReadWrite ) )
        {
            qDebug( "Control channel not opened!" );
            delete port;
            return ( -EPERM );
        }

        // set no flow-control, baudrate, data bits, parity, stop bits
        if ( !port->setFlowControl( QSerialPort::NoFlowControl ) )
        {
            qDebug() << port->errorString();
            delete port;
            return ( -EFAULT );
        }

        if ( !port->setBaudRate( r ) )
        {
            qDebug() << port->errorString();
            delete port;
            return ( -EFAULT );
        }

        if ( !port->setDataBits( b ) )
        {
            qDebug() << port->errorString();
            delete port;
            return ( -EFAULT );
        }

        if ( !port->setParity( p ) )
        {
            qDebug() << port->errorString();
            delete port;
            return ( -EFAULT );
        }

        if ( !port->setStopBits( s ) )
        {
            qDebug() << port->errorString();
            delete port;
            return ( -EFAULT );
        }

        // set port instance and configuration to the new port
        com->setPortIndex( conf->idx );
        com->setNoDataBits( conf->data );
        com->setParity( conf->parity );
        com->setNoStopBits( conf->stop );
        com->setBaudRate( conf->baudrate );
        com->setReOpenAble( true );
        com->setPort( port );

        return ( 0 );
    }

    return ( -ENOMEM );
}

/******************************************************************************
 * ctrl_channel_qtserial_rs232_close
 *****************************************************************************/
static int ctrl_channel_qtserial_rs232_close( void * const handle )
{
    // type cast context
    ComChannelRS232 * com = static_cast<ComChannelRS232 *>(handle);
    if ( com )
    {
        QSerialPort * port = com->getPort();
        com->setPort( nullptr );
        if ( port )
        {
            port->close();
            delete port;
        }
    }
   
    return ( 0 );
}

/******************************************************************************
 * ctrl_channel_qtserial_rs232_send_request
 *****************************************************************************/
static int ctrl_channel_qtserial_rs232_send_request
(
    void * const    handle,
    uint8_t * const data,
    int const       len
)
{
    int res = 0;

    // type cast context
    ComChannelRS232 * com = static_cast<ComChannelRS232 *>(handle);
    if ( com )
    {
        QSerialPort * port = com->getPort();
        if ( port )
        {
            /* We call the "emit data received" function here, although this is actually a data send.
             * This is done to have a local echo functionality in the debug terminal. */
            com->emitDataRecieved( reinterpret_cast<char *>(data) );

            res = static_cast<int>(port->write( reinterpret_cast<const char *>(data), len ));
        }
    }

    return ( res );
}

/******************************************************************************
 * ctrl_channel_qtserial_rs232_receive_response
 *****************************************************************************/
static int ctrl_channel_qtserial_rs232_receive_response
(
    void * const    handle,
    uint8_t * const data,
    int const       len
)
{
    int res = 0;

    // type cast context
    ComChannelRS232 * com = static_cast<ComChannelRS232 *>(handle);
    if ( com )
    {
        QSerialPort * port = com->getPort();
        if ( port )
        {
            // Check if bytes are available, otherwise wait 1ms for new data to arrive
            if( port->bytesAvailable() > 0 || port->waitForReadyRead(1) )
            {
                res = static_cast<int>(port->read( reinterpret_cast<char *>(data), len ));

                /* Call the emitDataReceived function which will emit a dataRecieved signal, if a slot
                 * is registered for that event. This is used for the debugging terminal, it is not needed
                 * for communication with the device */
                com->emitDataRecieved( reinterpret_cast<char *>(data) );
            }
        }
    }

    return ( res );
}

/******************************************************************************
 * ctrl_channel_qtserial_rs4xx_open
 *****************************************************************************/
static int ctrl_channel_qtserial_rs4xx_open
(
    void * const    handle,
    void * const    param,
    int const       size
)
{
    // type cast context
    ComChannelRS4xx * com = static_cast<ComChannelRS4xx *>(handle);

    QSerialPort * port;

    // type cast open configuration
    ctrl_channel_rs4xx_open_config_t * conf  = static_cast<ctrl_channel_rs4xx_open_config_t *>(param);

    // check parameter
    if ( !com || !conf || (size != sizeof(ctrl_channel_rs4xx_open_config_t)) )
    {
        return ( -EINVAL );
    }

    // close the old com port
    if ( com->getPort() != nullptr )
    {
        com->getPort()->close();
        delete com->getPort();
        com->setPort( nullptr );
    }

    // create a new serial-port instance
    port = new QSerialPort( QSerialPortInfo::availablePorts().at(conf->idx) );
    if ( port )
    {
        QSerialPort::DataBits b = QSerialPort::UnknownDataBits;
        QSerialPort::BaudRate r = QSerialPort::UnknownBaud;
        QSerialPort::Parity   p = QSerialPort::UnknownParity;
        QSerialPort::StopBits s = QSerialPort::UnknownStopBits;

        // check parameter
        switch ( conf->baudrate )
        {
            case CTRL_CHANNEL_BAUDRATE_9600:
                r = QSerialPort::Baud9600;
                break;

            case CTRL_CHANNEL_BAUDRATE_19200: 
                r = QSerialPort::Baud19200;
                break;

            case CTRL_CHANNEL_BAUDRATE_38400: 
                r = QSerialPort::Baud38400;
                break;

            case CTRL_CHANNEL_BAUDRATE_57600: 
                r = QSerialPort::Baud57600;
                break;

            case CTRL_CHANNEL_BAUDRATE_115200: 
                r = QSerialPort::Baud115200;
                break;

            default:    // unsupported 
                delete port;
                return ( -EINVAL );
        }

        switch ( conf->data )
        {
            case CTRL_CHANNEL_DATA_BITS_5:
                b = QSerialPort::Data5;
                break;

            case CTRL_CHANNEL_DATA_BITS_6:
                b = QSerialPort::Data6;
                break;

            case CTRL_CHANNEL_DATA_BITS_7:
                b = QSerialPort::Data7;
                break;

            case CTRL_CHANNEL_DATA_BITS_8:
                b = QSerialPort::Data8;
                break;

            default:    // unsupported
                delete port;
                return ( -EINVAL );
        }

        switch ( conf->parity )
        {
            case CTRL_CHANNEL_PARITY_NONE:
                p = QSerialPort::NoParity;
                break;

            case CTRL_CHANNEL_PARITY_ODD:
                p = QSerialPort::OddParity;
                break;

            case CTRL_CHANNEL_PARITY_EVEN:
                p = QSerialPort::EvenParity;
                break;

            default:    // unsupported
                delete port;
                return ( -EINVAL );
        }

        switch ( conf->stop )
        {
            case CTRL_CHANNEL_STOP_BITS_1:
                s = QSerialPort::OneStop;
                break;

            case CTRL_CHANNEL_STOP_BITS_2:
                s = QSerialPort::TwoStop;
                break;

            default:    // unsupported
                delete port;
                return ( -EINVAL );
        }
    
        // open serial port 
        if ( !port->open( QIODevice::ReadWrite ) )
        {
            qDebug( "Control channel not opened!" );
            delete port;
            return ( -EPERM );
        }

        // set no flow-control, baudrate, data bits, parity, stop bits
        if ( !port->setFlowControl( QSerialPort::NoFlowControl ) )
        {
            qDebug() << port->errorString();
            delete port;
            return ( -EFAULT );
        }

        if ( !port->setBaudRate( r ) )
        {
            qDebug() << port->errorString();
            delete port;
            return ( -EFAULT );
        }

        if ( !port->setDataBits( b ) )
        {
            qDebug() << port->errorString();
            delete port;
            return ( -EFAULT );
        }

        if ( !port->setParity( p ) )
        {
            qDebug() << port->errorString();
            delete port;
            return ( -EFAULT );
        }

        if ( !port->setStopBits( s ) )
        {
            qDebug() << port->errorString();
            delete port;
            return ( -EFAULT );
        }

        // set port instance and configuration to the new port
        com->setPortIndex( conf->idx );
        com->setNoDataBits( conf->data );
        com->setParity( conf->parity );
        com->setNoStopBits( conf->stop );
        com->setBaudRate( conf->baudrate );
        com->setDeviceAddress( conf->dev_addr );
        com->setReOpenAble( true );
        com->setPort( port );

        return ( 0 );
    }

    return ( -ENOMEM );
}

/******************************************************************************
 * ctrl_channel_qtserial_rs4xx_close
 *****************************************************************************/
static int ctrl_channel_qtserial_rs4xx_close( void * const handle )
{
    // type cast context
    ComChannelRS4xx * com = static_cast<ComChannelRS4xx *>(handle);
    if ( com )
    {
        QSerialPort * port = com->getPort();
        com->setPort( nullptr );
        if ( port )
        {
            port->close();
            delete port;
        }
    }
   
    return ( 0 );
}

/******************************************************************************
 * ctrl_channel_qtserial_rs4xx_send_request
 *****************************************************************************/
static int ctrl_channel_qtserial_rs4xx_send_request
(
    void * const    handle,
    uint8_t * const data,
    int const       len
)
{
    int res = 0;

    // type cast context
    ComChannelRS4xx * com = static_cast<ComChannelRS4xx *>(handle);
    if ( com )
    {
        QSerialPort * port = com->getPort();
        if ( port )
        {
            // send slave address
            QString s;
            s.sprintf("%d ", com->getDeviceAddress() );
            port->write( s.toUtf8() );

            /* We call the "emit data received" function here, although this is actually a data send.
             * This is done to have a local echo functionality in the debug terminal. */
            com->emitDataRecieved( s.toLocal8Bit().constData() );
            com->emitDataRecieved( reinterpret_cast<char *>(data) );

            // send command
            res = static_cast<int>(port->write( reinterpret_cast<const char *>(data), len ));
        }
    }

    return ( res );
}

/******************************************************************************
 * ctrl_channel_qtserial_rs4xx_receive_response
 *****************************************************************************/
static int ctrl_channel_qtserial_rs4xx_receive_response
(
    void * const    handle,
    uint8_t * const data,
    int const       len
)
{
    int res = 0;

    // type cast context
    ComChannelRS4xx * com = static_cast<ComChannelRS4xx *>(handle);
    if ( com )
    {
        QSerialPort * port = com->getPort();

        // Check if bytes are available, otherwise wait 1ms for new data to arrive
        if ( port )
        {
            if ( port->bytesAvailable() > 0 || port->waitForReadyRead(1) )
            {
                res = static_cast<int>(port->read( reinterpret_cast<char *>(data), len ));
                /* Mahr: After reading data, wait 250 us to give the RS485 tx/rx chips time
                 * to switch from write to read. This fixes problems where the device would not
                 * receive some commands.
                 * This problem is related to the baudrate, this fix only works for baudrates down
                 * to 38400. But the other baudrates are usually unused anyway. If the sleep value is
                 * increased to 30000 us, the problem also seems to be fixed for baudrates down to 9600
                 * but this also increases the latency of the GUI to unreasonable amounts. */
                QThread::usleep(250);

                /* Call the emitDataReceived function which will emit a dataRecieved signal, if a slot
                 * is registered for that event. This is used for the debugging terminal, it is not needed
                 * for communication with the device */
                com->emitDataRecieved( reinterpret_cast<char *>(data) );
            }
        }
    }

    return ( res );
}

/******************************************************************************
 * ComChannelSerial::getNoPorts
 *****************************************************************************/
int ComChannelSerial::getNoPorts() const
{
    return ( ctrl_channel_get_no_ports( GetInstance() ) );
}

/******************************************************************************
 * ComChannelSerial::getNoPorts
 *****************************************************************************/
int ComChannelSerial::getPortName( int idx, ctrl_channel_name_t name )
{
    return ( ctrl_channel_get_port_name( GetInstance(), idx, name ) );
}

/******************************************************************************
 * ComChannelSerial::emitDataRecieved
 *****************************************************************************/
void ComChannelSerial::emitDataRecieved( char const * data )
{
    if ( receivers(SIGNAL(dataReceived(QString))) > 0 )
    {
        emit dataReceived( QString(data) );
    }
}

/******************************************************************************
 * ComChannelSerial::onSendData
 *****************************************************************************/
void ComChannelSerial::onSendData( QString data, int responseWaitTime )
{
    /* This is used to send debugging data over the COM port, it is not
     * used for normal device communication. */
    if (m_port && m_port->isOpen() )
    {
        // Write data to port
        m_port->write( data.toLocal8Bit().constData(), data.length() );

        // Emit data received event for local echo
        emit dataReceived( data );

        // Get reply from device
        char buffer[500];
        memset( buffer, 0, sizeof(buffer) );
        QString readString;

        // Start a timer to measure how long command has been running
        QElapsedTimer timer;
        timer.start();

        // Wait for the given responseWaitTime until there is data available
        if ( m_port->bytesAvailable() <= 0 )
        {
            if ( !m_port->waitForReadyRead(responseWaitTime) )
            {
                // No data available, return
                return;
            }
        }

        // Loop while data is received
        /* Note: Long commands like dpc auto calibration or video mode only send a
         * newline ('\r\n') and then process for multiple seconds. To also catch the OK
         * response from those commands we wait for the full response wait time if
         * we have not received anything but '\r\n'.
         * Else we stop waiting if nothing new is received for 50ms, this makes the
         * debug interface more reactive and allows for fast script processing. */
        while ( m_port->bytesAvailable() > 0 || m_port->waitForReadyRead( 50 ) ||
                (readString == "\r\n" && timer.elapsed() <= responseWaitTime) )
        {
            memset( buffer, 0, sizeof(buffer) );
            int result = static_cast<int>(m_port->read( buffer, sizeof(buffer) ));

            if ( result > 0 )
            {
                readString.append( buffer );
            }
        }

        // Emit data received event
        emit dataReceived( readString );
    }
}

/******************************************************************************
 * ComChannelRS232::ComChannelRS232
 *****************************************************************************/
ComChannelRS232::ComChannelRS232( void )
{
    // register functions 
    int res = ctrl_channel_register( GetInstance(), this,
                    ctrl_channel_qtserial_get_no_ports,
                    ctrl_channel_qtserial_get_port_name,
                    ctrl_channel_qtserial_rs232_open,
                    ctrl_channel_qtserial_rs232_close,
                    ctrl_channel_qtserial_lock,
                    ctrl_channel_qtserial_release,
                    ctrl_channel_qtserial_rs232_send_request,
                    ctrl_channel_qtserial_rs232_receive_response );
    if ( res )
    {
        showError( res, __FILE__, __FUNCTION__, __LINE__ ); 
        return;
    }
}

/******************************************************************************
 * ComChannelRS232::ReOpen
 *****************************************************************************/
void ComChannelRS232::ReOpen()
{
    ctrl_channel_rs232_open_config_t cfg;

    cfg.idx        = getPortIndex();
    cfg.data       = getNoDataBits();
    cfg.parity     = getParity();
    cfg.stop       = getNoStopBits();
    cfg.baudrate   = getBaudRate();

    int res = ctrl_channel_open( GetInstance(), &cfg, sizeof(cfg) );
    if ( res )
    {
        showError( res, __FILE__, __FUNCTION__, __LINE__ ); 
        return;
    }
}

/******************************************************************************
 * ComChannelRS4xx::ComChannelRS4xx
 *****************************************************************************/
ComChannelRS4xx::ComChannelRS4xx( unsigned int dev_addr )
    : m_dev_addr( dev_addr )
{
    // register functions 
    int res = ctrl_channel_register( GetInstance(), this,
                    ctrl_channel_qtserial_get_no_ports,
                    ctrl_channel_qtserial_get_port_name,
                    ctrl_channel_qtserial_rs4xx_open,
                    ctrl_channel_qtserial_rs4xx_close,
                    ctrl_channel_qtserial_lock,
                    ctrl_channel_qtserial_release,
                    ctrl_channel_qtserial_rs4xx_send_request,
                    ctrl_channel_qtserial_rs4xx_receive_response );
    if ( res )
    {
        showError( res, __FILE__, __FUNCTION__, __LINE__ ); 
        return;
    }
}

/******************************************************************************
 * ComChannelRS4xx::ReOpen
 *****************************************************************************/
void ComChannelRS4xx::ReOpen()
{
    ctrl_channel_rs4xx_open_config_t cfg;

    cfg.idx        = getPortIndex();
    cfg.data       = getNoDataBits();
    cfg.parity     = getParity();
    cfg.stop       = getNoStopBits();
    cfg.baudrate   = getBaudRate();
    cfg.dev_addr   = static_cast<uint8_t>(getDeviceAddress());

    int res = ctrl_channel_open( GetInstance(), &cfg, sizeof(cfg) );
    if ( res )
    {
        showError( res, __FILE__, __FUNCTION__, __LINE__ ); 
        return;
    }
}
