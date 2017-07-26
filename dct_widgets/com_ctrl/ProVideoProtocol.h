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

