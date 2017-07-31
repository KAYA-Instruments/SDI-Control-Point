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
 * @file    rs232.h
 *
 * @brief   Serial interface abstraction
 *
 *****************************************************************************/
#ifndef __RS232_H__
#define __RS232_H__

int rs232_init( void );
int rs232_get_no_ports( void );
int rs232_get_port_name( int const idx, ctrl_channel_name_t name );
int rs232_open( int const idx, int const baudrate, int const data, int const parity, int const stop );
void rs232_close( int const idx );
int rs232_poll( int const idx, char * const buf, int const size );
int rs232_send_byte( int const idx, char const byte );
int rs232_send( int const idx, char * const buf, int const size );

#endif /* __RS232_H__ */

