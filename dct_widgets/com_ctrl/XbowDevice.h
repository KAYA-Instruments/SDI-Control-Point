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
 * @file    XbowDevice.h
 *
 * @brief   
 *
 *****************************************************************************/
#ifndef _XBOW_DEVICE_H_
#define _XBOW_DEVICE_H_

#include <QObject>

#include "ProVideoDevice.h"

class XbowDevice : public ProVideoDevice
{
public:
    explicit XbowDevice( ComChannel * c, ComProtocol * p );
    XbowDevice( const ProVideoDevice &baseObj );
    ~XbowDevice();

    // set communication channel
    void setComChannel( ComChannel * ) override;

    // resync all settings
    void resync() override;

    IspItf *        GetIspItf() const override;
    CprocItf *      GetCprocItf() const override;
    AutoItf *       GetAutoItf() const override;
    CamItf *        GetCamItf() const override;
    MccItf *        GetMccItf() const override;
    LutItf *        GetLutItf() const override;
    ChainItf *      GetChainItf() const override;
    IrisItf *       GetIrisItf() const override;
    KneeItf *       GetKneeItf() const override;
    DpccItf *       GetDpccItf() const override;

    bool GetCopyFlag() const override;
    void SetCopyFlag( const bool flag ) override;
    features getSupportedFeatures() override;

private:
    class HwMaskInterpreter;
    class SwMaskInterpreter;
    class PrivateData;
    PrivateData * d_data;
};

#endif // _XBOW_DEVICE_H_

