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
 * @file    ctrl_protocol_tests.c
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

#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_isp.h>
#include <ctrl_protocol/ctrl_protocol_system.h>

/******************************************************************************
 * global variables
 *****************************************************************************/

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
 * test_ctrl_protocol_init
 * - test to initialize a control protocol instance
 *****************************************************************************/
static void test_ctrl_protocol_init( void )
{
    uint8_t mem[ctrl_protocol_get_instance_size()];

    ctrl_protocol_handle_t protocol;
    
    int res;

    protocol = (ctrl_protocol_handle_t)mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = ctrl_protocol_get_gain_red( protocol, NULL, NULL ); 
    TEST_ASSERT_EQUAL_INT(  res, -EOPNOTSUPP );
}

/******************************************************************************
 * test_ctrl_protocol_isp_register
 * - test to register an ISP protocol driver at control protocol instance
 *****************************************************************************/
static uint16_t g_test_gain;

static int test_set_gain_red
(
    void * const                 ctx,
    ctrl_channel_handle_t const  channel, 
    uint16_t const               gain
)
{
    (void) ctx;
    (void) channel;

    g_test_gain = gain;

    return ( 0 );
}

static int test_get_gain_red
(
    void * const                 ctx,
    ctrl_channel_handle_t const  channel, 
    uint16_t * const             gain
)
{
    (void) ctx;
    (void) channel;

    *gain = g_test_gain;

    return ( 0 );
}

static ctrl_protocol_isp_drv_t test_isp_drv = 
{
    .set_gain_red   = test_set_gain_red,
    .get_gain_red   = test_get_gain_red,
};


static void test_ctrl_protocol_isp_register( void )
{
    uint8_t mem[ctrl_protocol_get_instance_size()];

    ctrl_protocol_handle_t protocol;

    int res;

    uint16_t value;

    protocol = (ctrl_protocol_handle_t)mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = ctrl_protocol_isp_register( protocol, NULL, &test_isp_drv );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_get_gain_red( protocol, NULL, NULL ); 
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );

    // TEST CASE FUNCTIONAL
    g_test_gain = 123;
    res = ctrl_protocol_get_gain_red( protocol, NULL, &value ); 
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( 123, value );

    res = ctrl_protocol_set_gain_red( protocol, NULL, 42 ); 
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( 42, g_test_gain );
    
    res = ctrl_protocol_get_gain_red( protocol, NULL, &value ); 
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( g_test_gain, value );

    res = ctrl_protocol_isp_unregister( protocol );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_ctrl_protocol_init
 * - test to register a SYSTEM protocol driver at control protocol instance
 *****************************************************************************/
static int test_get_system_id
(
    void * const                ctx,
    ctrl_channel_handle_t const channel,
    int const                   no,
    uint8_t * const             values
)
{
    (void) ctx;
    (void) channel;

    ctrl_protocol_device_id_t * device_id;

    if ( !no || !values || (no != sizeof(ctrl_protocol_device_id_t)) )
    {
        return ( -EINVAL );
    }

    device_id = (ctrl_protocol_device_id_t *)values;
    device_id->id0 = 0x01234567u;
    device_id->id1 = 0x87654321u;
    device_id->id2 = 0x11223344u;

    return ( 0 );
}

static ctrl_protocol_sys_drv_t test_sys_drv = 
{
    .get_system_id = test_get_system_id,
};

static void test_ctrl_protocol_sys_register( void )
{
    uint8_t mem[ctrl_protocol_get_instance_size()];

    ctrl_protocol_handle_t protocol;

    int res;

    ctrl_protocol_device_id_t device_id;
    
    protocol = (ctrl_protocol_handle_t)mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = ctrl_protocol_sys_register( protocol, NULL, &test_sys_drv );
    TEST_ASSERT_EQUAL_INT( 0, res );

    memset( &device_id, 0, sizeof(device_id) );

    res = ctrl_protocol_get_system_id( protocol, NULL, sizeof(device_id), (uint8_t *)&device_id ); 
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( 0x01234567u, device_id.id0 );
    TEST_ASSERT_EQUAL_INT( 0x87654321u, device_id.id1 );
    TEST_ASSERT_EQUAL_INT( 0x11223344u, device_id.id2 );
 
    res = ctrl_protocol_sys_unregister( protocol );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef ctrl_protocol_tests( void )
{
	EMB_UNIT_TESTFIXTURES( fixtures )
    {
		new_TestFixture( "ctrl_protocol_init"        , test_ctrl_protocol_init ),
		new_TestFixture( "ctrl_protocol_isp_register", test_ctrl_protocol_isp_register ),
		new_TestFixture( "ctrl_protocol_sys_register", test_ctrl_protocol_sys_register ),
	};
	EMB_UNIT_TESTCALLER( ctrl_protocol_test, "CTRL-PROTOCOL", setup, teardown, fixtures );

	return ( (TestRef)&ctrl_protocol_test );
}

