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
 * @file    provideo_protocol_auto.c
 *
 * @brief   Test implementation of provideo protocol for auto
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

#include <embUnit/embUnit.h>

// generic control channel layer
#include <ctrl_channel/ctrl_channel.h>
#include <ctrl_channel/ctrl_channel_serial.h>

// generic control protocol layer
#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_dpcc.h>

// specific control channel implementation
#include <rs232/ctrl_channel_rs232.h>

// specific control protocol implementation
#include <provideo_protocol/provideo_protocol.h>

/******************************************************************************
 * local definitions
 *****************************************************************************/
#ifdef _WIN32
#define sleep(n) Sleep(1000 * n)
#endif

#define ARRAY_SIZE(x)   (sizeof(x)/sizeof(x[0]))

/******************************************************************************
 * global variables
 *****************************************************************************/
extern uint8_t g_com_port;
extern uint32_t g_com_speed;

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
 * test_run_dpcc_enable - assertion checks if dpc_enable command work
 *****************************************************************************/
static void test_run_dpcc_enable( void )
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
    int test_value3 = 2;

    uint8_t mode;
    uint8_t v;

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
    open_config.baudrate = g_com_speed;

    res = ctrl_channel_open( channel, &open_config, sizeof(open_config) );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_dpcc_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current dpcc-enable configuration
    res = ctrl_protocol_get_dpcc_enable( protocol, channel, &mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_dpcc_enable( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_dpcc_enable( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_dpcc_enable( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_dpcc_enable( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );
    
    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_dpcc_enable( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_dpcc_enable( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    // restore pre-test configuration
    res = ctrl_protocol_set_dpcc_enable( protocol, channel, mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_run_dpcc_pixel - assertion checks if dpc_enable command work
 *****************************************************************************/
static void test_run_dpcc_pixel( void )
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
    int i;

    uint16_t pixelX[MAX_DPCC_NO_PIXEL];
    uint16_t pixelY[MAX_DPCC_NO_PIXEL];
    ctrl_protocol_dpcc_table_t table;

    uint16_t pixelX1[MAX_DPCC_NO_PIXEL];
    uint16_t pixelY1[MAX_DPCC_NO_PIXEL];
    ctrl_protocol_dpcc_table_t table1;

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
    open_config.baudrate = g_com_speed;

    res = ctrl_channel_open( channel, &open_config, sizeof(open_config) );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_dpcc_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current pixel table
    table.no   = 0u;
    table.size = MAX_DPCC_NO_PIXEL;
    table.x    = pixelX;
    table.y    = pixelY;

    // get pre-test dpcc table
    res = ctrl_protocol_get_dpcc_table( protocol, channel, sizeof(table), (uint32_t *)&table );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // clear dpcc table
    res = ctrl_protocol_clear_dpcc_table( protocol, channel );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    uint16_t px0[2] = { 3u, 3u };
    res = ctrl_protocol_add_pixel( protocol, channel, ARRAY_SIZE(px0), px0 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    uint16_t px1[2] = { 4u, 4u };
    res = ctrl_protocol_add_pixel( protocol, channel, ARRAY_SIZE(px1), px1 );
    TEST_ASSERT_EQUAL_INT( 0, res );

    uint16_t px2[2] = { 5u, 5u };
    res = ctrl_protocol_add_pixel( protocol, channel, ARRAY_SIZE(px2), px2 );
    TEST_ASSERT_EQUAL_INT( 0, res );

    table1.no   = 0u;
    table1.size = MAX_DPCC_NO_PIXEL;
    table1.x    = pixelX1;
    table1.y    = pixelY1;

    res = ctrl_protocol_get_dpcc_table( protocol, channel, sizeof(table1), (uint32_t *)&table1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    TEST_ASSERT_EQUAL_INT(                 3, table1.no );
    TEST_ASSERT_EQUAL_INT( MAX_DPCC_NO_PIXEL, table1.size );
    TEST_ASSERT_EQUAL_INT(            px0[0], table1.x[0] );
    TEST_ASSERT_EQUAL_INT(            px0[1], table1.y[0] );
    TEST_ASSERT_EQUAL_INT(            px1[0], table1.x[1] );
    TEST_ASSERT_EQUAL_INT(            px1[1], table1.y[1] );
    TEST_ASSERT_EQUAL_INT(            px2[0], table1.x[2] );
    TEST_ASSERT_EQUAL_INT(            px2[1], table1.y[2] );

    // restore pre-test configuration
    res = ctrl_protocol_clear_dpcc_table( protocol, channel );
    TEST_ASSERT_EQUAL_INT( 0, res );

    for ( i = 0; i<table.no; i++ )
    {
        uint16_t px[2];
        
        px[0] = table.x[i];
        px[1] = table.y[i];

        res = ctrl_protocol_add_pixel( protocol, channel, ARRAY_SIZE(px), px );
        TEST_ASSERT_EQUAL_INT( 0, res );
    }

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}


/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef provideo_protocol_dpcc_tests( void )
{
    EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "dpc_enable"   , test_run_dpcc_enable ),
        new_TestFixture( "dpcc_pixel"   , test_run_dpcc_pixel ),
    };
    EMB_UNIT_TESTCALLER( provideo_protocol_dpcc_test, "PROVIDEO-PROTOCOL-DPCC", setup, teardown, fixtures );

    return ( (TestRef)&provideo_protocol_dpcc_test );
}


