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
 * @file    CondorZeissDevice.h
 *
 * @brief   
 *
 *****************************************************************************/
#ifndef _CONDOR_ZEISS_DEVICE_H_
#define _CONDOR_ZEISS_DEVICE_H_

#include <QObject>

#include "ProVideoDevice.h"

class CondorZeissDevice : public ProVideoDevice
{
public:
    explicit CondorZeissDevice( ComChannel * c, ComProtocol * p );
    CondorZeissDevice( const ProVideoDevice &baseObj );
    ~CondorZeissDevice();

    // set communication channel
    void setComChannel( ComChannel * ) override;

    // resync all settings
    void resync() override;

    CamItf *        GetCamItf() const override;
    ChainItf *      GetChainItf() const override;

//    bool GetCopyFlag() const override;
//    void SetCopyFlag( const bool flag ) override;
    features getSupportedFeatures() override;

private:
    class HwMaskInterpreter;
    class SwMaskInterpreter;
    class PrivateData;
    PrivateData * d_data;
};

#endif // _CONDOR_ZEISS_DEVICE_H_
