/******************************************************************************
 *
 * Copyright 2016, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    CondorZeissDevice.cpp
 *
 * @brief
 *
 *****************************************************************************/
#include <cerrno>

#include "common.h"
#include "CondorZeissDevice.h"

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
class CondorZeissDevice::HwMaskInterpreter : public MaskInterpreter
{
public:
    QStringList interpret( const uint32_t mask ) override
    {
        QStringList features;

        // system overview

        features << "System: Condor Zeiss";
        features << "- Gain and Exposure Settings";
        features << "- Video Mode";

        // @TODO implement correct feature mask
        (void)mask;
//        if ( IS_XBOW_FEATURE_CAMERA_LINK( mask ) )
//        {
//            features << "- Camera Link";
//        }

//        if ( IS_XBOW_FEATURE_XPRESS( mask ) )
//        {
//            features << "- Coaxpress";
//        }

//        if ( IS_XBOW_FEATURE_CHANNEL_0( mask ) )
//        {
//            features << "- Live channel available";
//        }

//        if ( IS_XBOW_FEATURE_CHANNEL_1( mask ) )
//        {
//            features << "- Playback channel available";
//        }

//        if ( IS_XBOW_FEATURE_ZEBRA( mask ) )
//        {
//            features << "- zebra function available";
//        }

//        if ( IS_XBOW_FEATURE_TEST_PATTERN( mask ) )
//        {
//            features << "- test pattern generator available";
//        }

//        if ( IS_XBOW_FEATURE_CENTER_MARKER( mask ) )
//        {
//            features << "- marker generator available";
//        }

//        // Correction Modules
//        features << "Supported correction functions:";

//        if ( IS_XBOW_FEATURE_SEN_LUT( mask ) )
//        {
//            features << "- Sensor linearization available";
//        }

//        if ( IS_XBOW_FEATURE_FPNC( mask ) )
//        {
//            features << "- Fix pattern noise correction available";
//        }

//        if ( IS_XBOW_FEATURE_BFC( mask ) )
//        {
//            features << "- Dark frame correction available";
//        }

//        if ( IS_XBOW_FEATURE_MCC( mask ) )
//        {
//            features << "- Multi color controller available";
//        }

//        if ( IS_XBOW_FEATURE_TFLT( mask ) )
//        {
//            features << "- Temporal filter available";
//        }

//        if ( IS_XBOW_FEATURE_DPCC( mask ) )
//        {
//            features << "- Defect/Hot pixel correction available";
//        }

//        if ( IS_XBOW_FEATURE_WDR( mask ) )
//        {
//            features << "- Highlight control/Knee function available";
//        }

        return ( features );
    }
};

/******************************************************************************
 * software mask interpreter
 *****************************************************************************/
class CondorZeissDevice::SwMaskInterpreter : public MaskInterpreter
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
 * CondorZeissDevice::PrivateData
 *****************************************************************************/
class CondorZeissDevice::PrivateData
{
public:
    PrivateData( ComChannel * c, ComProtocol * p )
    {
        m_protocol      = p;
        m_userCtx       = p->createUserContext();

        m_CamItf        = new CamItf( c, p->clone( ComProtocol::CAM_INSTANCE, m_userCtx ) );
        m_ChainItf      = new ChainItf( c, p->clone( ComProtocol::CHAIN_INSTANCE, m_userCtx ) );

        // NotifyVideoModeChanged
        QObject::connect( m_ChainItf, SIGNAL(NotifyVideoModeChanged()),
                          m_CamItf  , SLOT(onNotifyVideoModeChange()) );
    };

    ~PrivateData()
    {
        // clear protocol clones
        delete m_CamItf->GetComProtocol();
        delete m_ChainItf->GetComProtocol();

        // clear interface classes
        delete m_CamItf;
        delete m_ChainItf;

        // clear user context
        m_protocol->deleteUserContext( m_userCtx );
        m_userCtx = NULL;
    };

    CamItf *        m_CamItf;
    ChainItf *      m_ChainItf;

    ComProtocol *   m_protocol;
    void *          m_userCtx;
};

/******************************************************************************
 * CondorZeissDevice::CondorZeissDevice()
 *****************************************************************************/
CondorZeissDevice::CondorZeissDevice( ComChannel * c, ComProtocol * p )
    : ProVideoDevice( c, p )
{
    d_data = new PrivateData( c, p );

    // set mask interpretation classes on interface class
    GetProVideoSystemItf()->SetMaskHwInterpreter( new HwMaskInterpreter() );
    GetProVideoSystemItf()->SetMaskSwInterpreter( new SwMaskInterpreter() );
}

/******************************************************************************
 * CondorZeissDevice::~CondorZeissDevice()
 *****************************************************************************/
CondorZeissDevice::~CondorZeissDevice()
{
    delete d_data;
}

/******************************************************************************
 * CondorZeissDevice::getSupportedFeatures()
 *****************************************************************************/
CondorZeissDevice::features CondorZeissDevice::getSupportedFeatures()
{
    // Init all features with false
    features deviceFeatures;
    memset(&deviceFeatures, 0, sizeof (deviceFeatures));

    // Set all available features to true
    deviceFeatures.hasCamItf                = true;
    deviceFeatures.hasChainItf              = true;
    deviceFeatures.hasSystemSaveLoad        = true;
    deviceFeatures.hasSystemUpdate          = true;
    deviceFeatures.hasSystemRuntime         = true;

    return deviceFeatures;
}

/******************************************************************************
 * CondorZeissDevice::GetCamItf()
 *****************************************************************************/
CamItf * CondorZeissDevice::GetCamItf() const
{
    return ( d_data->m_CamItf );
}

/******************************************************************************
 * CondorZeissDevice::GetChainItf()
 *****************************************************************************/
ChainItf * CondorZeissDevice::GetChainItf() const
{
    return ( d_data->m_ChainItf );
}

///******************************************************************************
// * CondorZeissDevice::CopyFlag()
// *****************************************************************************/
//bool CondorZeissDevice::GetCopyFlag() const
//{
//    return ( d_data->m_protocol->CopyFlag( d_data->m_userCtx ) );
//}

///******************************************************************************
// * CondorZeissDevice::setCopyFlag()
// *****************************************************************************/
//void CondorZeissDevice::SetCopyFlag( const bool flag )
//{
//    return ( d_data->m_protocol->setCopyFlag( d_data->m_userCtx, flag ) );
//}

/******************************************************************************
 * CondorZeissDevice::setComChannel()
 *****************************************************************************/
void CondorZeissDevice::setComChannel( ComChannel * c )
{
    ProVideoDevice::setComChannel( c );

    GetCamItf()     ->SetComChannel( c );
    GetChainItf()   ->SetComChannel( c );
 }

/******************************************************************************
 * CondorZeissDevice::resync()
 *****************************************************************************/
void CondorZeissDevice::resync()
{
    ProVideoDevice::resync();

    GetCamItf()     ->resync();
    GetChainItf()   ->resync();
}
