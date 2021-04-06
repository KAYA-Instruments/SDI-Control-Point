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
 * @file    ProVideoSystemItf.h
 *
 * @brief   ProVideo System Interface
 *
 * @note    Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _PROVIDEO_SYSTEM_INTERFACE_H_
#define _PROVIDEO_SYSTEM_INTERFACE_H_

#include <QObject>

#include "ProVideoItf.h"
#include <ctrl_protocol/ctrl_protocol_system.h>

// Struct that contains connection information about a device
typedef struct rs485Device
{
    QString device_platform;
    QString device_name;
    uint8_t rs485_address;
    uint8_t rs485_bc_address;
    uint8_t rs485_bc_master;
} rs485Device;

class MaskInterpreter
{
public:
    virtual ~MaskInterpreter() {}
    virtual QStringList interpret( const uint32_t mask ) = 0;
};

class ProVideoSystemItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit ProVideoSystemItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p ),
          m_HwMask( nullptr ),
          m_SwMask( nullptr ),
          m_bSysInfoInit(false)
    {
        memset(&m_system_info, 0, sizeof(m_system_info));
    }
 
    // resync all settings
    void resync() override;

    // complete system info
    void GetSystemInfo(bool bEmitUpdate = true);

    // system platform
    void GetSystemPlatform();

    // device name 
    void GetDeviceName();

    // device identifier
    void GetDeviceId();

    // system validity string
    void GetSystemValidity();
 
    // bitstream version
    void GetBitStreamVersion();
    
    // bootloader version
    void GetBootloaderVersion();
    
    // application version
    void GetApplicationVersion();

    // application release date
    void GetApplicationReleaseDate();

    // application build date
    void GetApplicationBuildDate();

    // bitstream feature mask
    void GetFeatureMaskHw();

    // resolution masks
    void GetResolutionMask();
    
    // software/application feature mask
    void GetFeatureMaskSw();
    
    // RS232 baudrate
    void GetRS232BaudRate();

    // RS485 baudrate
    void GetRS485BaudRate();
    
    // RS485 address
    void GetRS485Address();

    // RS485 broadcast address
    void GetRS485BroadcastAddress();

    // RS485 broadcast master
    void GetRS485BroadcastMaster();

    // RS485 termination
    void GetRS485Termination();

    // list of connected devices
    void GetDeviceList( uint32_t timeout );
    
    // prompt enable status
    void GetPrompt();
    
    // debug level 
    void GetDebugLevel();

    // run-time counter
    void GetRunTime();

    // temperature
    void GetTemp( uint8_t id );
    void GetMaxTemp();
    void GetFanSpeed();
    void GetFanTarget();
    void GetOverTempCount();

    void GetDefaultSettings();

    // check for connection to device
    bool isConnected();

    // flush device buffers
    void flushDeviceBuffers();

    // set mask interpreter (hardware mask)
    void SetMaskHwInterpreter( MaskInterpreter * );
    
    // set mask interpreter (software mask)
    void SetMaskSwInterpreter( MaskInterpreter * );

signals:
    // system identifier
    void SystemPlatformChanged( QString name );
    void DeviceNameChanged( QString name );
    void DeviceIdChanged( uint32_t id0, uint32_t id1, uint32_t id2, uint32_t id3 );
    void SystemValidityChanged( QString version );
    void BitStreamVersionChanged( uint32_t version );
    void BootloaderVersionChanged( uint32_t version );
    void ApplicationVersionChanged( QString version );
    void ApplicationReleaseDateChanged( QString date );
    void ApplicationBuildDateChanged( QString date );
    void FeatureMaskHwChanged( uint32_t mask );
    void FeatureMaskHwListChanged( QStringList features );
    void FeatureMaskSwChanged( uint32_t mask );
    void ResolutionMaskChanged( uint32_t id0, uint32_t id1, uint32_t id2 );
    void FeatureMaskSwListChanged( QStringList features );
    void RS232BaudRateChanged( uint32_t baudrate );
    void RS485BaudRateChanged( uint32_t baudrate );
    void RS485AddressChanged( uint32_t address );
    void RS485BroadcastAddressChanged( uint32_t broadcast_address );
    void RS485BroadcastMasterChanged( uint8_t is_master );
    void RS485TerminationChanged( bool enable );
    void DeviceListChanged( QList<rs485Device> device_list );
    void PromptChanged( uint8_t flag );
    void DebugLevelChanged( uint8_t level );
    void RunTimeChanged( uint32_t cnt );
    void TempChanged( uint8_t id, float temp, QString name );
    void MaxTempChanged( int32_t max_temp_current_user, int32_t max_temp_current_persistent, int32_t max_temp_allowed );
    void OverTempCountChanged( uint32_t count );
    void FanSpeedChanged( uint8_t speed );
    void FanTargetChanged( uint8_t target );
    void DefaultSettingsChanged( int8_t userSetting );

public slots:
    void onDeviceNameChange( QString name );
    void onRS232BaudRateChange( uint32_t baudrate );
    void onRS485BaudRateChange( uint32_t baudrate );
    void onRS485AddressChange( uint32_t address );
    void onRS485BroadcastAddressChange( uint32_t broadcast_address );
    void onRS485BroadcastMasterChange( int32_t master_address );
    void onRS485TerminationChange( bool enabled );
    void onPromptChange( uint8_t flag );
    void onDebugLevelChange( uint8_t level );
    void onGetRunTimeRequest();
    void onGetTempRequest( uint8_t id );
    void onGetMaxTempRequest();
    void onMaxTempReset();
    void onGetOverTempCountRequest();
    void onGetFanSpeedRequest();
    void onFanTargetChange( uint8_t target );
    void onBootIntoUpdateMode();
    void onSaveSettings(int userSetting);
    void onLoadSettings(int userSetting);
    void onSetDefaultSettings(int userSetting);
    void onGetDefaultSettingsRequest();
    void onResetSettings();
    void onCopySettings(int src , int dest);

private:
    MaskInterpreter * m_HwMask;
    MaskInterpreter * m_SwMask;

    ctrl_protocol_version_t m_system_info;
    bool m_bSysInfoInit;
};

#endif // _PROVIDEO_SYSTEM_INTERFACE_H_

