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
 * @file    ctrl_channel_generic.h
 *
 * @brief   ProVideo Generic interface definitions
 *
 *****************************************************************************/
#ifndef __CTRL_CHANNEL_GENERIC_H__
#define __CTRL_CHANNEL_GENERIC_H__

#include <ctrl_channel/ctrl_channel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @brief generic open configuration for channels, it's more a transfer object
 *****************************************************************************/
typedef struct ctrl_channel_generic_open_config_s
{
    void * config;      /**< pointer to a specific open configuration */
    int    size;        /**< size of specific open configuration */
} ctrl_channel_generic_config_t;

#ifdef __cplusplus
}
#endif

#endif /* __CTRL_CHANNEL_GENERIC_H__ */

