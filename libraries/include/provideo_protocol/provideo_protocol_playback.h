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
 * @file    provideo_protocol_playback.h
 *
 * @brief   ProVideo playback protocol implementation
 *
 *****************************************************************************/
#ifndef __PROVIDEO_PROTOCOL_PLAYBACK_H__
#define __PROVIDEO_PROTOCOL_PLAYBACK_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @brief      Register the playback protocol implementation at control protocol layer
 *
 * @param[in]  handle    Instance handle of control protocol layer
 *
 * @return     0 on success, error-code otherwise
 *****************************************************************************/
int provideo_protocol_playback_init
(
    ctrl_protocol_handle_t const handle
);

#ifdef __cplusplus
}
#endif

#endif /* __PROVIDEO_PROTOCOL_PLAYBACK_H__ */

