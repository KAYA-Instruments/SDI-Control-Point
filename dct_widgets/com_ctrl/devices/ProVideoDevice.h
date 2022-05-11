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
 * @file    ProVideoDevice.h
 *
 * @brief   ProVideo Device class
 *
 * @brief   Multiple Inheritance of an QObject is not allowed.
 *
 *****************************************************************************/
#ifndef _PROVIDEO_DEVICE_H_
#define _PROVIDEO_DEVICE_H_

#include <QObject>

#include "ComChannel.h"
#include "ComProtocol.h"

#include "ProVideoSystemItf.h"
#include "IspItf.h"
#include "CprocItf.h"
#include "AutoItf.h"
#include "CamItf.h"
#include "MccItf.h"
#include "LutItf.h"
#include "ChainItf.h"
#include "IrisItf.h"
#include "LensItf.h"
#include "KneeItf.h"
#include "ROIItf.h"
#include "DpccItf.h"
#include "OsdItf.h"

class ProVideoDevice : public QObject
{
    Q_OBJECT

public:
    struct features {
        bool hasCamItf;
        bool hasChainItf;
        bool hasChainRawMode;
        bool hasChainSdi2Mode;
        bool hasChainDownscale;
        bool hasChainGenLock;
        bool hasChainGenLockTermCrosslockOffset;
        bool hasChainTimeCode;
        bool hasChainTimeCodeHold;
        bool hasChainSdiSettings;
        bool hasChainHdmiSettings;
        bool hasChainSelection;
        bool hasChainFlipVertical;
        bool hasChainFlipHorizontal;
        bool hasChainAudio;
        bool hasAutoItf;
        bool hasIrisItf;
        bool hasLensItf;
        bool hasIspItf;
        bool hasIspLsc;
        bool hasIspSplitScreen;
        bool hasIspMasterBlackLevel;
        bool hasIspFlare;
        bool hasIspFilter;
        bool hasAntialiasingFilter;
        bool hasIspAutoWhiteBalance;
        bool hasIspGain;
        bool hasIspGreenGain;
        bool hasIspConversion;
        bool hasCprocItf;
        bool hasCprocItfHue;
        bool hasMccItf;
        bool hasKneeItf;
        bool hasLutItf;
        bool hasDpccItf;
        bool hasDpccFullFeautureSet;
        bool hasOsdItf;
        bool hasOsdTestPattern;
        bool hasSystemSaveLoad;
        bool hasSystemUpdate;
        bool hasSystemRuntime;
        bool hasSystemFan;
        bool hasSystemCopySettings;
        bool hasSystemBroadcast;
        bool hasRS232Interface;
        bool hasROIItf;

        unsigned int lutBitWidth;
        unsigned int numTempSensors;
    };

    explicit ProVideoDevice( ComChannel *, ComProtocol * );
    ~ProVideoDevice();

    // get communication channel
    ComChannel * getComChannel() const;

    // set communication channel
    virtual void setComChannel( ComChannel * );

    // get Provideo System Interface
    ProVideoSystemItf * GetProVideoSystemItf() const;

    // resync all settings
    virtual void resync();

    // resync only chain specific settings
    virtual void resyncChainSpecific();
    
    // check for connection
    bool isConnected();

    // Get the device information (this does not invoke a com command!)
    QString getSystemPlatform();
    QString getDeviceName();
    QString getDeviceVersion();
    unsigned int getBroadcastAddress();
    bool getBroadcastMasterMode();
    QList<rs485Device> getDeviceList();

    // Get the supported features of this device.
    virtual features getSupportedFeatures();

    // Virtual declarations of all get interface functions which will be
    // implemented in the derived classes.
    virtual IspItf *    GetIspItf() const;
    virtual CprocItf *  GetCprocItf() const;
    virtual AutoItf *   GetAutoItf() const;
    virtual CamItf *    GetCamItf() const;
    virtual MccItf *    GetMccItf() const;
    virtual LutItf *    GetLutItf() const;
    virtual ChainItf *  GetChainItf() const;
    virtual IrisItf *   GetIrisItf() const;
    virtual LensItf *   GetLensItf() const;
    virtual KneeItf *   GetKneeItf() const;
    virtual ROIItf *    GetROIItf() const;
    virtual DpccItf *   GetDpccItf() const;
    virtual OsdItf *    GetOsdItf() const;

    // Pure virtual implementations for set / get copy flag
    virtual bool GetCopyFlag() const;
    virtual void SetCopyFlag( const bool flag );

private slots:
    void onSystemPlatformChange( QString name );
    void onDeviceNameChange( QString name );
    void onDeviceVersionChange( QString name );
    void onBroadcastAddressChange( uint32_t broadcastAddress );
    void onBroadcastMasterModeChange( uint8_t isBroadcastMaster );
    void onDeviceListChange( QList<rs485Device> deviceList );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // _PROVIDEO_DEVICE_H_

