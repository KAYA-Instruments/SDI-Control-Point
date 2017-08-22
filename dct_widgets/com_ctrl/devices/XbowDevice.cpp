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
 * @file    XbowDevice.cpp
 *
 * @brief   
 *
 *****************************************************************************/
#include <cerrno>

#include "common.h"
#include "XbowDevice.h"

#include <QtDebug>

/******************************************************************************
 * hardware mask 
 *****************************************************************************/
#define XBOW_FEATURE_CAMERA_LINK         ( 0x00000001u ) /**< camera link device */
#define XBOW_FEATURE_XPRESS              ( 0x00000002u ) /**< coax-press device */
#define XBOW_FEATURE_CHANNEL_0           ( 0x00000004u ) /**< video pipeline/chain 0 available */
#define XBOW_FEATURE_CHANNEL_1           ( 0x00000008u ) /**< video pipeline/chain 1 available */
#define XBOW_FEATURE_FPNC                ( 0x00000020u ) /**< fix pattern noise correction available */
#define XBOW_FEATURE_TFLT                ( 0x00000040u ) /**< temporal filter available */
#define XBOW_FEATURE_BFC                 ( 0x00000100u ) /**< black frame correction available */
#define XBOW_FEATURE_ZEBRA               ( 0x00000800u ) /**< OSD for zebra available */
#define XBOW_FEATURE_LOGO                ( 0x00001000u ) /**< OSD for logo available */
#define XBOW_FEATURE_TEST_PATTERN        ( 0x00001000u ) /**< OSD for test-pattern available */
#define XBOW_FEATURE_CENTER_MARKER       ( 0x00001000u ) /**< OSD for center marker available */
#define XBOW_FEATURE_MCC                 ( 0x00002000u ) /**< MCC is available */
#define XBOW_FEATURE_SEN_LUT             ( 0x00004000u ) /**< sensor LUT available */
#define XBOW_FEATURE_DPCC                ( 0x00008000u ) /**< DPCC available */
#define XBOW_FEATURE_WDR                 ( 0x00080000u ) /**< WDR/Knee available */

#define IS_XBOW_FEATURE_CAMERA_LINK(x)   ( x & XBOW_FEATURE_CAMERA_LINK )
#define IS_XBOW_FEATURE_XPRESS(x)        ( x & XBOW_FEATURE_XPRESS )
#define IS_XBOW_FEATURE_CHANNEL_0(x)     ( x & XBOW_FEATURE_CHANNEL_0 )
#define IS_XBOW_FEATURE_CHANNEL_1(x)     ( x & XBOW_FEATURE_CHANNEL_1 )
#define IS_XBOW_FEATURE_FPNC(x)          ( x & XBOW_FEATURE_FPNC )
#define IS_XBOW_FEATURE_TFLT(x)          ( x & XBOW_FEATURE_TFLT )
#define IS_XBOW_FEATURE_BFC(x)           ( x & XBOW_FEATURE_BFC )
#define IS_XBOW_FEATURE_ZEBRA(x)         ( x & XBOW_FEATURE_ZEBRA )
#define IS_XBOW_FEATURE_LOGO(x)          ( x & XBOW_FEATURE_LOGO )
#define IS_XBOW_FEATURE_TEST_PATTERN(x)  ( x & XBOW_FEATURE_TEST_PATTERN )
#define IS_XBOW_FEATURE_CENTER_MARKER(x) ( x & XBOW_FEATURE_CENTER_MARKER )
#define IS_XBOW_FEATURE_MCC(x)           ( x & XBOW_FEATURE_MCC )
#define IS_XBOW_FEATURE_SEN_LUT(x)       ( x & XBOW_FEATURE_SEN_LUT )
#define IS_XBOW_FEATURE_DPCC(x)          ( x & XBOW_FEATURE_DPCC )
#define IS_XBOW_FEATURE_WDR(x)           ( x & XBOW_FEATURE_WDR )

/******************************************************************************
 * hardware mask interpreter
 *****************************************************************************/
class XbowDevice::HwMaskInterpreter : public MaskInterpreter
{
public:
    QStringList interpret( const uint32_t mask ) override
    {
        QStringList features;

        // system overview
        
        features << "Hardware Features:";

        if ( IS_XBOW_FEATURE_CAMERA_LINK( mask ) )
        {
            features << "- Camera Link";
        }
 
        if ( IS_XBOW_FEATURE_XPRESS( mask ) )
        {
            features << "- Coaxpress";
        }

        if ( IS_XBOW_FEATURE_CHANNEL_0( mask ) )
        {
            features << "- Live channel available";
        }
        
        if ( IS_XBOW_FEATURE_CHANNEL_1( mask ) )
        {
            features << "- Playback channel available";
        }

        if ( IS_XBOW_FEATURE_ZEBRA( mask ) )
        {
            features << "- zebra function available";
        }

        if ( IS_XBOW_FEATURE_TEST_PATTERN( mask ) )
        {
            features << "- test pattern generator available";
        }

        if ( IS_XBOW_FEATURE_CENTER_MARKER( mask ) )
        {
            features << "- marker generator available";
        }

        // Correction Modules
        features << "Supported correction functions:";

        if ( IS_XBOW_FEATURE_SEN_LUT( mask ) )
        {
            features << "- Sensor linearization available";
        }

        if ( IS_XBOW_FEATURE_FPNC( mask ) )
        {
            features << "- Fix pattern noise correction available";
        }

        if ( IS_XBOW_FEATURE_BFC( mask ) )
        {
            features << "- Dark frame correction available";
        }

        if ( IS_XBOW_FEATURE_MCC( mask ) )
        {
            features << "- Multi color controller available";
        }

        if ( IS_XBOW_FEATURE_TFLT( mask ) )
        {
            features << "- Temporal filter available";
        }
 
        if ( IS_XBOW_FEATURE_DPCC( mask ) )
        {
            features << "- Defect/Hot pixel correction available";
        }
        
        if ( IS_XBOW_FEATURE_WDR( mask ) )
        {
            features << "- Highlight control/Knee function available";
        }

        return ( features );
    }
};

/******************************************************************************
 * software mask interpreter
 *****************************************************************************/
class XbowDevice::SwMaskInterpreter : public MaskInterpreter
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
 * XbowDevice::PrivateData
 *****************************************************************************/
class XbowDevice::PrivateData
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
        m_IrisItf       = new IrisItf( c, p->clone( ComProtocol::IRIS_INSTANCE, m_userCtx ) );
        m_KneeItf       = new KneeItf( c, p->clone( ComProtocol::KNEE_INSTANCE, m_userCtx ) );
        m_DpccItf       = new DpccItf( c, p->clone( ComProtocol::DPCC_INSTANCE, m_userCtx ) );

        // NotifyWhiteBalanceChanged
        QObject::connect( m_AutoItf , SIGNAL(NotifyWhiteBalanceChanged()),
                          m_IspItf  , SLOT(onNotifyWhiteBalanceUpdate()) );
        QObject::connect( m_AutoItf , SIGNAL(NotifyWhiteBalanceChanged()),
                          m_AutoItf , SLOT(onNotifyWhiteBalanceUpdate()) );

        // NotifyVideoModeChanged
        QObject::connect( m_ChainItf, SIGNAL(NotifyVideoModeChanged()),
                          m_CamItf  , SLOT(onNotifyVideoModeChange()) );
        
        // NotifyCameraGainChanged
        QObject::connect( m_CamItf  , SIGNAL(NotifyCameraGainChanged()),
                          m_IspItf  , SLOT(onNotifyCameraGainChange()) );
        
    };

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
        delete m_IrisItf->GetComProtocol();
        delete m_KneeItf->GetComProtocol();
        delete m_DpccItf->GetComProtocol();

        // clear interface classes
        delete m_IspItf;
        delete m_CprocItf;
        delete m_AutoItf;
        delete m_CamItf;
        delete m_MccItf;
        delete m_LutItf;
        delete m_ChainItf;
        delete m_IrisItf;
        delete m_KneeItf;
        delete m_DpccItf;

        // clear user context 
        m_protocol->deleteUserContext( m_userCtx );
        m_userCtx = NULL;
    };

    IspItf *        m_IspItf;
    CprocItf *      m_CprocItf;
    AutoItf *       m_AutoItf;
    CamItf *        m_CamItf;
    MccItf *        m_MccItf;
    LutItf *        m_LutItf;
    ChainItf *      m_ChainItf;
    IrisItf *       m_IrisItf;
    KneeItf *       m_KneeItf;
    DpccItf *       m_DpccItf;

    ComProtocol *   m_protocol;
    void *          m_userCtx;
};

/******************************************************************************
 * XbowDevice::XbowDevice()
 *****************************************************************************/
XbowDevice::XbowDevice( ComChannel * c, ComProtocol * p )
    : ProVideoDevice( c, p )
{
    d_data = new PrivateData( c, p );

    // set mask interpretation classes on interface class
    GetProVideoSystemItf()->SetMaskHwInterpreter( new HwMaskInterpreter() );
    GetProVideoSystemItf()->SetMaskSwInterpreter( new SwMaskInterpreter() );
}

/******************************************************************************
 * XbowDevice::~XbowDevice()
 *****************************************************************************/
XbowDevice::~XbowDevice()
{
    delete d_data;
}

/******************************************************************************
 * XbowDevice::getSupportedFeatures()
 *****************************************************************************/
XbowDevice::features XbowDevice::getSupportedFeatures()
{
    // Init all features with false
    features deviceFeatures;
    memset(&deviceFeatures, 0, sizeof (deviceFeatures));

    // Set all available features to true
    deviceFeatures.hasCamItf                = true;
    deviceFeatures.hasChainItf              = true;
    deviceFeatures.hasChainSdi2Mode         = true;
    deviceFeatures.hasChainGenLock          = true;
    deviceFeatures.hasChainTimeCode         = true;
    deviceFeatures.hasChainSdiSettings      = true;
    deviceFeatures.hasChainSelection        = true;
    deviceFeatures.hasChainFlip             = true;
    deviceFeatures.hasAutoItf               = true;
    deviceFeatures.hasIrisItf               = true;
    deviceFeatures.hasIspItf                = true;
    deviceFeatures.hasIspLsc                = true;
    deviceFeatures.hasIspMasterBlackLevel   = true;
    deviceFeatures.hasIspFlare              = true;
    deviceFeatures.hasIspFilter             = true;
    deviceFeatures.hasIspAutoWhiteBalance   = true;
    deviceFeatures.hasIspGain               = true;
    deviceFeatures.hasIspConversion         = true;
    deviceFeatures.hasCprocItf              = true;
    deviceFeatures.hasMccItf                = true;
    deviceFeatures.hasKneeItf               = true;
    deviceFeatures.hasLutItf                = true;
    deviceFeatures.hasDpccItf               = true;
    deviceFeatures.hasDpccFlash             = true;
    deviceFeatures.hasSystemSaveLoad        = true;
    deviceFeatures.hasSystemUpdate          = true;
    deviceFeatures.hasSystemRuntime         = true;
    deviceFeatures.hasSystemBroadcast       = true;

    return deviceFeatures;
}

/******************************************************************************
 * XbowDevice::GetIspItf()
 *****************************************************************************/
IspItf * XbowDevice::GetIspItf() const
{
    return ( d_data->m_IspItf );
}

/******************************************************************************
 * XbowDevice::GetCprocItf()
 *****************************************************************************/
CprocItf * XbowDevice::GetCprocItf() const
{
    return ( d_data->m_CprocItf );
}

/******************************************************************************
 * XbowDevice::GetAutoItf()
 *****************************************************************************/
AutoItf * XbowDevice::GetAutoItf() const
{
    return ( d_data->m_AutoItf );
}

/******************************************************************************
 * XbowDevice::GetCamItf()
 *****************************************************************************/
CamItf * XbowDevice::GetCamItf() const
{
    return ( d_data->m_CamItf );
}

/******************************************************************************
 * XbowDevice::GetMccItf()
 *****************************************************************************/
MccItf * XbowDevice::GetMccItf() const
{
    return ( d_data->m_MccItf );
}

/******************************************************************************
 * XbowDevice::GetLutItf()
 *****************************************************************************/
LutItf * XbowDevice::GetLutItf() const
{
    return ( d_data->m_LutItf );
}

/******************************************************************************
 * XbowDevice::GetChainItf()
 *****************************************************************************/
ChainItf * XbowDevice::GetChainItf() const
{
    return ( d_data->m_ChainItf );
}

/******************************************************************************
 * XbowDevice::GetIrisItf()
 *****************************************************************************/
IrisItf * XbowDevice::GetIrisItf() const
{
    return ( d_data->m_IrisItf );
}

/******************************************************************************
 * XbowDevice::GetKneeItf()
 *****************************************************************************/
KneeItf * XbowDevice::GetKneeItf() const
{
    return ( d_data->m_KneeItf );
}

/******************************************************************************
 * XbowDevice::GetDpccItf()
 *****************************************************************************/
DpccItf * XbowDevice::GetDpccItf() const
{
    return ( d_data->m_DpccItf );
}

/******************************************************************************
 * XbowDevice::CopyFlag()
 *****************************************************************************/
bool XbowDevice::GetCopyFlag() const
{
    return ( d_data->m_protocol->CopyFlag( d_data->m_userCtx ) );
}

/******************************************************************************
 * XbowDevice::setCopyFlag()
 *****************************************************************************/
void XbowDevice::SetCopyFlag( const bool flag )
{
    return ( d_data->m_protocol->setCopyFlag( d_data->m_userCtx, flag ) );
}

/******************************************************************************
 * XbowDevice::setComChannel()
 *****************************************************************************/
void XbowDevice::setComChannel( ComChannel * c )
{
    ProVideoDevice::setComChannel( c );

    GetIspItf()     ->SetComChannel( c );
    GetCprocItf()   ->SetComChannel( c );
    GetAutoItf()    ->SetComChannel( c );
    GetCamItf()     ->SetComChannel( c );
    GetMccItf()     ->SetComChannel( c );
    GetLutItf()     ->SetComChannel( c );
    GetChainItf()   ->SetComChannel( c );
    GetIrisItf()    ->SetComChannel( c );
    GetKneeItf()    ->SetComChannel( c );
    GetDpccItf()    ->SetComChannel( c );
 }

/******************************************************************************
 * XbowDevice::resync()
 *****************************************************************************/
void XbowDevice::resync()
{
    ProVideoDevice::resync();

    GetIspItf()     ->resync();
    GetCprocItf()   ->resync();
    GetAutoItf()    ->resync();
    GetCamItf()     ->resync();
    GetMccItf()     ->resync();
    GetLutItf()     ->resync();
    GetChainItf()   ->resync();
    GetIrisItf()    ->resync();
    GetKneeItf()    ->resync();
    GetDpccItf()    ->resync();
}

