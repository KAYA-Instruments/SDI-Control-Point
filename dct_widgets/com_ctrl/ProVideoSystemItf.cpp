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

#include <ctrl_protocol/ctrl_protocol_system.h>

#include "common.h"
#include "ProVideoSystemItf.h"

#include <QtDebug>

/******************************************************************************
 * ProVideoSystemItf::resync()
 *****************************************************************************/
void ProVideoSystemItf::resync()
{
    // make sure device buffers are flushed
    flushDeviceBuffers();

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
    // sync prompt enable-status 
    GetPrompt();
    // sync debug-level 
    GetDebugLevel();
    // sync runtime counter
    GetRunTime();
}

/******************************************************************************
 * ProVideoSystemItf::GetSystemInfo
 *****************************************************************************/
void ProVideoSystemItf::GetSystemInfo()
{
    ctrl_protocol_version_t system_info;

    memset( &system_info, 0, sizeof(system_info) );

    // read current system info
    int res = ctrl_protocol_get_system_info( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this),
                    sizeof(system_info), (uint8_t *)&system_info );
    HANDLE_ERROR( res );

    // emit a SystemPlatformChanged signal
    emit SystemPlatformChanged( QString((char *)system_info.system_platform) );

    // emit a DeviceNameChanged signal
    emit DeviceNameChanged( QString((char *)system_info.device_name) );

    // emit a DeviceIdChanged signal
    emit DeviceIdChanged( system_info.system_id[0], system_info.system_id[1],
                          system_info.system_id[2], system_info.system_id[3] );

    // emit a SystemValidityChanged signal
    emit SystemValidityChanged( QString((char *)system_info.system_validity) );

    // emit a BitStreamVersionChanged signal
    emit BitStreamVersionChanged( system_info.hw_revision );

    // emit a BootloaderVersionChanged signal
    emit BootloaderVersionChanged( system_info.loader_version[0] );

    // emit a ApplicationVersionChanged signal
    emit ApplicationVersionChanged( QString((char *)system_info.sw_release_id) );

    // emit a ApplicationReleaseDateChanged signal
    emit ApplicationReleaseDateChanged( QString((char *)system_info.sw_release_date) );

    // emit a ApplicationBuildDateChanged signal
    emit ApplicationBuildDateChanged( QString((char *)system_info.sw_build_date) );

    // emit a FeatureMaskHwChanged signal
    emit FeatureMaskHwChanged( system_info.feature_mask_HW );

    if ( m_HwMask )
    {
        emit FeatureMaskHwListChanged( m_HwMask->interpret( system_info.feature_mask_HW ) );
    }

    // emit a FeatureMaskSwChanged signal
    emit FeatureMaskSwChanged( system_info.feature_mask_SW );

    if ( m_SwMask )
    {
        emit FeatureMaskSwListChanged( m_SwMask->interpret( system_info.feature_mask_SW ) );
    }

    // emit a ResolutionMaskChanged signal
    emit ResolutionMaskChanged( system_info.resolution_mask[0], system_info.resolution_mask[1], system_info.resolution_mask[2] );
}

/******************************************************************************
 * ProVideoSystemItf::GetSystemPlatform
 *****************************************************************************/
void ProVideoSystemItf::GetSystemPlatform()
{
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
        emit DeviceNameChanged( QString((char *)device_name) );
    }
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
    int res = 0;
    uint8_t flag = 0u;

    int retries = 5;

    // Loop until prompt can be read successfully
    do
    {
        // Read prompt 5 times to make sure buffers are empty
        for ( int i = 0; i < 5; i++ )
        {
            // read current prompt configuration
            res = ctrl_protocol_get_prompt( GET_PROTOCOL_INSTANCE(this),
                        GET_CHANNEL_INSTANCE(this), &flag );
        }

        // Decrement retry counter
        retries--;
    } while ( res != 0 && retries > 0 );
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

        // emit a DebugLevelChanged signal
        emit RunTimeChanged( cnt );
    }
}

/******************************************************************************
 * ProVideoSystemItf::onBootIntoUpdateMode
 *****************************************************************************/
void ProVideoSystemItf::onBootIntoUpdateMode()
{
    // write debug level 
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
void ProVideoSystemItf::onSaveSettings()
{
    // save setting 
    int res = ctrl_protocol_save_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this) ); 
    HANDLE_ERROR( res );
}

/******************************************************************************
 * ProVideoSystemItf::onLoadSettings
 *****************************************************************************/
void ProVideoSystemItf::onLoadSettings()
{
    // load settings
    int res = ctrl_protocol_load_settings( GET_PROTOCOL_INSTANCE(this),
                    GET_CHANNEL_INSTANCE(this) ); 
    HANDLE_ERROR( res );
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

