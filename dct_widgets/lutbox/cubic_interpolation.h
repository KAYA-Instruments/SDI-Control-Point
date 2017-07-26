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
#ifndef __CUBIC_INTERPOLATION_H__
#define __CUBIC_INTERPOLATION_H__

class CubicInterpolation
{
public:
    explicit CubicInterpolation();
    ~CubicInterpolation();

    void clear();
    void setSamples( QVector<int> &x, QVector<int> &y );
    int interpolate( int x );

private:
    class PrivateData;
    PrivateData * d_data;
};

#endif // __CUBIC_INTERPOLATION_H__

