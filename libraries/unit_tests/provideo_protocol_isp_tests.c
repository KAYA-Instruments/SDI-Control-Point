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
 * @file    provideo_protocol_isp_tests.c
 *
 * @brief   Test implementation of provideo protocol for isp
 *
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <stdint.h>

#include <embUnit/embUnit.h>

// generic control channel layer
#include <ctrl_channel/ctrl_channel.h>
#include <ctrl_channel/ctrl_channel_serial.h>

// generic control protocol layer
#include <ctrl_protocol/ctrl_protocol.h>
#include <ctrl_protocol/ctrl_protocol_isp.h>

// specific control channel implementation
#include <rs232/ctrl_channel_rs232.h>

// specific control protocol implementation
#include <provideo_protocol/provideo_protocol.h>

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
 * test_bayer - assertion checks if bayer request work
 *****************************************************************************/
static void test_bayer( void )
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

    int test_value1 = 3;
    int test_value2 = 5025;
    
    uint8_t bayer;
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
    open_config.baudrate = 115200u;

    res = ctrl_channel_open( channel, &open_config, sizeof(open_config) );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_isp_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current bayer configuration
    res = ctrl_protocol_get_bayer( protocol, channel, &bayer );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_bayer( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_bayer( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_bayer( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_bayer( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // restore pre-test configuration
    res = ctrl_protocol_set_bayer( protocol, channel, bayer );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_bayer - assertion checks if white balance gain requests work
 *****************************************************************************/
static void test_gains( void )
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

    int test_value1 = 234;
    int test_value2 = -1;

    uint16_t gain_red;
    uint16_t gain_green;
    uint16_t gain_blue;

    uint16_t v0;
    uint16_t v1;
    uint16_t v2;

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

    res = provideo_protocol_isp_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current gain configuration
    res = ctrl_protocol_get_gain_red( protocol, channel, &gain_red );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_gain_green( protocol, channel, &gain_green );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_gain_blue( protocol, channel, &gain_blue );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_gain_red( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_gain_red( protocol, channel, &v0 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v0 );

    res = ctrl_protocol_set_gain_red( protocol, channel, gain_red );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_set_gain_green( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_gain_green( protocol, channel, &v1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v1 );

    res = ctrl_protocol_set_gain_green( protocol, channel, gain_green );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_set_gain_blue( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_gain_blue( protocol, channel, &v2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v2 );

    res = ctrl_protocol_set_gain_blue( protocol, channel, gain_blue );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_gain_red( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_gain_red( protocol, channel, &v0 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( gain_red, v0 );

    res = ctrl_protocol_set_gain_green( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_gain_green( protocol, channel, &v1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( gain_green, v1 );

    res = ctrl_protocol_set_gain_blue( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_gain_blue( protocol, channel, &v2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( gain_blue, v2 );

    // restore pre-test configuration
    res = ctrl_protocol_set_gain_red( protocol, channel, gain_red );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_gain_green( protocol, channel, gain_green );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_gain_blue( protocol, channel, gain_blue );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_black_level - assertion checks if all black_... commands work
 *****************************************************************************/
static void test_black_level( void )
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

    int test_value1 = 333;
    int test_value2 = -333;
    int test_value3 = 5025;
    int test_value4 = -5025;

    int16_t black_red;
    int16_t black_green;
    int16_t black_blue;

    int16_t v0;
    int16_t v1;
    int16_t v2;

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

    res = provideo_protocol_isp_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current black level configuration
    res = ctrl_protocol_get_black_red( protocol, channel, &black_red );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_black_green( protocol, channel, &black_green );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_black_blue( protocol, channel, &black_blue );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL

    // 1.) positive value (> 0)
    res = ctrl_protocol_set_black_red( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_black_red( protocol, channel, &v0 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v0 );

    res = ctrl_protocol_set_black_red( protocol, channel, black_red );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_set_black_green( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_black_green( protocol, channel, &v1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v1 );

    res = ctrl_protocol_set_black_green( protocol, channel, black_green );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_set_black_blue( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_black_blue( protocol, channel, &v2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v2 );

    res = ctrl_protocol_set_black_blue( protocol, channel, black_blue );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // 2.) negative value (< 0)
    res = ctrl_protocol_set_black_red( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_black_red( protocol, channel, &v0 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v0 );

    res = ctrl_protocol_set_black_red( protocol, channel, black_red );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_set_black_green( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_black_green( protocol, channel, &v1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v1 );

    res = ctrl_protocol_set_black_green( protocol, channel, black_green );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_set_black_blue( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_black_blue( protocol, channel, &v2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v2 );

    res = ctrl_protocol_set_black_blue( protocol, channel, black_blue );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_black_red( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_black_red( protocol, channel, &v0 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( black_red, v0 );

    res = ctrl_protocol_set_black_green( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_black_green( protocol, channel, &v1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( black_green, v1 );

    res = ctrl_protocol_set_black_blue( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_black_blue( protocol, channel, &v2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( black_blue, v2 );

    res = ctrl_protocol_set_black_red( protocol, channel, test_value4 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_black_red( protocol, channel, &v0 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( black_red, v0 );

    res = ctrl_protocol_set_black_green( protocol, channel, test_value4 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_black_green( protocol, channel, &v1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( black_green, v1 );

    res = ctrl_protocol_set_black_blue( protocol, channel, test_value4 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_black_blue( protocol, channel, &v2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( black_blue, v2 );

    // restore pre-test configuration
    res = ctrl_protocol_set_black_red( protocol, channel, black_red );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_black_green( protocol, channel, black_green );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_set_black_blue( protocol, channel, black_blue );
    TEST_ASSERT_EQUAL_INT( 0, res );

     // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_flare - assertion checks if all flare commands work
 *****************************************************************************/
static void test_flare( void )
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

    int test_value0 =     0;
    int test_value1 =  3000;
    int test_value2 = 65535;

    uint16_t flare[NO_VALUES_ISP_FLARE];
    uint16_t value[NO_VALUES_ISP_FLARE];
    uint16_t v[NO_VALUES_ISP_FLARE];

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

    res = provideo_protocol_isp_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current flare compensation level configuration
    res = ctrl_protocol_get_flare( protocol, channel, NO_VALUES_ISP_FLARE, flare );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    value[0] = test_value0;
    value[1] = test_value0;
    value[2] = test_value0;
    res = ctrl_protocol_set_flare( protocol, channel, NO_VALUES_ISP_FLARE, value ); 
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_flare( protocol, channel, NO_VALUES_ISP_FLARE, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value0, v[0] );
    TEST_ASSERT_EQUAL_INT( test_value0, v[1] );
    TEST_ASSERT_EQUAL_INT( test_value0, v[2] );

    value[0] = test_value1;
    value[1] = test_value1;
    value[2] = test_value1;
    res = ctrl_protocol_set_flare( protocol, channel, NO_VALUES_ISP_FLARE, value ); 
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_flare( protocol, channel, NO_VALUES_ISP_FLARE, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v[0] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[1] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[2] );

    value[0] = test_value2;
    value[1] = test_value2;
    value[2] = test_value2;
    res = ctrl_protocol_set_flare( protocol, channel, NO_VALUES_ISP_FLARE, value ); 
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_flare( protocol, channel, NO_VALUES_ISP_FLARE, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v[0] );
    TEST_ASSERT_EQUAL_INT( test_value2, v[1] );
    TEST_ASSERT_EQUAL_INT( test_value2, v[2] );
      
    value[0] = test_value0;
    value[1] = test_value1;
    value[2] = test_value2;
    res = ctrl_protocol_set_flare( protocol, channel, NO_VALUES_ISP_FLARE, value ); 
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_flare( protocol, channel, NO_VALUES_ISP_FLARE, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value0, v[0] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[1] );
    TEST_ASSERT_EQUAL_INT( test_value2, v[2] );
 
    // TEST CASE ANTI-FUNCTIONAL

    // restore pre-test configuration
    res = ctrl_protocol_set_flare( protocol, channel, NO_VALUES_ISP_FLARE, flare );
    TEST_ASSERT_EQUAL_INT( 0, res );    

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_bayer - assertion checks if back_master commands work
 *****************************************************************************/
static void test_black_master( void )
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

    int test_value1 = 30;
    int test_value2 = -30;
    int test_value3 = 5025;
    int test_value4 = 5025;
    
    int16_t black[NO_VALUES_BLACK_MASTER];
    int16_t v_set[NO_VALUES_BLACK_MASTER];
    int16_t v[NO_VALUES_BLACK_MASTER];
    
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

    res = provideo_protocol_isp_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current black master configuration
    res = ctrl_protocol_get_black_master( protocol, channel, NO_VALUES_BLACK_MASTER, black );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    v_set[0] = test_value1;
    v_set[1] = test_value1;
    v_set[2] = test_value1;
    res = ctrl_protocol_set_black_master( protocol, channel, NO_VALUES_BLACK_MASTER, v_set );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_black_master( protocol, channel, NO_VALUES_BLACK_MASTER, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v[0] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[1] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[2] );

    v_set[0] = test_value2;
    v_set[1] = test_value2;
    v_set[2] = test_value2;
    res = ctrl_protocol_set_black_master( protocol, channel, NO_VALUES_BLACK_MASTER, v_set );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_black_master( protocol, channel, NO_VALUES_BLACK_MASTER, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v[0] );
    TEST_ASSERT_EQUAL_INT( test_value2, v[1] );
    TEST_ASSERT_EQUAL_INT( test_value2, v[2] );

    // TEST CASE ANTI-FUNCTIONAL
    v_set[0] = test_value3;
    v_set[1] = test_value3;
    v_set[2] = test_value3;
    res = ctrl_protocol_set_black_master( protocol, channel, NO_VALUES_BLACK_MASTER, v_set );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_black_master( protocol, channel, NO_VALUES_BLACK_MASTER, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    // check that old values are functional
    TEST_ASSERT_EQUAL_INT( test_value2, v[0] );
    TEST_ASSERT_EQUAL_INT( test_value2, v[1] );
    TEST_ASSERT_EQUAL_INT( test_value2, v[2] );

    v_set[0] = test_value4;
    v_set[1] = test_value4;
    v_set[2] = test_value4;
    res = ctrl_protocol_set_black_master( protocol, channel, NO_VALUES_BLACK_MASTER, v_set );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_black_master( protocol, channel, NO_VALUES_BLACK_MASTER, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    // check that old values are functional
    TEST_ASSERT_EQUAL_INT( test_value2, v[0] );
    TEST_ASSERT_EQUAL_INT( test_value2, v[1] );
    TEST_ASSERT_EQUAL_INT( test_value2, v[2] );
   
    // restore pre-test configuration
    res = ctrl_protocol_set_black_master( protocol, channel, NO_VALUES_BLACK_MASTER, black );
    TEST_ASSERT_EQUAL_INT( 0, res );    

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_filter_enable - assertion checks if filter_enable function works
 *****************************************************************************/
static void test_filter_enable( void )
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

    int test_value1 = 1;
    int test_value2 = 0;

    uint8_t flag;
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
    open_config.baudrate = 115200u;

    res = ctrl_channel_open( channel, &open_config, sizeof(open_config) );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_isp_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current filter-enable configuration
    res = ctrl_protocol_get_filter_enable( protocol, channel, &flag );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_filter_enable( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_filter_enable( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_filter_enable( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_filter_enable( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    // TEST CASE ANTI-FUNCTIONAL

    // restore pre-test configuration
    res = ctrl_protocol_set_filter_enable( protocol, channel, flag );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_filter_detail - assertion checks if filter_detail commands work
 *****************************************************************************/
static void test_filter_detail( void )
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

    int test_value1 =  3;
    int test_value2 = 78;
    int test_value3 = -8;

    uint8_t level;
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
    open_config.baudrate = 115200u;

    res = ctrl_channel_open( channel, &open_config, sizeof(open_config) );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_isp_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current filter-detail configuration
    res = ctrl_protocol_get_filter_detail( protocol, channel, &level );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_filter_detail( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_filter_detail( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_filter_detail( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_filter_detail( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_filter_detail( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_filter_detail( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // restore pre-test configuration
    res = ctrl_protocol_set_filter_detail( protocol, channel, level );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_flter_denoise - assertion checks if filter_denoise commands work
 *****************************************************************************/
static void test_filter_denoise( void )
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
    int test_value2 = 66;
    int test_value3 = -8;

    uint8_t level;
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
    open_config.baudrate = 115200u;

    res = ctrl_channel_open( channel, &open_config, sizeof(open_config) );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_isp_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current filter-detail configuration
    res = ctrl_protocol_get_filter_denoise( protocol, channel, &level );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_filter_denoise( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_filter_denoise( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_filter_denoise( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_filter_denoise( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_filter_denoise( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_filter_denoise( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // restore pre-test configuration
    res = ctrl_protocol_set_filter_denoise( protocol, channel, level );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_color_conv - assertion checks if color_conv commands work
 *****************************************************************************/
static void test_color_conv( void )
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

    int test_value0 = 0;
    int test_value1 = -5033;
    int test_value2 = 8200;

    int16_t coeff[NO_VALUES_COLOR_CONVERSION];
    int16_t v_set[NO_VALUES_COLOR_CONVERSION];
    int16_t v[NO_VALUES_COLOR_CONVERSION];

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

    res = provideo_protocol_isp_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current color-conversion configuration
    res = ctrl_protocol_get_color_conv( protocol, channel, NO_VALUES_COLOR_CONVERSION, coeff );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    v_set[0] = test_value1;
    v_set[1] = test_value0;
    v_set[2] = test_value0;
    v_set[3] = test_value0;
    v_set[4] = test_value1;
    v_set[5] = test_value0;
    v_set[6] = test_value0;
    v_set[7] = test_value0;
    v_set[8] = test_value1;
    res = ctrl_protocol_set_color_conv( protocol, channel, NO_VALUES_COLOR_CONVERSION, v_set );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_color_conv( protocol, channel, NO_VALUES_COLOR_CONVERSION, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v[0] );
    TEST_ASSERT_EQUAL_INT( test_value0, v[1] );
    TEST_ASSERT_EQUAL_INT( test_value0, v[2] );
    TEST_ASSERT_EQUAL_INT( test_value0, v[3] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[4] );
    TEST_ASSERT_EQUAL_INT( test_value0, v[5] );
    TEST_ASSERT_EQUAL_INT( test_value0, v[6] );
    TEST_ASSERT_EQUAL_INT( test_value0, v[7] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[8] );

    v_set[0] = test_value1;
    v_set[1] = test_value1;
    v_set[2] = test_value1;
    v_set[3] = test_value1;
    v_set[4] = test_value1;
    v_set[5] = test_value1;
    v_set[6] = test_value1;
    v_set[7] = test_value1;
    v_set[8] = test_value1;
    res = ctrl_protocol_set_color_conv( protocol, channel, NO_VALUES_COLOR_CONVERSION, v_set );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_color_conv( protocol, channel, NO_VALUES_COLOR_CONVERSION, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v[0] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[1] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[2] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[3] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[4] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[5] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[6] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[7] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[8] );

    v_set[0] = 1;
    v_set[1] = 2;
    v_set[2] = 3;
    v_set[3] = 4;
    v_set[4] = 5;
    v_set[5] = 6;
    v_set[6] = 7;
    v_set[7] = 8;
    v_set[8] = 9;
    res = ctrl_protocol_set_color_conv( protocol, channel, NO_VALUES_COLOR_CONVERSION, v_set );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_color_conv( protocol, channel, NO_VALUES_COLOR_CONVERSION, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( 1, v[0] );
    TEST_ASSERT_EQUAL_INT( 2, v[1] );
    TEST_ASSERT_EQUAL_INT( 3, v[2] );
    TEST_ASSERT_EQUAL_INT( 4, v[3] );
    TEST_ASSERT_EQUAL_INT( 5, v[4] );
    TEST_ASSERT_EQUAL_INT( 6, v[5] );
    TEST_ASSERT_EQUAL_INT( 7, v[6] );
    TEST_ASSERT_EQUAL_INT( 8, v[7] );
    TEST_ASSERT_EQUAL_INT( 9, v[8] );

    // TEST CASE ANTI-FUNCTIONAL
    v_set[0] = test_value2;
    v_set[1] = test_value2;
    v_set[2] = test_value2;
    v_set[3] = test_value2;
    v_set[4] = test_value2;
    v_set[5] = test_value2;
    v_set[6] = test_value2;
    v_set[7] = test_value2;
    v_set[8] = test_value2;
    res = ctrl_protocol_set_color_conv( protocol, channel, NO_VALUES_COLOR_CONVERSION, v_set ); 
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_color_conv( protocol, channel, NO_VALUES_COLOR_CONVERSION, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( 1, v[0] );
    TEST_ASSERT_EQUAL_INT( 2, v[1] );
    TEST_ASSERT_EQUAL_INT( 3, v[2] );
    TEST_ASSERT_EQUAL_INT( 4, v[3] );
    TEST_ASSERT_EQUAL_INT( 5, v[4] );
    TEST_ASSERT_EQUAL_INT( 6, v[5] );
    TEST_ASSERT_EQUAL_INT( 7, v[6] );
    TEST_ASSERT_EQUAL_INT( 8, v[7] );
    TEST_ASSERT_EQUAL_INT( 9, v[8] );

    // restore pre-test configuration
    res = ctrl_protocol_set_color_conv( protocol, channel, NO_VALUES_COLOR_CONVERSION, coeff );
    TEST_ASSERT_EQUAL_INT( 0, res );    

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_color_cross - assertion checks if color_cross commands work
 *****************************************************************************/
static void test_color_cross( void )
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

    int test_value0 = 0;
    int test_value1 = 12033;
    int test_value2 = -80200;

    int32_t coeff[NO_VALUES_CROSS];
    int32_t v_set[NO_VALUES_CROSS];
    int32_t v[NO_VALUES_CROSS];

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

    res = provideo_protocol_isp_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current cross-talk configuration
    res = ctrl_protocol_get_color_cross( protocol, channel, NO_VALUES_CROSS, coeff );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    v_set[0] = test_value1;
    v_set[1] = test_value0;
    v_set[2] = test_value0;
    v_set[3] = test_value0;
    v_set[4] = test_value1;
    v_set[5] = test_value0;
    v_set[6] = test_value0;
    v_set[7] = test_value0;
    v_set[8] = test_value1;
    res = ctrl_protocol_set_color_cross( protocol, channel, NO_VALUES_CROSS, v_set );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_color_cross( protocol, channel, NO_VALUES_CROSS, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v[0] );
    TEST_ASSERT_EQUAL_INT( test_value0, v[1] );
    TEST_ASSERT_EQUAL_INT( test_value0, v[2] );
    TEST_ASSERT_EQUAL_INT( test_value0, v[3] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[4] );
    TEST_ASSERT_EQUAL_INT( test_value0, v[5] );
    TEST_ASSERT_EQUAL_INT( test_value0, v[6] );
    TEST_ASSERT_EQUAL_INT( test_value0, v[7] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[8] );

    v_set[0] = test_value1;
    v_set[1] = test_value1;
    v_set[2] = test_value1;
    v_set[3] = test_value1;
    v_set[4] = test_value1;
    v_set[5] = test_value1;
    v_set[6] = test_value1;
    v_set[7] = test_value1;
    v_set[8] = test_value1;
    res = ctrl_protocol_set_color_cross( protocol, channel, NO_VALUES_CROSS, v_set ); 
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_color_cross( protocol, channel, NO_VALUES_CROSS, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v[0] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[1] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[2] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[3] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[4] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[5] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[6] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[7] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[8] );

    v_set[0] = 1;
    v_set[1] = 2;
    v_set[2] = 3;
    v_set[3] = 4;
    v_set[4] = 5;
    v_set[5] = 6;
    v_set[6] = 7;
    v_set[7] = 8;
    v_set[8] = 9;
    res = ctrl_protocol_set_color_cross( protocol, channel, NO_VALUES_CROSS, v_set ); 
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_color_cross( protocol, channel, NO_VALUES_CROSS, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( 1, v[0] );
    TEST_ASSERT_EQUAL_INT( 2, v[1] );
    TEST_ASSERT_EQUAL_INT( 3, v[2] );
    TEST_ASSERT_EQUAL_INT( 4, v[3] );
    TEST_ASSERT_EQUAL_INT( 5, v[4] );
    TEST_ASSERT_EQUAL_INT( 6, v[5] );
    TEST_ASSERT_EQUAL_INT( 7, v[6] );
    TEST_ASSERT_EQUAL_INT( 8, v[7] );
    TEST_ASSERT_EQUAL_INT( 9, v[8] );

    // TEST CASE ANTI-FUNCTIONAL
    v_set[0] = test_value2;
    v_set[1] = test_value2;
    v_set[2] = test_value2;
    v_set[3] = test_value2;
    v_set[4] = test_value2;
    v_set[5] = test_value2;
    v_set[6] = test_value2;
    v_set[7] = test_value2;
    v_set[8] = test_value2;
     res = ctrl_protocol_set_color_cross( protocol, channel, NO_VALUES_CROSS, v_set ); 
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_color_cross( protocol, channel, NO_VALUES_CROSS, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( 1, v[0] );
    TEST_ASSERT_EQUAL_INT( 2, v[1] );
    TEST_ASSERT_EQUAL_INT( 3, v[2] );
    TEST_ASSERT_EQUAL_INT( 4, v[3] );
    TEST_ASSERT_EQUAL_INT( 5, v[4] );
    TEST_ASSERT_EQUAL_INT( 6, v[5] );
    TEST_ASSERT_EQUAL_INT( 7, v[6] );
    TEST_ASSERT_EQUAL_INT( 8, v[7] );
    TEST_ASSERT_EQUAL_INT( 9, v[8] );

    // restore pre-test configuration
    res = ctrl_protocol_set_color_cross( protocol, channel, NO_VALUES_CROSS, coeff );
    TEST_ASSERT_EQUAL_INT( 0, res );    

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_color_cross_offset - checks if color_cross_offset commands work
 *****************************************************************************/
static void test_color_cross_offset( void )
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

    int test_value1 = -3;
    int test_value2 = 5025;

    int16_t offset[NO_VALUES_CROSS_OFFSET];
    int16_t v_set[NO_VALUES_CROSS_OFFSET];
    int16_t v[NO_VALUES_CROSS_OFFSET];
    
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

    res = provideo_protocol_isp_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current cross-talk configuration
    res = ctrl_protocol_get_color_cross_offset( protocol, channel, NO_VALUES_CROSS_OFFSET, offset );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    v_set[0] = test_value1;
    v_set[1] = test_value1;
    v_set[2] = test_value1;
    res = ctrl_protocol_set_color_cross_offset( protocol, channel, NO_VALUES_CROSS_OFFSET, v_set );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_color_cross_offset( protocol, channel, NO_VALUES_CROSS_OFFSET, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v[0] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[1] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[2] );

    // TEST CASE ANTI-FUNCTIONAL
    v_set[0] = test_value2;
    v_set[1] = test_value2;
    v_set[2] = test_value2;
    res = ctrl_protocol_set_color_cross_offset( protocol, channel, NO_VALUES_CROSS_OFFSET, v_set );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_color_cross_offset( protocol, channel, NO_VALUES_CROSS_OFFSET, v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    // check that old values are functional
    TEST_ASSERT_EQUAL_INT( test_value1, v[0] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[1] );
    TEST_ASSERT_EQUAL_INT( test_value1, v[2] );

    // restore pre-test configuration
    res = ctrl_protocol_set_color_cross_offset( protocol, channel, NO_VALUES_CROSS_OFFSET, offset );
    TEST_ASSERT_EQUAL_INT( 0, res );    

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef provideo_protocol_isp_tests( void )
{
	EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "bayer"                , test_bayer ),
        new_TestFixture( "gains"                , test_gains ),
        new_TestFixture( "black-level"          , test_black_level ),
        new_TestFixture( "flare"                , test_flare ),
        new_TestFixture( "black-master"         , test_black_master ),
        new_TestFixture( "filter_enable"        , test_filter_enable ),
        new_TestFixture( "filter_detail"        , test_filter_detail ),
        new_TestFixture( "filter_denoise"       , test_filter_denoise ),
        new_TestFixture( "color_conv"           , test_color_conv ),
        new_TestFixture( "color_cross"          , test_color_cross ),
        new_TestFixture( "color_cross_offset"   , test_color_cross_offset ),
	};
	EMB_UNIT_TESTCALLER( provideo_protocol_isp_test, "PROVIDEO-PROTOCOL-ISP", setup, teardown, fixtures );

	return ( (TestRef)&provideo_protocol_isp_test );
}


