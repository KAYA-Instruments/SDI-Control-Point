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
 * @file    common.h
 *
 * @brief   
 *
 *****************************************************************************/
#ifndef _COMMON_H_
#define _COMMON_H_

void showError( int const res, const char * fn, const char * func, int const line );

#define HANDLE_ERROR( res )                                 \
    if ( res )                                              \
    {                                                       \
        showError( res, __FILE__, __FUNCTION__, __LINE__ ); \
        return;                                             \
    }

#define HANDLE_ERROR_RETURN( res )                          \
    if ( res )                                              \
    {                                                       \
        showError( res, __FILE__, __FUNCTION__, __LINE__ ); \
        return false;                                       \
    }


#endif // _COMMON_H_

