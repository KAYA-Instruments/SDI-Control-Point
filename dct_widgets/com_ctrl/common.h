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
 * @file    common.h
 *
 * @brief   
 *
 *****************************************************************************/
#ifndef _COMMON_H_
#define _COMMON_H_

void showError( int const res, const char * fn, const char * func, int const line );

#define HANDLE_ERROR( res )                                 \
    if ( res )                                              \
    {                                                       \
        showError( res, __FILE__, __FUNCTION__, __LINE__ ); \
        return;                                             \
    }


#endif // _COMMON_H_

