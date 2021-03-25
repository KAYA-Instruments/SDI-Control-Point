/**
 * @file    IronSDI_Device.h
 *
 * @brief
 *
 *****************************************************************************/
#ifndef IRONSDI_DEVICE_H
#define IRONSDI_DEVICE_H

#include <QObject>

#include "ProVideoDevice.h"

class IronSDI_Device : public ProVideoDevice
{
public:
    explicit IronSDI_Device( ComChannel * c, ComProtocol * p );
    IronSDI_Device( const ProVideoDevice &baseObj );
    ~IronSDI_Device() override;

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
    ROIItf *        GetROIItf() const override;
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






#endif // IRONSDI_DEVICE_H
