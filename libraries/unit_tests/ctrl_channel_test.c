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

