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
 * @file    ComProtocol.cpp
 *
 * @brief   Implementation of a generic control protocol
 *
 *****************************************************************************/
#include <cerrno>
#include <cstring>

#include "common.h"
#include "ComProtocol.h"

/******************************************************************************
 * ComProtocol::ComProtocol
 *****************************************************************************/
ComProtocol::ComProtocol( instanceType i, void * )
{
    // get protocol instance size
    int size = ctrl_protocol_get_instance_size();

    // create protocol instance
    m_protocol = (ctrl_protocol_handle_t)(new char[size]);
    if ( !m_protocol )
    {
        showError( -ENOMEM, __FILE__, __FUNCTION__, __LINE__ ); 
        return;
    }

    // clear protocol instance memory
    memset( m_protocol, 0, size );

    m_instanceType = i;
}

/******************************************************************************
 * ComProtocol::~ComProtocol
 *****************************************************************************/
ComProtocol::~ComProtocol()
{
    delete[] ((char *)m_protocol);
}

