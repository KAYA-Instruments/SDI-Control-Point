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
#ifndef __XYZ2CT_H__
#define __XYZ2CT_H__

#include <stdint.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

int XYZtoCorColorTemp_Robertson( double * xyz, double * temp );
int XYZtoCorColorTemp_McCamy( double * xyz, double * temp );
int CorColorTemptoXYZ( double ct, double * xyz );

#ifdef __cplusplus
}
#endif

#endif /* __XYZ2CT_H__ */

