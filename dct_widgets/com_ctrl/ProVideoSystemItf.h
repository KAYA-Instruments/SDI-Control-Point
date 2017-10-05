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

class MaskInterpreter
{
public:
    virtual QStringList interpret( const uint32_t mask ) = 0;
};

class ProVideoSystemItf : public ProVideoItf
{
    Q_OBJECT

public:
    explicit ProVideoSystemItf( ComChannel * c, ComProtocol * p )
        : ProVideoItf( c, p ),
          m_HwMask( NULL ),
          m_SwMask( NULL )
    { }
 
    // resync all settings
    void resync() override;

    // complete system info
    void GetSystemInfo();

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
    
    // prompt enable status
    void GetPrompt();
    
    // debug level 
    void GetDebugLevel();

    // run-time counter
    void GetRunTime();

    // check for connection to device
    bool isConnected();

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
    void PromptChanged( uint8_t flag );
    void DebugLevelChanged( uint8_t level );
    void RunTimeChanged( uint32_t cnt );

public slots:
    void onRS232BaudRateChange( uint32_t baudrate );
    void onRS485BaudRateChange( uint32_t baudrate );
    void onRS485AddressChange( uint32_t address );
    void onRS485BroadcastAddressChange( uint32_t broadcast_address );
    void onRS485BroadcastMasterChange( int32_t master_address );
    void onPromptChange( uint8_t flag );
    void onDebugLevelChange( uint8_t level );
    void onBootIntoUpdateMode();
    void onSaveSettings();
    void onLoadSettings();
    void onResetSettings();
    void onCopySettings(int src , int dest);

private:
    MaskInterpreter * m_HwMask;
    MaskInterpreter * m_SwMask;
};

#define CONNECT_PROVIDEO_SYSTEM_INTERFACE(x, y)                                         \
{                                                                                       \
    QObject::connect( x, SIGNAL(DeviceIdChanged(uint32_t,uint32_t,uint32_t,uint32_t)),  \
                      y, SLOT(onDeviceIdChange(uint32_t,uint32_t,uint32_t,uint32_t)) ); \
    QObject::connect( x, SIGNAL(SystemValidityChanged(QString)),                        \
                      y, SLOT(onSystemValidityChange(QString)) );                       \
    QObject::connect( x, SIGNAL(BitStreamVersionChanged(uint32_t)),                     \
                      y, SLOT(onBitStreamVersionChange(uint32_t)) );                    \
    QObject::connect( x, SIGNAL(BootloaderVersionChanged(uint32_t)),                    \
                      y, SLOT(onBootloaderVersionChange(uint32_t)) );                   \
    QObject::connect( x, SIGNAL(ApplicationVersionChanged(QString)),                    \
                      y, SLOT(onApplicationVersionChange(QString)) );                   \
    QObject::connect( x, SIGNAL(ApplicationReleaseDateChanged(QString)),                \
                      y, SLOT(onApplicationReleaseDateChange(QString)) );               \
    QObject::connect( x, SIGNAL(ApplicationBuildDateChanged(QString)),                  \
                      y, SLOT(onApplicationBuildDateChange(QString)) );                 \
    QObject::connect( x, SIGNAL(FeatureMaskHwChanged(uint32_t)),                        \
                      y, SLOT(onFeatureMaskHwChange(uint32_t)) );                       \
    QObject::connect( x, SIGNAL(FeatureMaskSwChanged(uint32_t)),                        \
                      y, SLOT(onFeatureMaskSwChange(uint32_t)) );                       \
    QObject::connect( x, SIGNAL(RS232BaudRateChanged(uint32_t)),                        \
                      y, SLOT(onRS232BaudRateChange(uint32_t)) );                       \
    QObject::connect( x, SIGNAL(RS485BaudRateChanged(uint32_t)),                        \
                      y, SLOT(onRS485BaudRateChange(uint32_t)) );                       \
    QObject::connect( x, SIGNAL(RS485AddressChanged(uint32_t)),                         \
                      y, SLOT(onRS485AddressChange(uint32_t)) );                        \
    QObject::connect( x, SIGNAL(RS485BroadcastAddressChanged(int32_t)),                 \
                      y, SLOT(onRS485BroadcastAddressChange(int32_t)) );                \
    QObject::connect( x, SIGNAL(RS485BroadcastMasterChanged(uint8_t)),                  \
                      y, SLOT(onRS485BroadcastMasterChange(uint8_t)) );                 \
    QObject::connect( x, SIGNAL(PromptChanged(uint8_t)),                                \
                      y, SLOT(onPromptChange(uint8_t)) );                               \
    QObject::connect( x, SIGNAL(DebugLevelChanged(uint8_t)),                            \
                      y, SLOT(onDebugLevelChange(uint8_t)) );                           \
}

#endif // _PROVIDEO_SYSTEM_INTERFACE_H_

