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
 * @file    provideo_protocol_tflt_tests.c
 *
 * @brief   Test implementation of provideo protocol for tflt
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
#include <ctrl_protocol/ctrl_protocol_tflt.h>

// specific control channel implementation
#include <rs232/ctrl_channel_rs232.h>

// specific control protocol implementation
#include <provideo_protocol/provideo_protocol.h>

/******************************************************************************
 * local definitions
 *****************************************************************************/
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
 * test_tflt - assertion checks if tflt commands works
 *****************************************************************************/
static void test_tflt( void )
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
    open_config.baudrate = 115200u;

    res = ctrl_channel_open( channel, &open_config, sizeof(open_config) );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_tflt_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current tflt configuration
    res = ctrl_protocol_get_tflt_enable( protocol, channel, &mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_tflt_enable( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_tflt_enable( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_tflt_enable( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_tflt_enable( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_tflt_enable( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_tflt_enable( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_tflt_enable( protocol, channel, mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_tflt_min_max - assertion checks if tflt_min_max command works
 *****************************************************************************/
static void test_tflt_min_max( void )
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
    int test_value3 = 15;

    int test_value4 = -1;

    ctrl_protocol_tflt_index_t index;
    ctrl_protocol_tflt_index_t value;
    ctrl_protocol_tflt_index_t v;

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

    res = provideo_protocol_tflt_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current tflt configuration
    res = ctrl_protocol_get_tflt_min_max( protocol, channel, sizeof(index), (uint8_t *)&index );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    value.min = test_value1;
    value.max = test_value1;
    res = ctrl_protocol_set_tflt_min_max( protocol, channel, sizeof(value), (uint8_t *)&value );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_tflt_min_max( protocol, channel, sizeof(v), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v.min );
    TEST_ASSERT_EQUAL_INT( test_value1, v.max );

    value.min = test_value2;
    value.max = test_value2;
    res = ctrl_protocol_set_tflt_min_max( protocol, channel, sizeof(value), (uint8_t *)&value );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_tflt_min_max( protocol, channel, sizeof(v), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v.min );
    TEST_ASSERT_EQUAL_INT( test_value2, v.max );

    value.min = test_value3;
    value.max = test_value3;
    res = ctrl_protocol_set_tflt_min_max( protocol, channel, sizeof(value), (uint8_t *)&value );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_tflt_min_max( protocol, channel, sizeof(v), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v.min );
    TEST_ASSERT_EQUAL_INT( test_value3, v.max );

    value.min = test_value1;
    value.max = test_value2;
    res = ctrl_protocol_set_tflt_min_max( protocol, channel, sizeof(value), (uint8_t *)&value );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_tflt_min_max( protocol, channel, sizeof(v), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v.min );
    TEST_ASSERT_EQUAL_INT( test_value2, v.max );

    value.min = test_value1;
    value.max = test_value3;
    res = ctrl_protocol_set_tflt_min_max( protocol, channel, sizeof(value), (uint8_t *)&value );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_tflt_min_max( protocol, channel, sizeof(v), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v.min );
    TEST_ASSERT_EQUAL_INT( test_value3, v.max );

    // TEST CASE ANTI-FUNCTIONAL
    value.min = test_value3;
    value.max = test_value1;
    res = ctrl_protocol_set_tflt_min_max( protocol, channel, sizeof(value), (uint8_t *)&value );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_tflt_min_max( protocol, channel, sizeof(v), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v.min );
    TEST_ASSERT_EQUAL_INT( test_value3, v.max );

    value.min = test_value2;
    value.max = test_value1;
    res = ctrl_protocol_set_tflt_min_max( protocol, channel, sizeof(value), (uint8_t *)&value );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_tflt_min_max( protocol, channel, sizeof(v), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v.min );
    TEST_ASSERT_EQUAL_INT( test_value3, v.max );

    value.min = test_value4;
    value.max = test_value4;
    res = ctrl_protocol_set_tflt_min_max( protocol, channel, sizeof(value), (uint8_t *)&value );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_tflt_min_max( protocol, channel, sizeof(v), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v.min );
    TEST_ASSERT_EQUAL_INT( test_value3, v.max );

    // restore pre-test configuration
    res = ctrl_protocol_set_tflt_min_max( protocol, channel, sizeof(index), (uint8_t *)&index );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_tflt_denoise_level - assertion checks if tflt_denoise_level command works
 *****************************************************************************/
static void test_tflt_denoise_level( void )
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

    int test_value1[20] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
    
    int test_value2 = 20;
    int test_value3 = -1;

    uint8_t level;
    uint8_t v;

    unsigned i;

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

    res = provideo_protocol_tflt_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current tflt configuration
    res = ctrl_protocol_get_tflt_denoise_level( protocol, channel, &level );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    for ( i = 0u; i<ARRAY_SIZE(test_value1); i++ )
    {
        res = ctrl_protocol_set_tflt_denoise_level( protocol, channel, test_value1[i] );
        TEST_ASSERT_EQUAL_INT( 0, res );
        res = ctrl_protocol_get_tflt_denoise_level( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value1[i], v );
    }
    
    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_tflt_denoise_level( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_tflt_denoise_level( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1[ARRAY_SIZE(test_value1)-1], v );

    res = ctrl_protocol_set_tflt_denoise_level( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_tflt_denoise_level( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1[ARRAY_SIZE(test_value1)-1], v );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_tflt_denoise_level( protocol, channel, level );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef provideo_protocol_tflt_tests( void )
{
    EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "tflt"              , test_tflt ),
        new_TestFixture( "tflt_min_max"      , test_tflt_min_max ),
        new_TestFixture( "tflt_denoise_level", test_tflt_denoise_level ),
    };
    EMB_UNIT_TESTCALLER( provideo_protocol_tflt_test, "TFLT_PROCESSING", setup, teardown, fixtures );

    return ( (TestRef)&provideo_protocol_tflt_test );
}


