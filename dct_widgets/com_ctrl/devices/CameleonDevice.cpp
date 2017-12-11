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
    deviceFeatures.hasCprocItfHue           = true;
    deviceFeatures.hasMccItf                = true;
    deviceFeatures.hasSystemSaveLoad        = true;
    deviceFeatures.hasSystemCopySettings    = true;
    deviceFeatures.hasOsdItf                = true;
    deviceFeatures.hasOsdTestPattern        = true;

    deviceFeatures.lutBitWidth              = 12;

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
