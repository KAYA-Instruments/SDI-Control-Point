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
 * @file    CameleonDevice.cpp
 *
 * @brief   
 *
 *****************************************************************************/
#include <cerrno>

#include "common.h"
#include "CameleonDevice.h"

#include <QtDebug>

/******************************************************************************
 * hardware mask 
 *****************************************************************************/

/******************************************************************************
 * hardware mask interpreter
 *****************************************************************************/
class CameleonDevice::HwMaskInterpreter : public MaskInterpreter
{
public:
    QStringList interpret( const uint32_t mask ) override
    {
        QStringList features;

        // system overview
        
        features << "System: Cameleon";

        // TODO: implement HW mask!
        (void)mask;

        return ( features );
    }
};

/******************************************************************************
 * software mask interpreter
 *****************************************************************************/
class CameleonDevice::SwMaskInterpreter : public MaskInterpreter
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
 * CameleonDevice::PrivateData
 *****************************************************************************/
class CameleonDevice::PrivateData
{
public:
    PrivateData( ComChannel * c, ComProtocol * p )
    {
        m_protocol      = p;
        m_userCtx       = p->createUserContext();

        m_IspItf        = new IspItf( c, p->clone( ComProtocol::ISP_INSTANCE, m_userCtx ) ); 
        m_CprocItf      = new CprocItf( c, p->clone( ComProtocol::CPROC_INSTANCE, m_userCtx ) ); 
        m_MccItf        = new MccItf( c, p->clone( ComProtocol::MCC_INSTANCE, m_userCtx ) );
        m_ChainItf      = new ChainItf( c, p->clone( ComProtocol::CHAIN_INSTANCE, m_userCtx ) );
        m_OsdItf        = new OsdItf( c, p->clone( ComProtocol::OSD_INSTANCE, m_userCtx ) );
    };

    ~PrivateData()
    {
        // clear protocol clones
        delete m_IspItf->GetComProtocol();
        delete m_CprocItf->GetComProtocol();
        delete m_MccItf->GetComProtocol();
        delete m_ChainItf->GetComProtocol();
        delete m_OsdItf->GetComProtocol();

        // clear interface classes
        delete m_IspItf;
        delete m_CprocItf;
        delete m_MccItf;
        delete m_ChainItf;
        delete m_OsdItf;

        // clear user context 
        m_protocol->deleteUserContext( m_userCtx );
        m_userCtx = NULL;
    };

    IspItf *        m_IspItf;
    CprocItf *      m_CprocItf;
    MccItf *        m_MccItf;
    ChainItf *      m_ChainItf;
    OsdItf *        m_OsdItf;

    ComProtocol *   m_protocol;
    void *          m_userCtx;
};

/******************************************************************************
 * CameleonDevice::CameleonDevice()
 *****************************************************************************/
CameleonDevice::CameleonDevice( ComChannel * c, ComProtocol * p )
    : ProVideoDevice( c, p )
{
    d_data = new PrivateData( c, p );

    // set mask interpretation classes on interface class
    GetProVideoSystemItf()->SetMaskHwInterpreter( new HwMaskInterpreter() );
    GetProVideoSystemItf()->SetMaskSwInterpreter( new SwMaskInterpreter() );
}

/******************************************************************************
 * CameleonDevice::~CameleonDevice()
 *****************************************************************************/
CameleonDevice::~CameleonDevice()
{
    delete d_data;
}

/******************************************************************************
 * CameleonDevice::getSupportedFeatures()
 *****************************************************************************/
CameleonDevice::features CameleonDevice::getSupportedFeatures()
{
    // Init all features with false
    features deviceFeatures;
    memset(&deviceFeatures, 0, sizeof (deviceFeatures));

    // Set all available features to true
    deviceFeatures.hasChainItf              = true;
    deviceFeatures.hasChainSelection        = true;
    deviceFeatures.hasIspItf                = true;
    deviceFeatures.hasIspSplitScreen        = true;
    deviceFeatures.hasIspMasterBlackLevel   = true;
    deviceFeatures.hasIspGain               = true;
    deviceFeatures.hasIspGreenGain          = true;
    deviceFeatures.hasCprocItf              = true;
    deviceFeatures.hasMccItf                = true;
    deviceFeatures.hasSystemSaveLoad        = true;
    deviceFeatures.hasSystemCopySettings    = true;
    deviceFeatures.hasOsdItf                = true;
    deviceFeatures.hasOsdTestPattern        = true;

    return deviceFeatures;
}

/******************************************************************************
 * CameleonDevice::GetIspItf()
 *****************************************************************************/
IspItf * CameleonDevice::GetIspItf() const
{
    return ( d_data->m_IspItf );
}

/******************************************************************************
 * CameleonDevice::GetCprocItf()
 *****************************************************************************/
CprocItf * CameleonDevice::GetCprocItf() const
{
    return ( d_data->m_CprocItf );
}

/******************************************************************************
 * CameleonDevice::GetMccItf()
 *****************************************************************************/
MccItf * CameleonDevice::GetMccItf() const
{
    return ( d_data->m_MccItf );
}

/******************************************************************************
 * CameleonDevice::GetChainItf()
 *****************************************************************************/
ChainItf * CameleonDevice::GetChainItf() const
{
    return ( d_data->m_ChainItf );
}

/******************************************************************************
 * CameleonDevice::GetOsdItf()
 *****************************************************************************/
OsdItf * CameleonDevice::GetOsdItf() const
{
    return ( d_data->m_OsdItf );
}

/******************************************************************************
 * CameleonDevice::CopyFlag()
 *****************************************************************************/
bool CameleonDevice::GetCopyFlag() const
{
    return ( d_data->m_protocol->CopyFlag( d_data->m_userCtx ) );
}

/******************************************************************************
 * CameleonDevice::setCopyFlag()
 *****************************************************************************/
void CameleonDevice::SetCopyFlag( const bool flag )
{
    return ( d_data->m_protocol->setCopyFlag( d_data->m_userCtx, flag ) );
}

/******************************************************************************
 * CameleonDevice::setComChannel()
 *****************************************************************************/
void CameleonDevice::setComChannel( ComChannel * c )
{
    ProVideoDevice::setComChannel( c );

    GetIspItf()     ->SetComChannel( c );
    GetCprocItf()   ->SetComChannel( c );
    GetMccItf()     ->SetComChannel( c );
    GetChainItf()   ->SetComChannel( c );
    GetOsdItf()     ->SetComChannel( c );
 }

/******************************************************************************
 * CameleonDevice::resync()
 *****************************************************************************/
void CameleonDevice::resync()
{
    ProVideoDevice::resync();

    GetIspItf()     ->resync();
    GetCprocItf()   ->resync();
    GetMccItf()     ->resync();
    GetChainItf()   ->resync();
    GetOsdItf()     ->resync();
}
