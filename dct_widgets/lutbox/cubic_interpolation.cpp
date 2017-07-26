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
 * @file    lutbox.cpp
 *
 * @brief   Implementation of a cubic spline interpolation
 *
 *****************************************************************************/
#include <QtDebug>
#include <simple_math/cubic.h>

#include "cubic_interpolation.h"

/******************************************************************************
 * CubicInterpolation::PrivateData
 *****************************************************************************/
class CubicInterpolation::PrivateData
{
public:
    PrivateData()
    {
        sm_cubic_spline_interpolation_reset( &m_ctx );
    };
 
    ~PrivateData()
    {
        clear();
    };

    void clear()
    {
        sm_cubic_spline_interpolation_reset( &m_ctx );

        memset( m_x, 0, sizeof(spline_interpolation_sample_type_t) * CSIP_MAX_SAMPLES );
        memset( m_y, 0, sizeof(spline_interpolation_sample_type_t) * CSIP_MAX_SAMPLES );
    };

    void setSamples( QVector<int> &x, QVector<int> &y )
    {
        int res;

        Q_ASSERT( x.count() == y.count() );
        Q_ASSERT( (unsigned int)x.count() < CSIP_MAX_SAMPLES );

        clear();

        for (int i = 0; i<x.count(); i++ )
        {
            m_x[i] = CSIP_SAMPLE_TYPE( x[i] );
            m_y[i] = CSIP_SAMPLE_TYPE( y[i] );
        }
        
        res = sm_cubic_spline_interpolation_set_samples( &m_ctx, m_x, m_y, x.count() );
        Q_ASSERT( res == 0 );
        
        res = sm_cubic_spline_interpolation_calc_init( &m_ctx );
        Q_ASSERT( res == 0 );
    };
    
    int interpolate( int x )
    {
        int res;

        spline_interpolation_sample_type_t y = CSIP_SAMPLE_TYPE( 0 );
        res = sm_cubic_spline_interpolation_calc( &m_ctx, CSIP_SAMPLE_TYPE(x), &y );
        Q_ASSERT( res == 0 );

        return ( (int)y );
    }

    spline_interpolation_ctx_t              m_ctx;
    spline_interpolation_sample_type_t      m_x[CSIP_MAX_SAMPLES];
    spline_interpolation_sample_type_t      m_y[CSIP_MAX_SAMPLES];
};

/******************************************************************************
 * CubicInterpolation::CubicInterpolation
 *****************************************************************************/
CubicInterpolation::CubicInterpolation()
{
    // create private data container
    d_data = new PrivateData();
}

/******************************************************************************
 * CubicInterpolation::~CubicInterpolation
 *****************************************************************************/
CubicInterpolation::~CubicInterpolation()
{
    delete d_data;
}

/******************************************************************************
 * CubicInterpolation::clear
 *****************************************************************************/
void CubicInterpolation::clear()
{
    d_data->clear();
}

/******************************************************************************
 * CubicInterpolation::setSamples
 *****************************************************************************/
void CubicInterpolation::setSamples( QVector<int> &x, QVector<int> &y )
{
    d_data->setSamples( x, y );
}

/******************************************************************************
 * CubicInterpolation::interpolate
 *****************************************************************************/
int CubicInterpolation::interpolate( int x )
{
    return ( d_data->interpolate( x ) );
}

