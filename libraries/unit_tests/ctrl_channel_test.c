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
 * @file    ctrl_channel_tests.c
 *
 * @brief   implementation of control channel unit tests
 *
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>

#include <embUnit/embUnit.h>

#include <ctrl_channel/ctrl_channel.h>
#include <ctrl_channel/ctrl_channel_serial.h>

#include <rs232/ctrl_channel_rs232.h>

/******************************************************************************
 * global variables
 *****************************************************************************/
extern uint8_t g_com_port;

/******************************************************************************
 * called by test-framework before test-procedure
 *****************************************************************************/
static void setup( void )
{
}

/******************************************************************************
 * called by test-framwork after test-procedure
 *****************************************************************************/
static void teardown( void )
{
}

/******************************************************************************
 * test_ctrl_channel_rs232_init
 * - test to initialize a RS232 control channel
 *****************************************************************************/
static void test_ctrl_channel_rs232_init( void )
{
    uint8_t mem[ctrl_channel_get_instance_size()];

    ctrl_channel_rs232_context_t    priv;
    ctrl_channel_handle_t           channel;

    ctrl_channel_name_t             name;

    int res;
    int no;
    int i;
    
    channel = (ctrl_channel_handle_t)mem;
    TEST_ASSERT( ctrl_channel_get_instance_size() > 0 );
    memset( channel, 0, ctrl_channel_get_instance_size() );

    res = ctrl_channel_rs232_init( channel, &priv );
    TEST_ASSERT_EQUAL_INT( 0, res );

    no = ctrl_channel_get_no_ports( channel );
    TEST_ASSERT( no > 0 );
   
    for ( i = 0; i<no; i++ )
    {
        memset( name, 0, sizeof(ctrl_channel_name_t) );
        res = ctrl_channel_get_port_name( channel, i, name );
        TEST_ASSERT( strlen(name) > 0 );
    }
}

/******************************************************************************
 * test_ctrl_channel_open
 * - test to open and close a RS232 control channel
 *****************************************************************************/
static void test_ctrl_channel_rs232_open( void )
{
    uint8_t mem[ctrl_channel_get_instance_size()];

    ctrl_channel_rs232_context_t    priv;
    ctrl_channel_handle_t           channel;

    ctrl_channel_rs232_open_config_t open_config;

    int res;
    int no;
   
    channel = (ctrl_channel_handle_t)mem;
    TEST_ASSERT( ctrl_channel_get_instance_size() > 0 );
    memset( channel, 0, ctrl_channel_get_instance_size() );

    res = ctrl_channel_rs232_init( channel, &priv );
    TEST_ASSERT_EQUAL_INT( 0, res );

    no = ctrl_channel_get_no_ports( channel );
    TEST_ASSERT( no > 0 );

    memset( &open_config, 0, sizeof(ctrl_channel_rs232_open_config_t) );

    open_config.idx      = g_com_port;
    open_config.data     = CTRL_CHANNEL_DATA_BITS_8;
    open_config.parity   = CTRL_CHANNEL_PARITY_NONE;
    open_config.stop     = CTRL_CHANNEL_STOP_BITS_1;
    open_config.baudrate = 115200u;

    res = ctrl_channel_open( channel, &open_config, sizeof(open_config) );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef ctrl_channel_tests( void )
{
	EMB_UNIT_TESTFIXTURES( fixtures )
    {
		new_TestFixture( "ctrl_channel_rs232_init", test_ctrl_channel_rs232_init ),
		new_TestFixture( "ctrl_channel_rs232_open", test_ctrl_channel_rs232_open ),
	};
	EMB_UNIT_TESTCALLER( ctrl_channel_test, "CTRL-CHANNEL", setup, teardown, fixtures );

	return ( (TestRef)&ctrl_channel_test );
}

