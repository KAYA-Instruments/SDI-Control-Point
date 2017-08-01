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
 * @file    CameleonDevice.h
 *
 * @brief   
 *
 *****************************************************************************/
#ifndef _CAMELEON_DEVICE_H_
#define _CAMELEON_DEVICE_H_

#include <QObject>

#include "ProVideoDevice.h"

class CameleonDevice : public ProVideoDevice
{
public:
    explicit CameleonDevice( ComChannel * c, ComProtocol * p );
    CameleonDevice( const ProVideoDevice &baseObj );
    ~CameleonDevice();

    // set communication channel
    void setComChannel( ComChannel * ) override;

    // resync all settings
    void resync() override;

    IspItf *        GetIspItf() const override;
    CprocItf *      GetCprocItf() const override;
    MccItf *        GetMccItf() const override;
    ChainItf *      GetChainItf() const override;
    OsdItf *        GetOsdItf() const override;

    bool GetCopyFlag() const override;
    void SetCopyFlag( const bool flag ) override;
    features getSupportedFeatures() override;

private:
    class HwMaskInterpreter;
    class SwMaskInterpreter;
    class PrivateData;
    PrivateData * d_data;
};

#endif // _CAMELEON_DEVICE_H_

