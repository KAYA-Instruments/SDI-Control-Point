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
    ~Condor4kDevice();

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

