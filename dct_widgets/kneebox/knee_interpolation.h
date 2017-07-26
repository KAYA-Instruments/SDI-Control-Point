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
 * @file    interpolation.h
 *
 * @brief   Class definition of a cubic spline interpolation
 *
 *****************************************************************************/
#ifndef __KNEE_INTERPOLATION_H__
#define __KNEE_INTERPOLATION_H__

class KneeInterpolation
{
public:
    explicit KneeInterpolation();
    ~KneeInterpolation();

    void clear();
    void setConfig( uint8_t knee, uint16_t slope, uint8_t clip );
    int interpolate( int x );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __KNEE_INTERPOLATION_H__

