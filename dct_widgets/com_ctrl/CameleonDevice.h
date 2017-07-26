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

