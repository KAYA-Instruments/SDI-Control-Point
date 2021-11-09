/**
 * @file    IronSDI_Device.cpp
 *
 * @brief
 *
 *****************************************************************************/
#include <cerrno>

#include "common.h"
#include "IronSDI_Device.h"

#include <QtDebug>

/******************************************************************************
 * hardware mask
 *****************************************************************************/
#define IRON_SDI_FEATURE_HW_GENLOCK_MASK              ( 0x00000001u ) /**< GENLOCK available */
#define IRON_SDI_FEATURE_MULTI_USERSET_MASK           ( 0x00000002u ) /**< MULTI USERSET available */
#define IRON_SDI_FEATURE_HW_12BIT                     ( 0x00000004u ) /**< 12BIT available */
#define IRON_SDI_FEATURE_AWB_AEC_AEG_EX_MASK          ( 0x00000008u ) /**< AWB AEC AEG EX MASK available */
#define IRON_SDI_FEATURE_RS485_MASK                   ( 0x00000010u ) /**< RS485 available */
#define IRON_SDI_FEATURE_TIMECODE_MASK                ( 0x00000020u ) /**< TIMECODE available */
#define IRON_SDI_FEATURE_GENLOCK_TERM_CROSSLOCK_MASK  ( 0x00000040u ) /**< GENLOCK CROSSLOCK available */

#define IS_IRON_SDI_FEATURE_HW_GENLOCK_MASK(x)              ( x & IRON_SDI_FEATURE_HW_GENLOCK_MASK  )
#define IS_IRON_SDI_FEATURE_MULTI_USERSET_MASK(x)           ( x & IRON_SDI_FEATURE_MULTI_USERSET_MASK )
#define IS_IRON_SDI_FEATURE_HW_12BIT(x)                     ( x & IRON_SDI_FEATURE_HW_12BIT )
#define IS_IRON_SDI_FEATURE_AWB_AEC_AEG_EX_MASK(x)          ( x & IRON_SDI_FEATURE_AWB_AEC_AEG_EX_MASK )
#define IS_IRON_SDI_FEATURE_RS485_MASK(x)                   ( x & IRON_SDI_FEATURE_RS485_MASK )
#define IS_IRON_SDI_FEATURE_TIMECODE_MASK(x)                ( x & IRON_SDI_FEATURE_TIMECODE_MASK )
#define IS_IRON_SDI_FEATURE_GENLOCK_TERM_CROSSLOCK_OFFSET_MASK(x)  ( x & IRON_SDI_FEATURE_GENLOCK_TERM_CROSSLOCK_MASK )

/******************************************************************************
 * hardware mask interpreter
 *****************************************************************************/
class IronSDI_Device::HwMaskInterpreter : public MaskInterpreter
{
public:
    QStringList interpret( const uint32_t mask ) override
    {
        QStringList features;

        // system overview

        features << "System: IronSDI";

        features << "Supported modules:";

        if ( IS_IRON_SDI_FEATURE_HW_GENLOCK_MASK( mask ) )
        {
            features << "- Genlock";
        }

        if ( IS_IRON_SDI_FEATURE_GENLOCK_TERM_CROSSLOCK_OFFSET_MASK( mask ) )
        {
            features << "- Genlock Termination and Crosslock";
        }

        if ( IS_IRON_SDI_FEATURE_MULTI_USERSET_MASK( mask ) )
        {
            features << "- Multi Userset";
        }

        if ( IS_IRON_SDI_FEATURE_HW_12BIT( mask ) )
        {
            features << "- 12 bit";
        }

        if ( IS_IRON_SDI_FEATURE_AWB_AEC_AEG_EX_MASK( mask ) )
        {
            features << "- Auto white balance";
            features << "- Auto Exposure";
            features << "- Auto Gain";
            features << "- ROI offset";
        }

        if ( IS_IRON_SDI_FEATURE_RS485_MASK( mask ) )
        {
            features << "- RS485 interface";
        }

        if ( IS_IRON_SDI_FEATURE_TIMECODE_MASK( mask ) )
        {
            features << "- Timecode";
        }

        return ( features );
    }
};

/******************************************************************************
 * software mask interpreter
 *****************************************************************************/
class IronSDI_Device::SwMaskInterpreter : public MaskInterpreter
{
public:
    QStringList interpret( const uint32_t mask ) override
    {
        (void) mask;
        QStringList features;
        return ( features );
    }
};

/******************************************************************************
 * IronSDI_Device::PrivateData
 *****************************************************************************/
class IronSDI_Device::PrivateData
{
public:
    PrivateData( ComChannel * c, ComProtocol * p )
    {
        m_protocol      = p;
        m_userCtx       = p->createUserContext();

        m_IspItf        = new IspItf( c, p->clone( ComProtocol::ISP_INSTANCE, m_userCtx ) );
        m_CprocItf      = new CprocItf( c, p->clone( ComProtocol::CPROC_INSTANCE, m_userCtx ) );
        m_AutoItf       = new AutoItf( c, p->clone( ComProtocol::AUTO_INSTANCE, m_userCtx ) );
        m_CamItf        = new CamItf( c, p->clone( ComProtocol::CAM_INSTANCE, m_userCtx ) );
        m_MccItf        = new MccItf( c, p->clone( ComProtocol::MCC_INSTANCE, m_userCtx ) );
        m_LutItf        = new LutItf( c, p->clone( ComProtocol::LUT_INSTANCE, m_userCtx ) );
        m_ChainItf      = new ChainItf( c, p->clone( ComProtocol::CHAIN_INSTANCE, m_userCtx ) );
        m_LensItf       = new LensItf( c, p->clone( ComProtocol::LENS_INSTANCE, m_userCtx ) );
        m_KneeItf       = new KneeItf( c, p->clone( ComProtocol::KNEE_INSTANCE, m_userCtx ) );
        m_ROIItf        = new ROIItf( c, p->clone( ComProtocol::ROI_INSTANCE, m_userCtx ) );
        m_DpccItf       = new DpccItf( c, p->clone( ComProtocol::DPCC_INSTANCE, m_userCtx ) );

        // NotifyWhiteBalanceChanged
        QObject::connect( m_AutoItf , SIGNAL(NotifyWhiteBalanceChanged()),
                          m_IspItf  , SLOT(onNotifyWhiteBalanceUpdate()) );
        QObject::connect( m_AutoItf , SIGNAL(NotifyWhiteBalanceChanged()),
                          m_AutoItf , SLOT(onNotifyWhiteBalanceUpdate()) );

        // NotifyVideoModeChanged
        QObject::connect( m_ChainItf, SIGNAL(NotifyVideoModeChanged()),
                          m_CamItf  , SLOT(onNotifyVideoModeChange()) );

        // NotifyLogModeChanged
        QObject::connect( m_LutItf, SIGNAL(NotifyLogModeChanged()),
                          m_CamItf  , SLOT(onNotifyLogModeChange()) );

        // NotifyCameraGainChanged
        QObject::connect( m_CamItf  , SIGNAL(NotifyCameraGainChanged()),
                          m_IspItf  , SLOT(onNotifyCameraGainChange()) );

    }

    ~PrivateData()
    {
        // clear protocol clones
        delete m_IspItf->GetComProtocol();
        delete m_CprocItf->GetComProtocol();
        delete m_AutoItf->GetComProtocol();
        delete m_CamItf->GetComProtocol();
        delete m_MccItf->GetComProtocol();
        delete m_LutItf->GetComProtocol();
        delete m_ChainItf->GetComProtocol();
        delete m_LensItf->GetComProtocol();
        delete m_KneeItf->GetComProtocol();
        delete m_ROIItf->GetComProtocol();
        delete m_DpccItf->GetComProtocol();

        // clear interface classes
        delete m_IspItf;
        delete m_CprocItf;
        delete m_AutoItf;
        delete m_CamItf;
        delete m_MccItf;
        delete m_LutItf;
        delete m_ChainItf;
        delete m_LensItf;
        delete m_KneeItf;
        delete m_ROIItf;
        delete m_DpccItf;

        // clear user context
        m_protocol->deleteUserContext( m_userCtx );
        m_userCtx = nullptr;
    }

    IspItf *        m_IspItf;
    CprocItf *      m_CprocItf;
    AutoItf *       m_AutoItf;
    CamItf *        m_CamItf;
    MccItf *        m_MccItf;
    LutItf *        m_LutItf;
    ChainItf *      m_ChainItf;
    LensItf *       m_LensItf;
    KneeItf *       m_KneeItf;
    ROIItf *        m_ROIItf;
    DpccItf *       m_DpccItf;

    ComProtocol *   m_protocol;
    void *          m_userCtx;

    features m_deviceFeatures;
};

/******************************************************************************
 * IronSDI_Device::IronSDI_Device()
 *****************************************************************************/
IronSDI_Device::IronSDI_Device( ComChannel * c, ComProtocol * p , uint32_t HwMask, uint32_t SwMask)
    : ProVideoDevice( c, p )
{
    d_data = new PrivateData( c, p );

    // set mask interpretation classes on interface class
    GetProVideoSystemItf()->SetMaskHwInterpreter( new HwMaskInterpreter() );
    GetProVideoSystemItf()->SetMaskSwInterpreter( new SwMaskInterpreter() );

    setSupportedFeatures(HwMask, SwMask);
}

/******************************************************************************
 * IronSDI_Device::~IronSDI_Device()
 *****************************************************************************/
IronSDI_Device::~IronSDI_Device()
{
    delete d_data;
}

/******************************************************************************
 * IronSDI_Device::()
 *****************************************************************************/
void IronSDI_Device::setSupportedFeatures(const uint32_t HwMask, const uint32_t SwMask)
{
    memset(&d_data->m_deviceFeatures, 0, sizeof (d_data->m_deviceFeatures));

    features * deviceFeatures = &d_data->m_deviceFeatures;

    // Set all available features
    deviceFeatures->hasCamItf                           = true;
    deviceFeatures->hasChainItf                         = true;
    deviceFeatures->hasChainSdi2Mode                    = false;
    deviceFeatures->hasChainAudio                       = false;
    deviceFeatures->hasChainDownscale                   = true;
    deviceFeatures->hasChainGenLock                     = false;
    deviceFeatures->hasChainGenLockTermCrosslockOffset  = false;
    deviceFeatures->hasChainTimeCode                    = false;
    deviceFeatures->hasChainTimeCodeHold                = false;
    deviceFeatures->hasChainSdiSettings                 = true;
    deviceFeatures->hasChainSelection                   = false;
    deviceFeatures->hasChainFlipVertical                = true;
    deviceFeatures->hasChainFlipHorizontal              = true;
    deviceFeatures->hasAutoItf                          = true;
    deviceFeatures->hasLensItf                          = false;
    deviceFeatures->hasIspItf                           = true;
    deviceFeatures->hasIspLsc                           = false;
    deviceFeatures->hasIspMasterBlackLevel              = true;
    deviceFeatures->hasIspFlare                         = true;
    deviceFeatures->hasIspFilter                        = false;
    deviceFeatures->hasIspAutoWhiteBalance              = true;
    deviceFeatures->hasIspGain                          = true;
    deviceFeatures->hasIspGreenGain                     = true;
    deviceFeatures->hasIspConversion                    = true;
    deviceFeatures->hasCprocItf                         = true;
    deviceFeatures->hasCprocItfHue                      = true;
    deviceFeatures->hasMccItf                           = false;
    deviceFeatures->hasKneeItf                          = true;
    deviceFeatures->hasLutItf                           = true;
    deviceFeatures->hasDpccItf                          = true;
    deviceFeatures->hasDpccFullFeautureSet              = true;
    deviceFeatures->hasSystemSaveLoad                   = true;
    deviceFeatures->hasSystemUpdate                     = true;
    deviceFeatures->hasSystemRuntime                    = false;
    deviceFeatures->hasSystemFan                        = false;
    deviceFeatures->hasSystemBroadcast                  = false;
    deviceFeatures->hasRS232Interface                   = true;
    deviceFeatures->hasROIItf                           = false;
    deviceFeatures->lutBitWidth                         = 10;
    deviceFeatures->numTempSensors                      = 2;

    if((0xFFFFFFFF != SwMask) && HwMask)
    {
        deviceFeatures->hasChainGenLock                    = IS_IRON_SDI_FEATURE_HW_GENLOCK_MASK( HwMask );
        deviceFeatures->hasChainTimeCode                   = IS_IRON_SDI_FEATURE_TIMECODE_MASK( HwMask );
        deviceFeatures->hasChainTimeCodeHold               = IS_IRON_SDI_FEATURE_TIMECODE_MASK( HwMask );
        deviceFeatures->hasROIItf                          = IS_IRON_SDI_FEATURE_AWB_AEC_AEG_EX_MASK( HwMask );
        deviceFeatures->hasSystemBroadcast                 = IS_IRON_SDI_FEATURE_RS485_MASK(HwMask); // TODO: only if RS485 is enabled
        deviceFeatures->hasSystemSaveLoad                  = IS_IRON_SDI_FEATURE_MULTI_USERSET_MASK( HwMask );
        deviceFeatures->lutBitWidth                        = IS_IRON_SDI_FEATURE_HW_12BIT( HwMask ) ? 12 : 10;
        deviceFeatures->hasChainGenLockTermCrosslockOffset = IS_IRON_SDI_FEATURE_GENLOCK_TERM_CROSSLOCK_OFFSET_MASK( HwMask );
    }

}

/******************************************************************************
 * IronSDI_Device::getSupportedFeatures()
 *****************************************************************************/
IronSDI_Device::features IronSDI_Device::getSupportedFeatures()
{
    return d_data->m_deviceFeatures;
}

/******************************************************************************
 * IronSDI_Device::GetIspItf()
 *****************************************************************************/
IspItf * IronSDI_Device::GetIspItf() const
{
    return ( d_data->m_IspItf );
}

/******************************************************************************
 * IronSDI_Device::GetCprocItf()
 *****************************************************************************/
CprocItf * IronSDI_Device::GetCprocItf() const
{
    return ( d_data->m_CprocItf );
}

/******************************************************************************
 * IronSDI_Device::GetAutoItf()
 *****************************************************************************/
AutoItf * IronSDI_Device::GetAutoItf() const
{
    return ( d_data->m_AutoItf );
}

/******************************************************************************
 * IronSDI_Device::GetCamItf()
 *****************************************************************************/
CamItf * IronSDI_Device::GetCamItf() const
{
    return ( d_data->m_CamItf );
}

/******************************************************************************
 * IronSDI_Device::GetMccItf()
 *****************************************************************************/
MccItf * IronSDI_Device::GetMccItf() const
{
    return ( d_data->m_MccItf );
}

/******************************************************************************
 * IronSDI_Device::GetLutItf()
 *****************************************************************************/
LutItf * IronSDI_Device::GetLutItf() const
{
    return ( d_data->m_LutItf );
}

/******************************************************************************
 * IronSDI_Device::GetChainItf()
 *****************************************************************************/
ChainItf * IronSDI_Device::GetChainItf() const
{
    return ( d_data->m_ChainItf );
}

/******************************************************************************
 * IronSDI_Device::GetLensItf()
 *****************************************************************************/
LensItf * IronSDI_Device::GetLensItf() const
{
    return ( d_data->m_LensItf );
}

/******************************************************************************
 * IronSDI_Device::GetKneeItf()
 *****************************************************************************/
KneeItf * IronSDI_Device::GetKneeItf() const
{
    return ( d_data->m_KneeItf );
}

/******************************************************************************
 * IronSDI_Device::GetROIItf()
 *****************************************************************************/
ROIItf * IronSDI_Device::GetROIItf() const
{
    return ( d_data->m_ROIItf );
}

/******************************************************************************
 * IronSDI_Device::GetDpccItf()
 *****************************************************************************/
DpccItf * IronSDI_Device::GetDpccItf() const
{
    return ( d_data->m_DpccItf );
}

///******************************************************************************
// * IronSDI_Device::CopyFlag()
// *****************************************************************************/
//bool IronSDI_Device::GetCopyFlag() const
//{
//    return ( d_data->m_protocol->CopyFlag( d_data->m_userCtx ) );
//}

///******************************************************************************
// * IronSDI_Device::setCopyFlag()
// *****************************************************************************/
//void IronSDI_Device::SetCopyFlag( const bool flag )
//{
//    return ( d_data->m_protocol->setCopyFlag( d_data->m_userCtx, flag ) );
//}

/******************************************************************************
 * IronSDI_Device::setComChannel()
 *****************************************************************************/
void IronSDI_Device::setComChannel( ComChannel * c )
{
    ProVideoDevice::setComChannel( c );

    GetIspItf()     ->SetComChannel( c );
    GetCprocItf()   ->SetComChannel( c );
    GetCamItf()     ->SetComChannel( c );
    GetAutoItf()    ->SetComChannel( c );
    GetMccItf()     ->SetComChannel( c );
    GetLutItf()     ->SetComChannel( c );
    GetChainItf()   ->SetComChannel( c );
    GetLensItf()    ->SetComChannel( c );
    GetKneeItf()    ->SetComChannel( c );
    GetROIItf()     ->SetComChannel( c );
    GetDpccItf()    ->SetComChannel( c );
}

/******************************************************************************
 * IronSDI_Device::resync()
 *****************************************************************************/
void IronSDI_Device::resync()
{
    ProVideoDevice::resync();

    GetIspItf()     ->resync();
    GetCprocItf()   ->resync();
    GetCamItf()     ->resync();
    GetAutoItf()    ->resync();
    GetMccItf()     ->resync();
    GetLutItf()     ->resync();
    GetChainItf()   ->resync();
    GetLensItf()    ->resync();
    GetKneeItf()    ->resync();
    GetROIItf()     ->resync();
    GetDpccItf()    ->resync();
}

/******************************************************************************
 * IronSDI_Device::resyncChainSpecific()
 *****************************************************************************/
void IronSDI_Device::resyncChainSpecific()
{
    GetIspItf()     ->resync();
    GetLutItf()     ->resync();
}
