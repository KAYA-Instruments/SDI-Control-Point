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
 * @file    id.h
 *
 * @brief   Unique camera system identifier (used by GUI)
 *
 *****************************************************************************/
#ifndef __ID_H__
#define __ID_H__

enum SystemId
{
    SYSTEM_ID_INVALID       = -1,   // unknown system
    SYSTEM_ID_ATOM_ONE      = 0,    // ATOM ONE (X-BOW)
    SYSTEM_ID_ATOM_ONE_4K   = 1,    // ATOM ONE 4K (CONDOR)
    SYSTEM_ID_MAX
};

#endif // __ID_H__

