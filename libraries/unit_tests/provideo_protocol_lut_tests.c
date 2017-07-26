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
 * @file    provideo_protocol_lut_tests.c
 *
 * @brief   Test implementation of provideo protocol for lut
 *
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <simple_math/simple_math.h>

#include <embUnit/embUnit.h>

// generic control channel layer
#include <ctrl_channel/ctrl_channel.h>
#include <ctrl_channel/ctrl_channel_serial.h>

// generic control protocol layer
#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_system.h>
#include <ctrl_protocol/ctrl_protocol_lut.h>

// specific control channel implementation
#include <rs232/ctrl_channel_rs232.h>

// specific control protocol implementation
#include <provideo_protocol/provideo_protocol.h>

/******************************************************************************
 * local definitions
 *****************************************************************************/
#ifdef _WIN32
#define sleep(n)        Sleep(1000 * n)
#endif

#define ARRAY_SIZE(x)   ( sizeof(x)/sizeof(x[0]) )

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
 * test_lut_enable - assertion checks if lut_enable command works
 *****************************************************************************/
static void test_lut_enable( void )
{
    // reserve memory for control channel instance
    uint8_t channel_mem[ctrl_channel_get_instance_size()];
    
    // reserve memory for protocol instance
    uint8_t protocol_mem[ctrl_protocol_get_instance_size()];

    ctrl_channel_rs232_context_t        channel_priv;
    ctrl_channel_handle_t               channel;
    ctrl_channel_rs232_open_config_t    open_config;

    ctrl_protocol_handle_t              protocol;

    int res;
    int no;

    int test_value1 = 0;
    int test_value2 = 1;
    int test_value3 = -1;
    
    ctrl_protocol_enable_t mode = { .id = 0, .flag = 0 };

    ctrl_protocol_enable_t t;
    ctrl_protocol_enable_t v;

    // initialize control channel
    channel = (ctrl_channel_handle_t)channel_mem;
    TEST_ASSERT( ctrl_channel_get_instance_size() > 0 );
    memset( channel, 0, ctrl_channel_get_instance_size() );

    memset( &channel_priv, 0, sizeof(channel_priv) );
    res = ctrl_channel_rs232_init( channel, &channel_priv );
    TEST_ASSERT_EQUAL_INT( 0, res );

    no = ctrl_channel_get_no_ports( channel );
    TEST_ASSERT( no >= g_com_port );

    // open control channel
    memset( &open_config, 0, sizeof(ctrl_channel_rs232_open_config_t) );

    open_config.idx      = g_com_port;
    open_config.data     = CTRL_CHANNEL_DATA_BITS_8;
    open_config.parity   = CTRL_CHANNEL_PARITY_NONE;
    open_config.stop     = CTRL_CHANNEL_STOP_BITS_1;
    open_config.baudrate = 115200u;

    res = ctrl_channel_open( channel, &open_config, sizeof(open_config) );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_lut_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current gamma enable configuration
    res = ctrl_protocol_get_lut_enable( protocol, channel, sizeof(mode), (uint8_t *)&mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    t.id    = 0;
    t.flag  = test_value1;
    res = ctrl_protocol_set_lut_enable( protocol, channel, sizeof(t), (uint8_t *)&t );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    v.id    = 0;
    v.flag  = 255;
    res = ctrl_protocol_get_lut_enable( protocol, channel, sizeof(v), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( t.id  , v.id );
    TEST_ASSERT_EQUAL_INT( test_value1, v.flag );

    t.id    = 0;
    t.flag  = test_value2;
    res = ctrl_protocol_set_lut_enable( protocol, channel, sizeof(t), (uint8_t *)&t );
    TEST_ASSERT_EQUAL_INT( 0, res );

    v.id    = 0;
    v.flag  = 255;
    res = ctrl_protocol_get_lut_enable( protocol, channel, sizeof(v), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( t.id  , v.id );
    TEST_ASSERT_EQUAL_INT( test_value2, v.flag );

    // TEST CASE ANTI-FUNCTIONAL
    t.id    = 0;
    t.flag  = test_value3;
    res = ctrl_protocol_set_lut_enable( protocol, channel, sizeof(t), (uint8_t *)&t );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    
    v.id    = 0;
    v.flag  = 255;
    res = ctrl_protocol_get_lut_enable( protocol, channel, sizeof(v), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( t.id  , v.id );
    TEST_ASSERT_EQUAL_INT( test_value2, v.flag );

    // restore pre-test configuration
    res = ctrl_protocol_set_lut_enable( protocol, channel, sizeof(mode), (uint8_t *)&mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_lut_write_index - assertion checks if lut_write_addr command works
 *****************************************************************************/
static void test_lut_write_index( void )
{
    // reserve memory for control channel instance
    uint8_t channel_mem[ctrl_channel_get_instance_size()];

    // reserve memory for sys-protocol instance
    uint8_t protocol_mem_sys[ctrl_protocol_get_instance_size()];

    // reserve memory for protocol instance
    uint8_t protocol_mem[ctrl_protocol_get_instance_size()];

    ctrl_channel_rs232_context_t        channel_priv;
    ctrl_channel_handle_t               channel;
    ctrl_channel_rs232_open_config_t    open_config;

    ctrl_protocol_handle_t              protocol;
    ctrl_protocol_handle_t              protocol_sys;

    int res;
    int no;

    int test_value1 = 100;
    int test_value2 = 200;
    int test_value3 = 300;
    int test_value4 = 400;
    int test_value5 = 500;

    int test_value6 = 4096;
    int test_value7 = -1;

    uint8_t debug;

    uint16_t red;
    uint16_t green;
    uint16_t blue;
    
    ctrl_protocol_enable_t mode = { .id = 0, .flag = 0 };
    uint16_t v;

    // initialize control channel
    channel = (ctrl_channel_handle_t)channel_mem;
    TEST_ASSERT( ctrl_channel_get_instance_size() > 0 );
    memset( channel, 0, ctrl_channel_get_instance_size() );

    memset( &channel_priv, 0, sizeof(channel_priv) );
    res = ctrl_channel_rs232_init( channel, &channel_priv );
    TEST_ASSERT_EQUAL_INT( 0, res );

    no = ctrl_channel_get_no_ports( channel );
    TEST_ASSERT( no >= g_com_port );

    // open control channel
    memset( &open_config, 0, sizeof(ctrl_channel_rs232_open_config_t) );

    open_config.idx      = g_com_port;
    open_config.data     = CTRL_CHANNEL_DATA_BITS_8;
    open_config.parity   = CTRL_CHANNEL_PARITY_NONE;
    open_config.stop     = CTRL_CHANNEL_STOP_BITS_1;
    open_config.baudrate = 115200u;

    res = ctrl_channel_open( channel, &open_config, sizeof(open_config) );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol lut-instance
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_lut_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol sys-instance
    protocol_sys = (ctrl_protocol_handle_t)protocol_mem_sys;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol_sys, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_sys_init( protocol_sys, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current gamma enable/index configuration and system debug level
    res = ctrl_protocol_get_debug( protocol_sys, channel, &debug );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_lut_enable( protocol, channel, sizeof(mode), (uint8_t *)&mode );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_lut_write_index_red( protocol, channel, &red );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_lut_write_index_green( protocol, channel, &green );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_lut_write_index_blue( protocol, channel, &blue );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // prepare system for testing
    res = ctrl_protocol_set_debug( protocol_sys, channel, 20u );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL

    // test_value1
    res = ctrl_protocol_set_lut_write_index( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_lut_write_index_red( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );
    res = ctrl_protocol_get_lut_write_index_green( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );
    res = ctrl_protocol_get_lut_write_index_blue( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // test_value2
    res = ctrl_protocol_set_lut_write_index( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_lut_write_index_red( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );
    res = ctrl_protocol_get_lut_write_index_green( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );
    res = ctrl_protocol_get_lut_write_index_blue( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    // test different values, test_value3, test_value4, test_value5
    res = ctrl_protocol_set_lut_write_index_red( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_lut_write_index_green( protocol, channel, test_value4 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_lut_write_index_blue( protocol, channel, test_value5 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_lut_write_index_red( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );
    res = ctrl_protocol_get_lut_write_index_green( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value4, v );
    res = ctrl_protocol_get_lut_write_index_blue( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value5, v );

    // TEST CASE ANTI-FUNCTIONAL

    // test_value3
    res = ctrl_protocol_set_lut_write_index( protocol, channel, test_value6 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_lut_write_index_red( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );
    res = ctrl_protocol_get_lut_write_index_green( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value4, v );
    res = ctrl_protocol_get_lut_write_index_blue( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value5, v );

    // test_value4
    res = ctrl_protocol_set_lut_write_index( protocol, channel, test_value7 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_lut_write_index_red( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );
    res = ctrl_protocol_get_lut_write_index_green( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value4, v );
    res = ctrl_protocol_get_lut_write_index_blue( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value5, v );
     
    // restore pre-test configuration
    res = ctrl_protocol_set_lut_write_index_red( protocol, channel, red );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_lut_write_index_green( protocol, channel, green );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_lut_write_index_blue( protocol, channel, blue );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_lut_enable( protocol, channel, sizeof(mode), (uint8_t *)&mode );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_debug( protocol_sys, channel, debug );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_lut_read - assertion checks if lut_read command works
 *****************************************************************************/
static void test_lut_read( void )
{
    // reserve memory for control channel instance
    uint8_t channel_mem[ctrl_channel_get_instance_size()];

    // reserve memory for sys-protocol instance
    uint8_t protocol_mem_sys[ctrl_protocol_get_instance_size()];

    // reserve memory for protocol instance
    uint8_t protocol_mem[ctrl_protocol_get_instance_size()];

    ctrl_channel_rs232_context_t        channel_priv;
    ctrl_channel_handle_t               channel;
    ctrl_channel_rs232_open_config_t    open_config;

    ctrl_protocol_handle_t              protocol;
    ctrl_protocol_handle_t              protocol_sys;

    int res;
    int no;

    uint16_t values_red[MAX_VALUES_LUT];
    uint16_t values_green[MAX_VALUES_LUT];
    uint16_t values_blue[MAX_VALUES_LUT];
    
    uint8_t debug;

    ctrl_protocol_enable_t mode = { .id = 0, .flag = 0 };

    uint16_t red;
    uint16_t green;
    uint16_t blue;
    
    // initialize control channel
    channel = (ctrl_channel_handle_t)channel_mem;
    TEST_ASSERT( ctrl_channel_get_instance_size() > 0 );
    memset( channel, 0, ctrl_channel_get_instance_size() );

    memset( &channel_priv, 0, sizeof(channel_priv) );
    res = ctrl_channel_rs232_init( channel, &channel_priv );
    TEST_ASSERT_EQUAL_INT( 0, res );

    no = ctrl_channel_get_no_ports( channel );
    TEST_ASSERT( no >= g_com_port );

    // open control channel
    memset( &open_config, 0, sizeof(ctrl_channel_rs232_open_config_t) );

    open_config.idx      = g_com_port;
    open_config.data     = CTRL_CHANNEL_DATA_BITS_8;
    open_config.parity   = CTRL_CHANNEL_PARITY_NONE;
    open_config.stop     = CTRL_CHANNEL_STOP_BITS_1;
    open_config.baudrate = 115200u;

    res = ctrl_channel_open( channel, &open_config, sizeof(open_config) );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol lut-instance
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_lut_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol sys-instance
    protocol_sys = (ctrl_protocol_handle_t)protocol_mem_sys;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol_sys, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_sys_init( protocol_sys, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current gamma enable/index configuration and system debug level
    res = ctrl_protocol_get_debug( protocol_sys, channel, &debug );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_lut_enable( protocol, channel, sizeof(mode), (uint8_t *)&mode );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_lut_write_index_red( protocol, channel, &red );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_lut_write_index_green( protocol, channel, &green );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_lut_write_index_blue( protocol, channel, &blue );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // prepare system for testing
    res = ctrl_protocol_set_debug( protocol_sys, channel, 20u );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_lut_write_index( protocol, channel, 0 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    // TEST CASE FUNCTIONAL
    memset( values_red, 0, sizeof(values_red) );
    res = ctrl_protocol_get_lut_read_red( protocol, channel, ARRAY_SIZE(values_red), values_red );
    TEST_ASSERT_EQUAL_INT( 0, res );

    memset( values_green, 0, sizeof(values_green) );
    res = ctrl_protocol_get_lut_read_green( protocol, channel, ARRAY_SIZE(values_green), values_green );
    TEST_ASSERT_EQUAL_INT( 0, res );

    memset( values_blue, 0, sizeof(values_blue) );
    res = ctrl_protocol_get_lut_read_blue( protocol, channel, ARRAY_SIZE(values_blue), values_blue );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE ANTI-FUNCTIONAL
     
    // restore pre-test configuration
    res = ctrl_protocol_set_lut_write_index_red( protocol, channel, red );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_lut_write_index_green( protocol, channel, green );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_lut_write_index_blue( protocol, channel, blue );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_lut_enable( protocol, channel, sizeof(mode), (uint8_t *)&mode );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_debug( protocol_sys, channel, debug );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef provideo_protocol_lut_tests( void )
{
    EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "lut_enable"     , test_lut_enable ),
        new_TestFixture( "lut_write_index", test_lut_write_index ),
        new_TestFixture( "lut_read"       , test_lut_read ),
    };
    EMB_UNIT_TESTCALLER( provideo_protocol_lut_test, "PROVIDEO-PROTOCOL-GAMMA", setup, teardown, fixtures );

    return ( (TestRef)&provideo_protocol_lut_test );
}


