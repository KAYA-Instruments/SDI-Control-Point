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
 * @file    provideo_protocol.h
 *
 * @brief   ProVideo serial command protocol
 *
 *****************************************************************************/
#ifndef __PROVIDEO_PROTOCOL_H__
#define __PROVIDEO_PROTOCOL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <ctrl_protocol/ctrl_protocol.h>

/**************************************************************************//**
 * @brief      Provideo Protocol User Context
 *****************************************************************************/
typedef struct provideo_protocol_user_ctx_s
{
    uint8_t     use_copy_flag;  /**< use copy flag, to configure all processing chains in once */
} provideo_protocol_user_ctx_t;

/**************************************************************************//**
 * @brief      Initialize a ProVideo System command control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_sys_init( ctrl_protocol_handle_t const, void * const );

/**************************************************************************//**
 * @brief      Initialize a ProVideo ISP command control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_isp_init( ctrl_protocol_handle_t const, void * const );

/**************************************************************************//**
 * @brief      Initialize a ProVideo Color Processing command control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_cproc_init( ctrl_protocol_handle_t const, void * const );

/**************************************************************************//**
 * @brief      Initialize a ProVideo Auto command control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_auto_init( ctrl_protocol_handle_t const, void * const );

/**************************************************************************//**
 * @brief      Initialize a ProVideo Multi Color Controller command control
 *             protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_mcc_init( ctrl_protocol_handle_t const, void * const );

/**************************************************************************//**
 * @brief      Initialize a ProVideo Chain command control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_chain_init( ctrl_protocol_handle_t const, void * const );

/**************************************************************************//**
 * @brief      Initialize a ProVideo Gamma/Lookup Tablbe command control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_lut_init( ctrl_protocol_handle_t const, void * const );

/**************************************************************************//**
 * @brief      Initialize a ProVideo Camera command control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_cam_init( ctrl_protocol_handle_t const, void * const );

/**************************************************************************//**
 * @brief      Initialize a ProVideo Temporal filter command control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_tflt_init( ctrl_protocol_handle_t const, void * const );

/**************************************************************************//**
 * @brief      Initialize a ProVideo Fix pattern noise correction command
 *             control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_fpnc_init( ctrl_protocol_handle_t const, void * const );

/**************************************************************************//**
 * @brief      Initialize a ProVideo Playback command control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_playback_init( ctrl_protocol_handle_t const, void * const );

/**************************************************************************//**
 * @brief      Initialize a ProVideo On screen display command control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_osd_init( ctrl_protocol_handle_t const, void * const  );

/**************************************************************************//**
 * @brief      Initialize a ProVideo Timecode command control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_timecode_init( ctrl_protocol_handle_t const, void * const  );

/**************************************************************************//**
 * @brief      Initialize a ProVideo Iris command control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_iris_init( ctrl_protocol_handle_t const, void * const  );

/**************************************************************************//**
 * @brief      Initialize a ProVideo Knee command control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_knee_init( ctrl_protocol_handle_t const, void * const  );

/**************************************************************************//**
 * @brief      Initialize a ProVideo Dpcc command control protocol
 *
 * @param[in]  handle   protocol instance to initialize
 * @param[in]  ctx      user protocol context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_dpcc_init( ctrl_protocol_handle_t const, void * const  );

#ifdef __cplusplus
}
#endif

#endif /* __PROVIDEO_PROTOCOL_H__ */

