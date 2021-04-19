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
 * @file    ProVideoDevice.cpp
 *
 * @brief   
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include "common.h"
#include "ProVideoItf.h"
#include "ProVideoSystemItf.h"
#include "ProVideoDevice.h"

#include <QtDebug>

/******************************************************************************
 * local definitions
 *****************************************************************************/

/******************************************************************************
 * ProVideoDevice::PrivateData
 *****************************************************************************/
class ProVideoDevice::PrivateData
{
public:
    PrivateData( ComChannel * c, ComProtocol * p )
    {
        m_systemPlatform = "unknown platform";
        m_deviceName = "unknown device";
        m_deviceVersion = "unknown version";
        m_broadcastAddress = 0;
        m_isBroadcastMaster = false;
        m_ProVideoSystemItf = new ProVideoSystemItf( c, p );
    }

    ~PrivateData()
    {
        delete m_ProVideoSystemItf;
    }

    QString m_systemPlatform;
    QString m_deviceName;
    QString m_deviceVersion;
    unsigned int m_broadcastAddress;
    bool m_isBroadcastMaster;
    QList<rs485Device> m_deviceList;
    ProVideoSystemItf * m_ProVideoSystemItf;
};

/******************************************************************************
 * ProVideoDevice::ProVideoDevice()
 *****************************************************************************/
ProVideoDevice::ProVideoDevice( ComChannel * c, ComProtocol * p )
{
    d_data = new PrivateData( c, p );

    // Connect signals to copy device parameters to local class variables on change
    connect( GetProVideoSystemItf(), SIGNAL(SystemPlatformChanged(QString)), this, SLOT(onSystemPlatformChange(QString)) );
    connect( GetProVideoSystemItf(), SIGNAL(DeviceNameChanged(QString)), this, SLOT(onDeviceNameChange(QString)) );
    connect( GetProVideoSystemItf(), SIGNAL(DeviceVersionChanged(QString)), this, SLOT(onDeviceVersionChange(QString)) );
    connect( GetProVideoSystemItf(), SIGNAL(RS485BroadcastAddressChanged(uint32_t)), this, SLOT(onBroadcastAddressChange(uint32_t)) );
    connect( GetProVideoSystemItf(), SIGNAL(RS485BroadcastMasterChanged(uint8_t)), this, SLOT(onBroadcastMasterModeChange(uint8_t)) );
    connect( GetProVideoSystemItf(), SIGNAL(DeviceListChanged(QList<rs485Device>)), this, SLOT(onDeviceListChange(QList<rs485Device>)) );
}

/******************************************************************************
 * ProVideoDevice::~ProVideoDevice()
 *****************************************************************************/
ProVideoDevice::~ProVideoDevice()
{
    delete d_data;
}

/******************************************************************************
 * ProVideoDevice::getSupportedFeatures()
 *****************************************************************************/
ProVideoDevice::features ProVideoDevice::getSupportedFeatures()
{
    // This generic device does not support any features
    features deviceFeatures;
    memset(&deviceFeatures, 0, sizeof (deviceFeatures));

    return deviceFeatures;
}

/******************************************************************************
 * ProVideoDevice::GetProVideoSystemItf()
 *****************************************************************************/
ProVideoSystemItf * ProVideoDevice::GetProVideoSystemItf() const
{
    return ( d_data->m_ProVideoSystemItf );
}

/******************************************************************************
 * ProVideoDevice::GetIspItf()
 *****************************************************************************/
IspItf * ProVideoDevice::GetIspItf() const
{
    return nullptr;
}

/******************************************************************************
 * ProVideoDevice::GetCprocItf()
 *****************************************************************************/
CprocItf * ProVideoDevice::GetCprocItf() const
{
    return nullptr;
}

/******************************************************************************
 * ProVideoDevice::GetAutoItf()
 *****************************************************************************/
AutoItf * ProVideoDevice::GetAutoItf() const
{
    return nullptr;
}

/******************************************************************************
 * ProVideoDevice::GetCamItf()
 *****************************************************************************/
CamItf * ProVideoDevice::GetCamItf() const
{
    return nullptr;
}

/******************************************************************************
 * ProVideoDevice::GetMccItf()
 *****************************************************************************/
MccItf * ProVideoDevice::GetMccItf() const
{
    return nullptr;
}

/******************************************************************************
 * ProVideoDevice::GetLutItf()
 *****************************************************************************/
LutItf * ProVideoDevice::GetLutItf() const
{
    return nullptr;
}

/******************************************************************************
 * ProVideoDevice::GetChainItf()
 *****************************************************************************/
ChainItf * ProVideoDevice::GetChainItf() const
{
    return nullptr;
}

/******************************************************************************
 * ProVideoDevice::GetIrisItf()
 *****************************************************************************/
IrisItf * ProVideoDevice::GetIrisItf() const
{
    return nullptr;
}

/******************************************************************************
 * ProVideoDevice::GetLensItf()
 *****************************************************************************/
LensItf * ProVideoDevice::GetLensItf() const
{
    return nullptr;
}

/******************************************************************************
 * ProVideoDevice::GetKneeItf()
 *****************************************************************************/
KneeItf * ProVideoDevice::GetKneeItf() const
{
    return nullptr;
}

/******************************************************************************
 * ProVideoDevice::GetROIItf()
 *****************************************************************************/
ROIItf * ProVideoDevice::GetROIItf() const
{
    return nullptr;
}

/******************************************************************************
 * ProVideoDevice::GetKneeItf()
 *****************************************************************************/
DpccItf * ProVideoDevice::GetDpccItf() const
{
    return nullptr;
}

/******************************************************************************
 * ProVideoDevice::GetOsdItf()
 *****************************************************************************/
OsdItf * ProVideoDevice::GetOsdItf() const
{
    return nullptr;
}

/******************************************************************************
 * ProVideoDevice::CopyFlag()
 *****************************************************************************/
bool ProVideoDevice::GetCopyFlag() const
{
    return false;
}

/******************************************************************************
 * ProVideoDevice::setCopyFlag()
 *****************************************************************************/
void ProVideoDevice::SetCopyFlag( const bool flag )
{
    (void)flag;
}

/******************************************************************************
 * ProVideoDevice::getComChannel()
 *****************************************************************************/
ComChannel * ProVideoDevice::getComChannel() const
{
    return GetProVideoSystemItf()->GetComChannel();
}

/******************************************************************************
 * ProVideoDevice::setComChannel()
 *****************************************************************************/
void ProVideoDevice::setComChannel( ComChannel * c )
{
    GetProVideoSystemItf()->SetComChannel( c );
}

/******************************************************************************
 * ProVideoDevice::resync()
 *****************************************************************************/
void ProVideoDevice::resync()
{
    GetProVideoSystemItf()->resync();
}

/******************************************************************************
 * ProVideoDevice::resyncChainSpecific()
 *****************************************************************************/
void ProVideoDevice::resyncChainSpecific()
{
    // Nothing to be done here, has to be overwritten in device implementations
}

/******************************************************************************
 * ProVideoDevice::isConnected()
 *****************************************************************************/
bool ProVideoDevice::isConnected()
{
    return ( GetProVideoSystemItf()->isConnected() );
}

/******************************************************************************
 * ProVideoDevice::onSystemPlatformChange
 *****************************************************************************/
void ProVideoDevice::onSystemPlatformChange( QString name )
{
    d_data->m_systemPlatform = name;
}

/******************************************************************************
 * ProVideoDevice::getSystemPlatform
 * @brief This gets system platform name which is stored in the class variable
 *        of this object. It does not invoke a com command!
 * @returns The deivce name
 *****************************************************************************/
QString ProVideoDevice::getSystemPlatform()
{
    return d_data->m_systemPlatform;
}

/******************************************************************************
 * ProVideoDevice::onDeviceNameChange
 *****************************************************************************/
void ProVideoDevice::onDeviceNameChange( QString name )
{
    d_data->m_deviceName = name;
}

/******************************************************************************
 * ProVideoDevice::getDeviceName
 * @brief This gets the device name which is stored in the class variable
 *        of this object. It does not invoke a com command!
 * @returns The deivce name
 *****************************************************************************/
QString ProVideoDevice::getDeviceName()
{
    return d_data->m_deviceName;
}

/******************************************************************************
 * ProVideoDevice::onDeviceVersionChange
 *****************************************************************************/
void ProVideoDevice::onDeviceVersionChange( QString version )
{
    d_data->m_deviceVersion = version;
}

/******************************************************************************
 * ProVideoDevice::getDeviceVersion
 * @brief This gets the device version which is stored in the class variable
 *        of this object. It does not invoke a com command!
 * @returns The deivce version
 *****************************************************************************/
QString ProVideoDevice::getDeviceVersion()
{
    return d_data->m_deviceVersion;
}

/******************************************************************************
 * ProVideoDevice::onBroadcastAddressChange
 *****************************************************************************/
void ProVideoDevice::onBroadcastAddressChange( uint32_t broadcastAddress )
{
    d_data->m_broadcastAddress = broadcastAddress;
}

/******************************************************************************
 * ProVideoDevice::getBroadcastAddress
 * @brief This gets the devices broadcast address which is stored in the class
 *        variable of this object. It does not invoke a com command!
 * @returns The devices broadcast address
 *****************************************************************************/
unsigned int ProVideoDevice::getBroadcastAddress()
{
    return d_data->m_broadcastAddress;
}

/******************************************************************************
 * ProVideoDevice::onBroadcastMasterModeChange
 *****************************************************************************/
void ProVideoDevice::onBroadcastMasterModeChange( uint8_t isBroadcastMaster )
{
    d_data->m_isBroadcastMaster = static_cast<bool>(isBroadcastMaster);
}

/******************************************************************************
 * ProVideoDevice::getBroadcastMasterMode
 * @brief This gets the devices broadcast master mode which is stored in the
 *        class variable of this object. It does not invoke a com command!
 * @returns The devices broadcast address
 *****************************************************************************/
bool ProVideoDevice::getBroadcastMasterMode()
{
    return d_data->m_isBroadcastMaster;
}

/******************************************************************************
 * ProVideoDevice::onDeviceListChange
 *****************************************************************************/
void ProVideoDevice::onDeviceListChange( QList<rs485Device> deviceList )
{
    d_data->m_deviceList = deviceList;
}

/******************************************************************************
 * ProVideoDevice::getDeviceList
 * @brief This gets the list of connected devices which is stored in the
 *        class variable of this object. It does not invoke a com command!
 * @returns The list of connected devices
 *****************************************************************************/
QList<rs485Device> ProVideoDevice::getDeviceList()
{
    return d_data->m_deviceList;
}

