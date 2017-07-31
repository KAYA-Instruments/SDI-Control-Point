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
 * @file    AdamDevice.cpp
 *
 * @brief   
 *
 *****************************************************************************/
#include <cerrno>

#include "common.h"
#include "AdamDevice.h"

#include <QtDebug>

/******************************************************************************
 * hardware mask 
 *****************************************************************************/
// @TODO: At the moment Adam devices do not have a HW mask

/******************************************************************************
 * hardware mask interpreter
 *****************************************************************************/
class AdamDevice::HwMaskInterpreter : public MaskInterpreter
{
public:
    QStringList interpret( const uint32_t mask ) override
    {
        (void) mask;
        QStringList features;
        features << "System: Adam";

        // Correction Modules
        features << "Supported correction functions:";
        features << "- Defect/Hot pixel correction available";

        return ( features );
    }
};

/******************************************************************************
 * software mask interpreter
 *****************************************************************************/
class AdamDevice::SwMaskInterpreter : public MaskInterpreter
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
 * AdamDevice::PrivateData
 *****************************************************************************/
class AdamDevice::PrivateData
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
        m_LutItf        = new LutItf( c, p->clone( ComProtocol::LUT_INSTANCE, m_userCtx ) );
        m_ChainItf      = new ChainItf( c, p->clone( ComProtocol::CHAIN_INSTANCE, m_userCtx ) );
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
        delete m_LutItf->GetComProtocol();
        delete m_ChainItf->GetComProtocol();
        delete m_DpccItf->GetComProtocol();

        // clear interface classes
        delete m_IspItf;
        delete m_CprocItf;
        delete m_AutoItf;
        delete m_CamItf;
        delete m_LutItf;
        delete m_ChainItf;
        delete m_DpccItf;

        // clear user context 
        m_protocol->deleteUserContext( m_userCtx );
        m_userCtx = NULL;
    };

    IspItf *        m_IspItf;
    CprocItf *      m_CprocItf;
    AutoItf *       m_AutoItf;
    CamItf *        m_CamItf;
    LutItf *        m_LutItf;
    ChainItf *      m_ChainItf;
    DpccItf *       m_DpccItf;

    ComProtocol *   m_protocol;
    void *          m_userCtx;
};

/******************************************************************************
 * AdamDevice::AdamDevice()
 *****************************************************************************/
AdamDevice::AdamDevice( ComChannel * c, ComProtocol * p )
    : ProVideoDevice( c, p )
{
    d_data = new PrivateData( c, p );

    // set mask interpretation classes ton interface class
    GetProVideoSystemItf()->SetMaskHwInterpreter( new HwMaskInterpreter() );
    GetProVideoSystemItf()->SetMaskSwInterpreter( new SwMaskInterpreter() );
}

/******************************************************************************
 * AdamDevice::getSupportedFeatures()
 *****************************************************************************/
AdamDevice::features AdamDevice::getSupportedFeatures()
{
    // Init all features with false
    features deviceFeatures;
    memset( &deviceFeatures, 0, sizeof (deviceFeatures)) ;

    // Set all available features to true
    deviceFeatures.hasCamItf                = true;
    deviceFeatures.hasChainItf              = true;
    deviceFeatures.hasChainRawMode          = true;
    deviceFeatures.hasChainHdmiSettings     = true;
    deviceFeatures.hasAutoItf               = true;
    deviceFeatures.hasIspItf                = true;
    deviceFeatures.hasIspFlare              = true;
    deviceFeatures.hasIspFilter             = true;
    deviceFeatures.hasIspAutoWhiteBalance   = true;
    deviceFeatures.hasIspGain               = true;
    deviceFeatures.hasIspConversion         = true;
    deviceFeatures.hasIspMasterBlackLevel   = true;
    deviceFeatures.hasCprocItf              = true;
    deviceFeatures.hasLutItf                = true;
    deviceFeatures.hasDpccItf               = true;

    return ( deviceFeatures );
}

/******************************************************************************
 * AdamDevice::~AdamDevice()
 *****************************************************************************/
AdamDevice::~AdamDevice()
{
    delete d_data;
}

/******************************************************************************
 * AdamDevice::GetIspItf()
 *****************************************************************************/
IspItf * AdamDevice::GetIspItf() const
{
    return ( d_data->m_IspItf );
}

/******************************************************************************
 * AdamDevice::GetCprocItf()
 *****************************************************************************/
CprocItf * AdamDevice::GetCprocItf() const
{
    return ( d_data->m_CprocItf );
}

/******************************************************************************
 * AdamDevice::GetAutoItf()
 *****************************************************************************/
AutoItf * AdamDevice::GetAutoItf() const
{
    return ( d_data->m_AutoItf );
}

/******************************************************************************
 * AdamDevice::GetCamItf()
 *****************************************************************************/
CamItf * AdamDevice::GetCamItf() const
{
    return ( d_data->m_CamItf );
}

/******************************************************************************
 * AdamDevice::GetLutItf()
 *****************************************************************************/
LutItf * AdamDevice::GetLutItf() const
{
    return ( d_data->m_LutItf );
}

/******************************************************************************
 * AdamDevice::GetChainItf()
 *****************************************************************************/
ChainItf * AdamDevice::GetChainItf() const
{
    return ( d_data->m_ChainItf );
}

/******************************************************************************
 * AdamDevice::GetDpccItf()
 *****************************************************************************/
DpccItf * AdamDevice::GetDpccItf() const
{
    return ( d_data->m_DpccItf );
}

/******************************************************************************
 * AdamDevice::CopyFlag()
 *****************************************************************************/
bool AdamDevice::GetCopyFlag() const
{
    return ( d_data->m_protocol->CopyFlag( d_data->m_userCtx ) );
}

/******************************************************************************
 * AdamDevice::setCopyFlag()
 *****************************************************************************/
void AdamDevice::SetCopyFlag( const bool flag )
{
    return ( d_data->m_protocol->setCopyFlag( d_data->m_userCtx, flag ) );
}

/******************************************************************************
 * AdamDevice::setComChannel()
 *****************************************************************************/
void AdamDevice::setComChannel( ComChannel * c )
{
    ProVideoDevice::setComChannel( c );

    GetIspItf()     ->SetComChannel( c );
    GetCprocItf()   ->SetComChannel( c );
    GetAutoItf()    ->SetComChannel( c );
    GetCamItf()     ->SetComChannel( c );
    GetLutItf()     ->SetComChannel( c );
    GetChainItf()   ->SetComChannel( c );
    GetDpccItf()    ->SetComChannel( c );
 }

/******************************************************************************
 * AdamDevice::resync()
 *****************************************************************************/
void AdamDevice::resync()
{
    ProVideoDevice::resync();

    GetIspItf()     ->resync();
    GetCprocItf()   ->resync();
    GetAutoItf()    ->resync();
    GetCamItf()     ->resync();
    GetLutItf()     ->resync();
    GetChainItf()   ->resync();
    GetDpccItf()    ->resync();
}


