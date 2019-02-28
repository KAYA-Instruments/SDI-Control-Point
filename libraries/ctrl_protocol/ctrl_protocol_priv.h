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
 * @file    ctrl_protocol_priv.h
 *
 * @brief   Private control protocol definitions
 *
 *****************************************************************************/
#ifndef __CTR_PROTOCOL_PRIV_H__
#define __CTR_PROTOCOL_PRIV_H__

#include <stdint.h>

#include <ctrl_protocol/ctrl_protocol_system.h>
#include <ctrl_protocol/ctrl_protocol_cam.h>
#include <ctrl_protocol/ctrl_protocol_isp.h>
#include <ctrl_protocol/ctrl_protocol_auto.h>
#include <ctrl_protocol/ctrl_protocol_cproc.h>
#include <ctrl_protocol/ctrl_protocol_mcc.h>
#include <ctrl_protocol/ctrl_protocol_chain.h>
#include <ctrl_protocol/ctrl_protocol_lut.h>
#include <ctrl_protocol/ctrl_protocol_tflt.h>
#include <ctrl_protocol/ctrl_protocol_fpnc.h>
#include <ctrl_protocol/ctrl_protocol_playback.h>
#include <ctrl_protocol/ctrl_protocol_osd.h>
#include <ctrl_protocol/ctrl_protocol_iris.h>
#include <ctrl_protocol/ctrl_protocol_knee.h>
#include <ctrl_protocol/ctrl_protocol_dpcc.h>
#include <ctrl_protocol/ctrl_protocol_lens.h>

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * @brief Macro to check for valid handle
 *****************************************************************************/
#define CHECK_HANDLE( h )                   \
{                                           \
    if ( !h ) return ( -EFAULT );           \
}

/******************************************************************************
 * @brief Macro to check for valid handle
 *****************************************************************************/
#define CHECK_NOT_NULL( p )                 \
{                                           \
    if ( !p ) return ( -EINVAL );           \
}

/******************************************************************************
 * @brief Macro to check valid driver and driver function
 *****************************************************************************/
#define CHECK_DRV_FUNC( drv, func )         \
{                                           \
    if ( !drv || !drv->func )               \
    {                                       \
        return ( -EOPNOTSUPP );             \
    }                                       \
}

/**************************************************************************//**
 * @brief Control protocol context
 *****************************************************************************/
typedef struct ctrl_protocol_s
{
    void * ctx;
    void * drv;
#if 0
    void *                          tflt_ctx;       /**< instance context of protocol implementation */
    ctrl_protocol_tflt_drv_t *      tflt_drv;       /**< tflt driver functions of protocol implementation */
#endif    
} ctrl_protocol_t;

#ifdef __cplusplus
}
#endif

#endif /* __CTR_PROTOCOL_PRIV_H__ */

