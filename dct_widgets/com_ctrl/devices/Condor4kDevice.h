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
 * @file    Condor4kDevice.h
 *
 * @brief   
 *
 *****************************************************************************/
#ifndef _CONDOR4K_DEVICE_H_
#define _CONDOR4K_DEVICE_H_

#include <QObject>

#include "ProVideoDevice.h"

class Condor4kDevice : public ProVideoDevice
{
public:
    explicit Condor4kDevice( ComChannel * c, ComProtocol * p );
    Condor4kDevice( const ProVideoDevice &baseObj );
    ~Condor4kDevice() override;

    // set communication channel
    void setComChannel( ComChannel * ) override;

    // resync all settings
    void resync() override;

    // resync only chain specific settings
    void resyncChainSpecific() override;

    IspItf *        GetIspItf() const override;
    CprocItf *      GetCprocItf() const override;
    AutoItf *       GetAutoItf() const override;
    CamItf *        GetCamItf() const override;
    MccItf *        GetMccItf() const override;
    LutItf *        GetLutItf() const override;
    ChainItf *      GetChainItf() const override;
    LensItf *       GetLensItf() const override;
    KneeItf *       GetKneeItf() const override;
    DpccItf *       GetDpccItf() const override;

//    bool GetCopyFlag() const override;
//    void SetCopyFlag( const bool flag ) override;
    features getSupportedFeatures() override;

private:
    class HwMaskInterpreter;
    class SwMaskInterpreter;
    class PrivateData;
    PrivateData * d_data;
};

#endif // _CONDOR4K_DEVICE_H_

