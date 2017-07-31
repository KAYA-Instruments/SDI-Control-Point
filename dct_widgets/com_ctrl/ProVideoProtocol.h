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
 * @file    ProVideoProtocol.h
 *
 * @brief   Definition of a proVideo control protocol 
 *
 *****************************************************************************/
#ifndef _PROVIDEO_PROTOCOL_H_
#define _PROVIDEO_PROTOCOL_H_

// generic control protocol layer
#include "ComProtocol.h"

// ProVideoProtocol
class ProVideoProtocol : public ComProtocol
{
public:
    explicit ProVideoProtocol( void * usrCtx = NULL );
    explicit ProVideoProtocol( instanceType i, void * usrCtx = NULL );

    ProVideoProtocol * clone( instanceType i, void * usrCtx = NULL ) const override;
    void * createUserContext() const override;
    void deleteUserContext( void * UsrCtx ) override;

    bool CopyFlag( void * UsrCtx ) const override;
    void setCopyFlag( void * UsrCtx, const bool flag ) override;
};

#endif // _PROVIDEO_PROTOCOL_H_

