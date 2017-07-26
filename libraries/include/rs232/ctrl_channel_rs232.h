/******************************************************************************
 *
 * Copyright 2015, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
/**
 * @file    ctrl_channel_rs232.h
 *
 * @brief   ProVideo RS232/USB serial command interface
 *
 *****************************************************************************/
#ifndef __CTRL_CHANNEL_RS232_H__
#define __CTRL_CHANNEL_RS232_H__

#include <ctrl_channel/ctrl_channel.h>
#include <ctrl_channel/ctrl_channel_serial.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @brief RS232 internal context
 * @note  Memory for this context needs to be provided by the upper layer. 
 *****************************************************************************/
typedef struct ctrl_channel_rs232_context_s
{
    uint8_t     idx;                /**< system port index */
} ctrl_channel_rs232_context_t;

/**************************************************************************//**
 * @brief      Initialize a RS232 control channel instance 
 *
 * @note       memory allocation is not part of this module, so this 
 *             function provides memory to the control channel layer
 *
 * @param[in]  ch   pointer to an control channel instance object
 * @param[in]  ctx  pointer to an internal control channel context
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int ctrl_channel_rs232_init
(
    ctrl_channel_handle_t const             ch,
    ctrl_channel_rs232_context_t * const    ctx
);

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_CHANNEL_RS232_H__ */

