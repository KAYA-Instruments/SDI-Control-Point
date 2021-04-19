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
 * @file    ProVideoSystemItf.cpp
 *
 * @brief   Implementation of Pro-Video System Interface 
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include <defines.h>
#include <ctrl_protocol/ctrl_protocol_system.h>

#include "common.h"
#include "ProVideoSystemItf.h"

#include <QThread>
#include <QRegularExpression>
#include <QTextStream>

/******************************************************************************
 * ProVideoSystemItf::resync()
 *****************************************************************************/
void ProVideoSystemItf::resync()
{
    // make sure device buffers are flushed
    // flushDeviceBuffers();

    // sync system info
    GetSystemInfo();
    // sync RS232 bausrate 
    GetRS232BaudRate();
    // sync RS485 bausrate 
    GetRS485BaudRate();
    // sync RS485 slave address 
    GetRS485Address();
    // sync RS485 broadcast address
    GetRS485BroadcastAddress();
    // sync RS485 broadcast master
    GetRS485BroadcastMaster();
    // sync RS485 termination
    GetRS485Termination();
    // sync prompt enable-status 
    GetPrompt();
    // sync debug-level 
    GetDebugLevel();
    // sync runtime counter
    GetRunTime();
    // sync target system temperature
    GetFanTarget();
    // sync default settings
    GetDefaultSettings();
}

/******************************************************************************
 * ProVideoSystemItf::GetSystemInfo
 *****************************************************************************/
void ProVideoSystemItf::GetSystemInfo(bool bEmitUpdate)
{
    // read current system info
    if(!m_bSysInfoInit)
    {
        m_bSysInfoInit = true;

        int res = ctrl_protocol_get_system_info( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this),
                    sizeof(m_system_info), (uint8_t *)&m_system_info );
        HANDLE_ERROR( res );
    }

    if(bEmitUpdate)
    {

        // emit a SystemPlatformChanged signal
        emit SystemPlatformChanged( QString((char *)m_system_info.system_platform) );

        // emit a DeviceNameChanged signal
        /* Check if device name contains valid characters. When devices come
     * fresh from the factory they might have garbage device names which can
     * crash the GUI. */
        QString device_name((char *)m_system_info.device_name);
        bool containsNonASCII = device_name.contains(QRegularExpression(QStringLiteral("[^\\x{0000}-\\x{007F}]")));
        if ( containsNonASCII )
        {
            device_name = QString("???");
        }
        emit DeviceNameChanged( device_name );

        // emit a DeviceIdChanged signal
        emit DeviceIdChanged( m_system_info.system_id[0], m_system_info.system_id[1],
                m_system_info.system_id[2], m_system_info.system_id[3] );

        // emit a SystemValidityChanged signal
        emit SystemValidityChanged( QString((char *)m_system_info.system_validity) );

        // emit a BitStreamVersionChanged signal
        emit BitStreamVersionChanged( m_system_info.hw_revision );

        // emit a BootloaderVersionChanged signal
        emit BootloaderVersionChanged( m_system_info.loader_version[0] );

        // emit a ApplicationVersionChanged signal
        emit ApplicationVersionChanged( QString((char *)m_system_info.sw_release_id) );

        emit DeviceVersionChanged( QString((char *)m_system_info.sw_release_id) );

        // emit a ApplicationReleaseDateChanged signal
        emit ApplicationReleaseDateChanged( QString((char *)m_system_info.sw_release_date) );

        // emit a ApplicationBuildDateChanged signal
        emit ApplicationBuildDateChanged( QString((char *)m_system_info.sw_build_date) );

        // emit a FeatureMaskHwChanged signal
        emit FeatureMaskHwChanged( m_system_info.feature_mask_HW );

        if ( m_HwMask )
        {
            emit FeatureMaskHwListChanged( m_HwMask->interpret( m_system_info.feature_mask_HW ) );
        }

        // emit a FeatureMaskSwChanged signal
        emit FeatureMaskSwChanged( m_system_info.feature_mask_SW );

        if ( m_SwMask )
        {
            emit FeatureMaskSwListChanged( m_SwMask->interpret( m_system_info.feature_mask_SW ) );
        }

        // emit a ResolutionMaskChanged signal
        emit ResolutionMaskChanged( m_system_info.resolution_mask[0], m_system_info.resolution_mask[1], m_system_info.resolution_mask[2] );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetSystemPlatform
 *****************************************************************************/
void ProVideoSystemItf::GetSystemPlatform()
{
    if(!m_bSysInfoInit)
    {
        GetSystemInfo(false);
    }

    emit SystemPlatformChanged( QString((char *)m_system_info.system_platform) );
    return;

    /*
    // Is there a signal listener
    if ( receivers(SIGNAL(SystemPlatformChanged(QString))) > 0 )
    {
        ctrl_protocol_system_desc_t system_platform;

        memset( system_platform, 0, sizeof(system_platform) );

        // read current device identifier
        int res = ctrl_protocol_get_system_platform( GET_PROTOCOL_INSTANCE(this),
                        GET_CHANNEL_INSTANCE(this),
                        sizeof(system_platform), (uint8_t *)system_platform );
        HANDLE_ERROR( res );

        // emit a SystemPlatformChanged signal
        emit SystemPlatformChanged( QString((char *)system_platform) );
    }
    */
}

/******************************************************************************
 * ProVideoSystemItf::GetDeviceName
 *****************************************************************************/
void ProVideoSystemItf::GetDeviceName()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(DeviceNameChanged(QString))) > 0 )
    {
        ctrl_protocol_system_desc_t device_name;

        memset( device_name, 0, sizeof(device_name) );

        // read current device identifier 
        int res = ctrl_protocol_get_device_name( GET_PROTOCOL_INSTANCE(this),
                        GET_CHANNEL_INSTANCE(this), 
                        sizeof(device_name), (uint8_t *)device_name );
        HANDLE_ERROR( res );

        // emit a DeviceNameChanged signal
        /* Check if device name contains valid characters. When devices come
         * fresh from the factory they might have garbage device names which can
         * crash the GUI. */
        QString device_name_str((char *)device_name);
        bool containsNonASCII = device_name_str.contains(QRegularExpression(QStringLiteral("[^\\x{0000}-\\x{007F}]")));
        if ( containsNonASCII )
        {
            device_name_str = QString("???");
        }
        emit DeviceNameChanged( device_name_str );
    }
}

/******************************************************************************
 * ProVideoSystemItf::onDeviceNameChange
 *****************************************************************************/
void ProVideoSystemItf::onDeviceNameChange( QString name )
{
    // Copy device name to array
    ctrl_protocol_system_desc_t device_name;
    memset( device_name, 0, sizeof(device_name) );

    std::string str = name.toStdString();

    strcpy( (char*)device_name, str.c_str());

    // send device name to device
    int res = ctrl_protocol_set_device_name( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), name.length(), (uint8_t *)device_name );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::GetDeviceId
 *****************************************************************************/
void ProVideoSystemItf::GetDeviceId()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(DeviceIdChanged(uint32_t,uint32_t,uint32_t,uint32_t))) > 0 )
    {
        ctrl_protocol_device_id_t system_id;

        memset( &system_id, 0, sizeof(system_id) );

        // read current device identifier 
        int res = ctrl_protocol_get_system_id( GET_PROTOCOL_INSTANCE(this),
                        GET_CHANNEL_INSTANCE(this), 
                        sizeof(system_id), (uint8_t *)&system_id );
        HANDLE_ERROR( res );

        // emit a DeviceIdChanged signal
        emit DeviceIdChanged( system_id.id0, system_id.id1, system_id.id2, system_id.id3 );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetSystemValidity
 *****************************************************************************/
void ProVideoSystemItf::GetSystemValidity()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(SystemValidityChanged(QString))) > 0 )
    {
        ctrl_protocol_system_desc_t validity;

        // clear memory
        memset( validity, 0, sizeof(validity) );

        // read current system validity string 
        int res = ctrl_protocol_get_system_validity( GET_PROTOCOL_INSTANCE(this),
                        GET_CHANNEL_INSTANCE(this), sizeof(validity), (uint8_t *)validity ); 
        HANDLE_ERROR( res );

        // emit a SystemValidityChanged signal
        emit SystemValidityChanged( QString((char *)validity) );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetBitStreamVersion
 *****************************************************************************/
void ProVideoSystemItf::GetBitStreamVersion()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(BitStreamVersionChanged(uint32_t))) > 0 )
    {
        uint32_t version = 0xFFFFFFFFu;

        // read current bit-stream version
        int res = ctrl_protocol_get_bitstream_version( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &version ); 
        HANDLE_ERROR( res );

        // emit a BitStreamVersionChanged signal
        emit BitStreamVersionChanged( version );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetBootloaderVersion
 *****************************************************************************/
void ProVideoSystemItf::GetBootloaderVersion()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(BootloaderVersionChanged(uint32_t))) > 0 )
    {
        uint32_t version = 0xFFFFFFFFu;

        // read current boot-loader version
        int res = ctrl_protocol_get_bootloader_version( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &version ); 
        HANDLE_ERROR( res );

        // emit a BootloaderVersionChanged signal
        emit BootloaderVersionChanged( version );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetApplicationVersion
 *****************************************************************************/
void ProVideoSystemItf::GetApplicationVersion()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ApplicationVersionChanged(QString))) > 0 )
    {
        ctrl_protocol_system_desc_t version;

        // clear memory
        memset( version, 0, sizeof(version) );

        // read current application/firmware version 
        int res = ctrl_protocol_get_application_version( GET_PROTOCOL_INSTANCE(this),
                        GET_CHANNEL_INSTANCE(this), sizeof(version), (uint8_t *)version ); 
        HANDLE_ERROR( res );

        // emit a ApplicationVersionChanged signal
        emit ApplicationVersionChanged( QString((char *)version) );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetApplicationReleaseDate
 *****************************************************************************/
void ProVideoSystemItf::GetApplicationReleaseDate()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ApplicationReleaseDateChanged(QString))) > 0 )
    {
        ctrl_protocol_system_desc_t date;

        // clear memory
        memset( date, 0, sizeof(date) );

        // read current application/firmware release date
        int res = ctrl_protocol_get_application_release_date( GET_PROTOCOL_INSTANCE(this),
                        GET_CHANNEL_INSTANCE(this), sizeof(date), (uint8_t *)date );
        HANDLE_ERROR( res );

        // emit a ApplicationReleaseDateChanged signal
        emit ApplicationReleaseDateChanged( QString((char *)date) );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetApplicationBuildDate
 *****************************************************************************/
void ProVideoSystemItf::GetApplicationBuildDate()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ApplicationBuildDateChanged(QString))) > 0 )
    {
        ctrl_protocol_system_desc_t date;

        // clear memory
        memset( date, 0, sizeof(date) );

        // read current application/firmware build date
        int res = ctrl_protocol_get_application_build_date( GET_PROTOCOL_INSTANCE(this),
                        GET_CHANNEL_INSTANCE(this), sizeof(date), (uint8_t *)date );
        HANDLE_ERROR( res );

        // emit a ApplicationBuildDateChanged signal
        emit ApplicationBuildDateChanged( QString((char *)date) );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetFeatureMaskHw
 *****************************************************************************/
void ProVideoSystemItf::GetFeatureMaskHw()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(FeatureMaskHwChanged(uint32_t))) > 0 )
    {
        uint32_t mask = 0xFFFFFFFFu;

        // read current hardware feature mask
        int res = ctrl_protocol_get_feature_mask_hw( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &mask ); 
        HANDLE_ERROR( res );

        // emit a FeatureMaskHwChanged signal
        emit FeatureMaskHwChanged( mask );

        if ( m_HwMask )
        {
            emit FeatureMaskHwListChanged( m_HwMask->interpret( mask ) );
        }
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetFeatureMaskSw
 *****************************************************************************/
void ProVideoSystemItf::GetFeatureMaskSw()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(FeatureMaskSwChanged(uint32_t))) > 0 )
    {
        uint32_t mask = 0xFFFFFFFFu;

        // read current software feature mask
        int res = ctrl_protocol_get_feature_mask_sw( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &mask ); 
        HANDLE_ERROR( res );

        // emit a FeatureMaskSwChanged signal
        emit FeatureMaskSwChanged( mask );

        if ( m_SwMask )
        {
            emit FeatureMaskSwListChanged( m_SwMask->interpret( mask ) );
        }
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetResolutionMask
 *****************************************************************************/
void ProVideoSystemItf::GetResolutionMask()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(ResolutionMaskChanged(uint32_t,uint32_t,uint32_t))) > 0 )
    {
        ctrl_protocol_resolution_id_t res_id;

        memset( &res_id, 0, sizeof(ctrl_protocol_resolution_id_t) );

        // read current device identifier 
        int res = ctrl_protocol_get_resolution_mask( GET_PROTOCOL_INSTANCE(this),
                        GET_CHANNEL_INSTANCE(this), 
                        sizeof(ctrl_protocol_resolution_id_t), (uint8_t *)&res_id );
        HANDLE_ERROR( res );

        // emit a ResolutionMaskChanged signal
        emit ResolutionMaskChanged( res_id.id0, res_id.id1, res_id.id2 );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetRS232BaudRate
 *****************************************************************************/
void ProVideoSystemItf::GetRS232BaudRate()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(RS232BaudRateChanged(uint32_t))) > 0 )
    {
        uint32_t baudrate = 0u;

        // read current RS232 baud-rate configuration
        int res = ctrl_protocol_get_rs232_baud( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &baudrate ); 
        HANDLE_ERROR( res );

        // emit a RS232BaudRateChanged signal
        emit RS232BaudRateChanged( baudrate );
    }
}

/******************************************************************************
 * ProVideoSystemItf::onRS232BaudRateChange
 *****************************************************************************/
void ProVideoSystemItf::onRS232BaudRateChange( uint32_t baudrate )
{
    // write RS232 baud-rate configuration 
    int res = ctrl_protocol_set_rs232_baud( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), baudrate ); 
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::GetRS485BaudRate
 *****************************************************************************/
void ProVideoSystemItf::GetRS485BaudRate()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(RS485BaudRateChanged(uint32_t))) > 0 )
    {
        uint32_t baudrate = 0u;

        // read current RS485 baud-rate configuration
        int res = ctrl_protocol_get_rs485_baud( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &baudrate ); 
        HANDLE_ERROR( res );

        // emit a RS485BaudRateChanged signal
        emit RS485BaudRateChanged( baudrate );
    }
}

/******************************************************************************
 * ProVideoSystemItf::onRS485BaudRateChange
 *****************************************************************************/
void ProVideoSystemItf::onRS485BaudRateChange( uint32_t baudrate )
{
    // write RS485 baud-rate configuration 
    int res = ctrl_protocol_set_rs485_baud( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), baudrate ); 
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::GetRS485Address
 *****************************************************************************/
void ProVideoSystemItf::GetRS485Address()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(RS485AddressChanged(uint32_t))) > 0 )
    {
        uint32_t address = 0u;

        // read current RS485 slave address
        int res = ctrl_protocol_get_rs485_addr( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &address ); 
        HANDLE_ERROR( res );

        // emit a RS485AddressChanged signal
        emit RS485AddressChanged( address );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetRS485BroadcastAddress
 *****************************************************************************/
void ProVideoSystemItf::GetRS485BroadcastAddress()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(RS485BroadcastAddressChanged(uint32_t))) > 0 )
    {
        uint32_t broadcast_address = -1u;

        // read current RS485 broadcast address
        int res = ctrl_protocol_get_rs485_bc_addr( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &broadcast_address );
        HANDLE_ERROR( res );

        // emit a RS485BroadcastAddressChanged signal
        emit RS485BroadcastAddressChanged( broadcast_address );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetRS485BroadcastMaster
 *****************************************************************************/
void ProVideoSystemItf::GetRS485BroadcastMaster()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(RS485BroadcastMasterChanged(uint8_t))) > 0 )
    {
        uint8_t is_master = 0u;

        // read current RS485 slave address
        int res = ctrl_protocol_get_rs485_bc_master( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &is_master );
        HANDLE_ERROR( res );

        // emit a RS485BroadcastMasterChanged signal
        emit RS485BroadcastMasterChanged( is_master );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetRS485Termination
 *****************************************************************************/
void ProVideoSystemItf::GetRS485Termination()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(RS485TerminationChanged(bool))) > 0 )
    {
        uint8_t is_enabled = 0u;

        // read current RS485 slave address
        int res = ctrl_protocol_get_rs485_termination( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &is_enabled );
        HANDLE_ERROR( res );

        // emit a RS485TerminationChanged signal
        emit RS485TerminationChanged( is_enabled > 0 ? true : false );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetDeviceList
 *****************************************************************************/
void ProVideoSystemItf::GetDeviceList( uint32_t timeout )
{
    // Is there a signal listener
    if ( receivers(SIGNAL(DeviceListChanged(QList<rs485Device>))) > 0 )
    {
        // Flush buffers
        flushDeviceBuffers();

        // Up to MAX_DEVICE_ID + 1 devices can be detected (because 0 is a valid address too)
        ctrl_protocol_device_t devices[MAX_DEVICE_ID + 1];

        memset( &devices, 0, sizeof(devices) );

        // Write timeout to buffer, it will be read by the provideo protcol get_device_list() function
        *((uint32_t*)devices) = timeout;

        // read device list
        int res = ctrl_protocol_get_device_list( GET_PROTOCOL_INSTANCE(this),
                        GET_CHANNEL_INSTANCE(this),
                        sizeof(devices), (uint8_t *)&devices );

        // Convert to QList, if get device list command was succesfull
        QList<rs485Device> deviceList;
        if ( res == 0 )
        {
            for ( int i = 0; i <= MAX_DEVICE_ID; i++ )
            {
                // If entry contains a valid device
                if ( devices[i].device_platform[0] != 0u )
                {
                    // Add it to list
                    rs485Device device;
                    device.device_name = QString::fromLocal8Bit( (char*)devices[i].device_name );

                    /* Check if device name contains valid characters. When devices come
                     * fresh from the factory they might have garbage device names which can
                     * crash the GUI. */
                    bool containsNonASCII = device.device_name.contains(QRegularExpression(QStringLiteral("[^\\x{0000}-\\x{007F}]")));
                    if ( containsNonASCII )
                    {
                        device.device_name = QString("???");
                    }

                    device.device_platform = QString::fromLocal8Bit( (char*)devices[i].device_platform );
                    device.rs485_address = devices[i].rs485_address;
                    device.rs485_bc_address = devices[i].rs485_bc_address;
                    device.rs485_bc_master = devices[i].rs485_bc_master;
                    deviceList.append( device );
                }
                // Else: Last valid device was found, stop iterating over the array
                else
                {
                    break;
                }
            }
        }

        // emit a DeviceIdChanged signal
        emit DeviceListChanged( deviceList );

        /* Handle error at the end of this function, so that DeviceListChanged
         * is always emitted, even if the list is empty */
        HANDLE_ERROR( res );
    }
}

/******************************************************************************
 * ProVideoSystemItf::isConnected
 *****************************************************************************/
bool ProVideoSystemItf::isConnected()
{
    uint8_t flag = 0u;

    // read current prompt configuration
    int res = ctrl_protocol_get_prompt( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &flag );

    return ( res ? false : true );
}

/******************************************************************************
 * ProVideoSystemItf::flushDeviceBuffers
 *****************************************************************************/
void ProVideoSystemItf::flushDeviceBuffers()
{
    uint8_t data[32];

    // Flush buffer of com port by reading from it until there is no data left
    while ( ctrl_channel_receive_response( GET_CHANNEL_INSTANCE(this), data, sizeof(data) ) != 0 ) {};

    // Flush buffers of device
    int res = ctrl_protocol_flush_buffers( GET_PROTOCOL_INSTANCE(this),
                                           GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );

    // Wait 10 ms
    QThread::msleep(10);

    // Flush buffer of com port by reading from it until there is no data left
    while ( ctrl_channel_receive_response( GET_CHANNEL_INSTANCE(this), data, sizeof(data) ) != 0 ) {};
}

/******************************************************************************
 * ProVideoSystemItf::SetMaskHwInterpreter
 *****************************************************************************/
void ProVideoSystemItf::SetMaskHwInterpreter( MaskInterpreter * interpreter )
{
    m_HwMask = interpreter;
}

/******************************************************************************
 * ProVideoSystemItf::SetMaskSwInterpreter
 *****************************************************************************/
void ProVideoSystemItf::SetMaskSwInterpreter( MaskInterpreter * interpreter )
{
    m_SwMask = interpreter;
}

/******************************************************************************
 * ProVideoSystemItf::onRS485AddressChange
 *****************************************************************************/
void ProVideoSystemItf::onRS485AddressChange( uint32_t address )
{
    // write RS485 slave address 
    int res = ctrl_protocol_set_rs485_addr( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), address ); 
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::onRS485BroadcastAddressChange
 *****************************************************************************/
void ProVideoSystemItf::onRS485BroadcastAddressChange( uint32_t broadcast_address )
{
    // write RS485 broadcast address
    int res = ctrl_protocol_set_rs485_bc_addr( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), broadcast_address );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::onRS485BroadcastAddressChange
 *****************************************************************************/
void ProVideoSystemItf::onRS485BroadcastMasterChange( int32_t master_address )
{
    // write RS485 broadcast address
    int res = ctrl_protocol_set_rs485_bc_master( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), master_address );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::onRS485BroadcastAddressChange
 *****************************************************************************/
void ProVideoSystemItf::onRS485TerminationChange( bool enabled )
{
    // write RS485 termination
    int res = ctrl_protocol_set_rs485_termination( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), enabled ? 1 : 0 );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::GetPrompt
 *****************************************************************************/
void ProVideoSystemItf::GetPrompt()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(PromptChanged(uint8_t))) > 0 )
    {
        uint8_t flag = 0u;

        // read current prompt configuration 
        int res = ctrl_protocol_get_prompt( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &flag ); 
        HANDLE_ERROR( res );

        // emit a PromptChanged signal
        emit PromptChanged( flag );
    }
}

/******************************************************************************
 * ProVideoSystemItf::onPromptChange
 *****************************************************************************/
void ProVideoSystemItf::onPromptChange( uint8_t flag )
{
    // write prompt configuration 
    int res = ctrl_protocol_set_prompt( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), flag ); 
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::GetDebugLevel
 *****************************************************************************/
void ProVideoSystemItf::GetDebugLevel()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(DebugLevelChanged(uint8_t))) > 0 )
    {
        uint8_t level = 0u;

        // read current debug level 
        int res = ctrl_protocol_get_debug( GET_PROTOCOL_INSTANCE(this), 
                    GET_CHANNEL_INSTANCE(this), &level ); 
        HANDLE_ERROR( res );

        // emit a DebugLevelChanged signal
        emit DebugLevelChanged( level );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetRunTime
 *****************************************************************************/
void ProVideoSystemItf::GetRunTime()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(RunTimeChanged(uint32_t))) > 0 )
    {
        uint32_t cnt = 0u;

        // read current runtime counter
        int res = ctrl_protocol_get_runtime( GET_PROTOCOL_INSTANCE(this), 
                    GET_CHANNEL_INSTANCE(this), &cnt ); 
        HANDLE_ERROR( res );

        // emit a RunTimeChanged signal
        emit RunTimeChanged( cnt );
    }
}

/******************************************************************************
 * ProVideoSystemItf::onGetRunTimeRequest
 *****************************************************************************/
void ProVideoSystemItf::onGetRunTimeRequest()
{
    GetRunTime();
}

/******************************************************************************
 * ProVideoSystemItf::GetTemp
 *****************************************************************************/
void ProVideoSystemItf::GetTemp( uint8_t id )
{
    // Is there a signal listener
    if ( receivers(SIGNAL(TempChanged(uint8_t,float,QString))) > 0 )
    {
        ctrl_protocol_temp_t temp;
        temp.id = id;
        temp.temp = 0;
        strcpy( temp.name, "" );

        // read current temperature of the given id
        int res = ctrl_protocol_get_temp( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), sizeof(temp), (uint8_t *)&temp );
        HANDLE_ERROR( res );

        // emit a TempChanged signal
        emit TempChanged( temp.id, temp.temp, QString::fromLocal8Bit(temp.name) );
    }
}

/******************************************************************************
 * ProVideoSystemItf::onGetTempRequest
 *****************************************************************************/
void ProVideoSystemItf::onGetTempRequest( uint8_t id )
{
    GetTemp( id );
}

/******************************************************************************
 * ProVideoSystemItf::GetMaxTemp
 *****************************************************************************/
void ProVideoSystemItf::GetMaxTemp()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(MaxTempChanged(int32_t, int32_t, int32_t))) > 0 )
    {
        int32_t values[3];

        // read maximum logged temperature
        int res = ctrl_protocol_get_max_temp( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), 3, values );
        HANDLE_ERROR( res );

        // emit a MaxTempChanged signal
        emit MaxTempChanged( values[0], values[1], values[2] );
    }
}

/******************************************************************************
 * ProVideoSystemItf::onGetMaxTempRequest
 *****************************************************************************/
void ProVideoSystemItf::onGetMaxTempRequest()
{
    GetMaxTemp();
}

/******************************************************************************
 * ProVideoSystemItf::onMaxTempReset
 *****************************************************************************/
void ProVideoSystemItf::onMaxTempReset()
{
    // reset maximum temperature
    int res = ctrl_protocol_max_temp_reset( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );
}


/******************************************************************************
 * ProVideoSystemItf::GetFanSpeed
 *****************************************************************************/
void ProVideoSystemItf::GetFanSpeed()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(FanSpeedChanged(uint8_t))) > 0 )
    {
        uint8_t speed = 0u;

        // read current fan speed
        int res = ctrl_protocol_get_fan_speed( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &speed );
        HANDLE_ERROR( res );

        // emit a OverTempCountChanged signal
        emit FanSpeedChanged( speed );
    }
}

/******************************************************************************
 * ProVideoSystemItf::GetFanTarget
 *****************************************************************************/
void ProVideoSystemItf::GetFanTarget()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(FanTargetChanged(uint8_t))) > 0 )
    {
        uint8_t target = 0u;

        // read current fan target temperature
        int res = ctrl_protocol_get_fan_target( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &target );
        HANDLE_ERROR( res );

        // emit a RS485BaudRateChanged signal
        emit FanTargetChanged( target );
    }
}

/******************************************************************************
 * ProVideoSystemItf::onFanTargetChange
 *****************************************************************************/
void ProVideoSystemItf::onFanTargetChange( uint8_t target )
{
    // write fan target temperature
    int res = ctrl_protocol_set_fan_target( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), target );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::onGetFanSpeedRequest
 *****************************************************************************/
void ProVideoSystemItf::onGetFanSpeedRequest()
{
    GetFanSpeed();
}

/******************************************************************************
 * ProVideoSystemItf::GetOverTempCount
 *****************************************************************************/
void ProVideoSystemItf::GetOverTempCount()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(OverTempCountChanged(uint32_t))) > 0 )
    {
        uint32_t count = 0u;

        // read over temperature counter
        int res = ctrl_protocol_get_over_temp_count( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &count );
        HANDLE_ERROR( res );

        // emit a OverTempCountChanged signal
        emit OverTempCountChanged( count );
    }
}

/******************************************************************************
 * ProVideoSystemItf::onGetOverTempCountRequest
 *****************************************************************************/
void ProVideoSystemItf::onGetOverTempCountRequest()
{
    GetOverTempCount();
}

/******************************************************************************
 * ProVideoSystemItf::onBootIntoUpdateMode
 *****************************************************************************/
void ProVideoSystemItf::onBootIntoUpdateMode()
{
    // boot into update mode
    int res = ctrl_protocol_update( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::onDebugLevelChange
 *****************************************************************************/
void ProVideoSystemItf::onDebugLevelChange( uint8_t level )
{
    // write debug level 
    int res = ctrl_protocol_set_debug( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), level ); 
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::onSaveSettings
 *****************************************************************************/
void ProVideoSystemItf::onSaveSettings(int userSetting)
{
    // save setting 
    int res = ctrl_protocol_save_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), uint8_t(userSetting) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::onLoadSettings
 *****************************************************************************/
void ProVideoSystemItf::onLoadSettings(int userSetting)
{
    // load settings
    int res = ctrl_protocol_load_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), uint8_t(userSetting) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::onSetDefaultSettings
 *****************************************************************************/
void ProVideoSystemItf::onSetDefaultSettings(int userSetting)
{
    // default settings
    int res = ctrl_protocol_set_default_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), uint8_t(userSetting) );
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::onGetDefaultSettingsRequest
 *****************************************************************************/
void ProVideoSystemItf::onGetDefaultSettingsRequest()
{
    GetDefaultSettings();
}

/******************************************************************************
 * ProVideoSystemItf::GetDefaultSettings
 *****************************************************************************/
void ProVideoSystemItf::GetDefaultSettings()
{
    // Is there a signal listener
    if ( receivers(SIGNAL(DefaultSettingsChanged(int8_t))) > 0 )
    {
        int8_t userSetting = -1;

        // read current default setting
        int res = ctrl_protocol_get_default_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), &userSetting );
        HANDLE_ERROR( res );

        emit DefaultSettingsChanged( userSetting );
    }
}

/******************************************************************************
 * ProVideoSystemItf::onResetSettings
 *****************************************************************************/
void ProVideoSystemItf::onResetSettings()
{
    // reset settings
    int res = ctrl_protocol_reset_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this) ); 
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::onCopySettings
 *****************************************************************************/
void ProVideoSystemItf::onCopySettings( int src, int dest )
{
    uint8_t v[2] = { (uint8_t)src, (uint8_t)dest };

    // reset settings
    int res = ctrl_protocol_copy_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this), sizeof(v), v ); 
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::GetSavedSettingsToFile
 *****************************************************************************/
void ProVideoSystemItf::GetSavedSettingsToFile(QFile & file)
{
    ctrl_protocol_system_settings_desc_t settings;
    //memset( settings, 0, sizeof(settings) );

    // read current system string
    int res = ctrl_protocol_save_settings_to_file( GET_PROTOCOL_INSTANCE(this),
              GET_CHANNEL_INSTANCE(this), sizeof(settings), (uint8_t *)&settings );
    HANDLE_ERROR( res );

    QTextStream stream(&file);

    stream << settings;
}

/******************************************************************************
 * ProVideoSystemItf::GetHwMask
 *****************************************************************************/
uint32_t ProVideoSystemItf::GetHwMask()
{
    return m_system_info.feature_mask_HW;
}

/******************************************************************************
 * ProVideoSystemItf::GetSwMask
 *****************************************************************************/
uint32_t ProVideoSystemItf::GetSwMask()
{
    return m_system_info.feature_mask_SW;
}
