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

