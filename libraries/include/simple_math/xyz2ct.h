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

