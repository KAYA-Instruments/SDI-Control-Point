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
 * @file    knee.cpp
 *
 * @brief   Implementation of a cubic spline interpolation
 *
 *****************************************************************************/
#include <QtDebug>
#include <simple_math/knee.h>

#include "common.h"
#include "knee_interpolation.h"

#define CFG_WDR_INPUT_WIDTH         ( 12u )         /**< input bit-width of wdr module */
#define CFG_WDR_GAIN_COMMA          ( 14u )         /**< comma position of wdr gain table  */
#define CFG_WDR_GAIN_MASK           ( 0x0003ffffu ) /**< value 18 bit in FP4.14 */

/******************************************************************************
 * KneeInterpolation::PrivateData
 *****************************************************************************/
class KneeInterpolation::PrivateData
{
public:
    PrivateData()
        : m_ctx( NULL ),
          knee( 0 ),
          slope( 0 ),
          clip( 0 )
    {
        sm_knee_interpolation_init( &m_ctx );
        if ( !m_ctx )
        {
            showError( -ENOMEM, __FILE__, __FUNCTION__, __LINE__ );
            return;
        }
        Q_ASSERT( m_ctx != NULL );

        clear();
    };
 
    ~PrivateData()
    {
        clear();
        m_ctx = NULL;
    };

    void clear()
    {
        int res = sm_knee_interpolation_reset( m_ctx );
        Q_ASSERT( res == 0 );
    };

    void setConfig( uint8_t knee, uint16_t slope, uint8_t clip )
    {
        int res;

        clear();
        
        res = sm_knee_interpolation_calc_init( m_ctx, knee, slope, clip );
        Q_ASSERT( res == 0 );
    }

    int interpolate( int x )
    {
        int res;

        uint32_t  y = 0;

        // compute new gain
        res = sm_knee_interpolation_calc( m_ctx, (uint32_t)x, &y, CFG_WDR_INPUT_WIDTH, CFG_WDR_GAIN_COMMA );
        Q_ASSERT( res == 0 );

        // clip gain
        y &= CFG_WDR_GAIN_MASK;
 
        return ( (int)y );
    }

    knee_interpolation_ctx_t *  m_ctx;
    uint8_t                     knee;
    uint16_t                    slope;
    uint8_t                     clip;
};

/******************************************************************************
 * KneeInterpolation::KneeInterpolation
 *****************************************************************************/
KneeInterpolation::KneeInterpolation()
{
    // create private data container
    d_data = new PrivateData();
}

/******************************************************************************
 * KneeInterpolation::~KneeInterpolation
 *****************************************************************************/
KneeInterpolation::~KneeInterpolation()
{
    delete d_data;
}

/******************************************************************************
 * KneeInterpolation::setConfig
 *****************************************************************************/
void KneeInterpolation::setConfig( uint8_t knee, uint16_t slope, uint8_t clip )
{
    d_data->setConfig( knee, slope, clip );
}

/******************************************************************************
 * KneeInterpolation::setConfig
 *****************************************************************************/
int KneeInterpolation::interpolate( int x )
{
    return ( d_data->interpolate( x ) );
}

