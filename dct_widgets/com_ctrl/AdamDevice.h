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
 * @file    AdamDevice.h
 *
 * @brief   
 *
 *****************************************************************************/
#ifndef _ADAM_DEVICE_H_
#define _ADAM_DEVICE_H_

#include <QObject>

#include "ProVideoDevice.h"

class AdamDevice : public ProVideoDevice
{
public:
    explicit AdamDevice( ComChannel * c, ComProtocol * p );
    ~AdamDevice();

    // set communication channel
    void setComChannel( ComChannel * ) override;

    // resync all settings
    void resync() override;

    IspItf *        GetIspItf() const override;
    CprocItf *      GetCprocItf() const override;
    AutoItf *       GetAutoItf() const override;
    CamItf *        GetCamItf() const override;
    LutItf *        GetLutItf() const override;
    ChainItf *      GetChainItf() const override;
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

#endif // _ADAM_DEVICE_H_

