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
 * @file    ComProtocol.h
 *
 * @brief   Definition of a generic control protocol 
 *
 *****************************************************************************/
#ifndef _COM_PROTOCOL_H_
#define _COM_PROTOCOL_H_

// generic control protocol layer
#include <ctrl_protocol/ctrl_protocol.h>

// generic communication protocol class 
class ComProtocol
{
public:
    enum instanceType
    {
        SYS_INSTANCE = 0,
        ISP_INSTANCE,
        CPROC_INSTANCE,
        AUTO_INSTANCE,
        MCC_INSTANCE,
        CHAIN_INSTANCE,
        LUT_INSTANCE,
        CAM_INSTANCE,
        TFLT_INSTANCE,
        FPNC_INSTANCE,
        PLAYBACK_INSTANCE,
        OSD_INSTANCE,
        TIMECODE_INSTANCE,
        IRIS_INSTANCE,
        KNEE_INSTANCE,
        DPCC_INSTANCE
    };

    explicit ComProtocol( instanceType i, void * UsrCtx = NULL );
    virtual ~ComProtocol();

    instanceType GetInstanceType() const
    {
        return ( m_instanceType );
    }

    ctrl_protocol_handle_t GetInstance() const
    {
        return ( m_protocol );
    }

    virtual ComProtocol * clone( instanceType i, void * UsrCtx = NULL ) const = 0;
    virtual void * createUserContext() const = 0;
    virtual void deleteUserContext( void * UsrCtx ) = 0;
    virtual bool CopyFlag( void * UsrCtx ) const = 0;
    virtual void setCopyFlag( void * UsrCtx, const bool flag ) = 0;

private:
    ctrl_protocol_handle_t  m_protocol;         // control protocol handle
    instanceType            m_instanceType;
};

#endif // _COM_PROTOCOL_H_

