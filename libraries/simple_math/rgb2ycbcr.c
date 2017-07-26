/**************************************************************************//**
 *
 * Copyright 2016, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
#include <stdint.h>
#include <string.h>
#include <errno.h>

#include <simple_math/rgb2ycbcr.h>

/******************************************************************************
 * local definitions
 *****************************************************************************/

/******************************************************************************
 * cal_YCbCr_coefficients_bt601
 *****************************************************************************/
int cal_YCbCr_coefficients_bt601
(
    float * c0, float * c1, float * c2,
    float * c3, float * c4, float * c5,
    float * c6, float * c7, float * c8
)
{
    if ( !c0 || !c1 || !c2 || !c3 || !c4 || !c5 || !c6 || !c7 || !c8 )
    {
        return ( -EINVAL );
    }

    *c0 =  Kr_BT601;
    *c1 =  Kg_BT601;
    *c2 =  Kb_BT601;

    *c3 = -(Cb_MAX * Kr_BT601) / (1.0000f - Kb_BT601);
    *c4 = -(Cb_MAX * Kg_BT601) / (1.0000f - Kb_BT601);
    *c5 =   Cb_MAX;
    
    *c6 =   Cr_MAX;
    *c7 = -(Cr_MAX * Kg_BT601) / (1.0000f - Kr_BT601);
    *c8 = -(Cr_MAX * Kb_BT601) / (1.0000f - Kr_BT601);
    
    return ( 0 );
}

/******************************************************************************
 * cal_YCbCr_coefficients_bt709
 *****************************************************************************/
int cal_YCbCr_coefficients_bt709
(
    float * c0, float * c1, float * c2,
    float * c3, float * c4, float * c5,
    float * c6, float * c7, float * c8
)
{
    if ( !c0 || !c1 || !c2 || !c3 || !c4 || !c5 || !c6 || !c7 || !c8 )
    {
        return ( -EINVAL );
    }

    *c0 =  Kr_BT709;
    *c1 =  Kg_BT709;
    *c2 =  Kb_BT709;

    *c3 = -(Cb_MAX * Kr_BT709) / (1.0000f - Kb_BT709);
    *c4 = -(Cb_MAX * Kg_BT709) / (1.0000f - Kb_BT709);
    *c5 =   Cb_MAX;
    
    *c6 =   Cr_MAX;
    *c7 = -(Cr_MAX * Kg_BT709) / (1.0000f - Kr_BT709);
    *c8 = -(Cr_MAX * Kb_BT709) / (1.0000f - Kr_BT709);

    return ( 0 );
}

/******************************************************************************
 * cal_YCbCr_coefficients_smpte240M
 *****************************************************************************/
int cal_YCbCr_coefficients_smpte240M
(
    float * c0, float * c1, float * c2,
    float * c3, float * c4, float * c5,
    float * c6, float * c7, float * c8
)
{
    if ( !c0 || !c1 || !c2 || !c3 || !c4 || !c5 || !c6 || !c7 || !c8 )
    {
        return ( -EINVAL );
    }

    *c0 =  Kr_SMPTE240M;
    *c1 =  Kg_SMPTE240M;
    *c2 =  Kb_SMPTE240M;

    *c3 = -(Cb_MAX * Kr_SMPTE240M) / (1.0000f - Kb_SMPTE240M);
    *c4 = -(Cb_MAX * Kg_SMPTE240M) / (1.0000f - Kb_SMPTE240M);
    *c5 =   Cb_MAX;
    
    *c6 =   Cr_MAX;
    *c7 = -(Cr_MAX * Kg_SMPTE240M) / (1.0000f - Kr_SMPTE240M);
    *c8 = -(Cr_MAX * Kb_SMPTE240M) / (1.0000f - Kr_SMPTE240M);

    return ( 0 );
}

/******************************************************************************
 * cal_YCbCr_coefficients
 *****************************************************************************/
int cal_YCbCr_coefficients
(
    float Kr, float Kb,
    float * c0, float * c1, float * c2,
    float * c3, float * c4, float * c5,
    float * c6, float * c7, float * c8
)
{
    float Kg;

    if ( (Kr < 0.000f) || (Kr > 1.000f) || 
         (Kb < 0.000f) || (Kb > 1.000f) ||
         ((Kr + Kb) > 1.000f)           )
    {
        return ( -EINVAL );
    }

    if ( !c0 || !c1 || !c2 || !c3 || !c4 || !c5 || !c6 || !c7 || !c8 )
    {
        return ( -EINVAL );
    }

    Kg = 1.000f - Kr - Kb;

    *c0 = Kr;
    *c1 = Kg;
    *c2 = Kb;

    *c3 = -(Cb_MAX * Kr) / (1.0000f - Kb);
    *c4 = -(Cb_MAX * Kg) / (1.0000f - Kb);
    *c5 =   Cb_MAX;
    
    *c6 =   Cr_MAX;
    *c7 = -(Cr_MAX * Kg) / (1.0000f - Kr);
    *c8 = -(Cr_MAX * Kb) / (1.0000f - Kr);

    return ( 0 );
}
 
