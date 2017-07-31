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

