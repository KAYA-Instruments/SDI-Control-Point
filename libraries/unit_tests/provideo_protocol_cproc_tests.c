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
 * @file    provideo_protocol_cproc.c
 *
 * @brief   Test implementation of provideo protocol for cproc
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
#include <ctrl_protocol/ctrl_protocol_cproc.h>

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
 * testpost_bright - assertion checks if post_bright commands work
 *****************************************************************************/
static void test_post_bright( void )
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
    int test_value2 = 128;
    
    int16_t brightness;
    int16_t v;

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

    res = provideo_protocol_cproc_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current brightness configuration
    res = ctrl_protocol_get_post_bright( protocol, channel, &brightness );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_post_bright( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_post_bright( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_post_bright( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_post_bright( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // restore pre-test configuration
    res = ctrl_protocol_set_post_bright( protocol, channel, brightness );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_post_cont - assertion checks if post_cont commands work
 *****************************************************************************/
static void test_post_cont( void )
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
    int test_value2 = -25;

    uint16_t contrast;
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

    // initialize provideo protocol
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_cproc_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current contrast configuration
    res = ctrl_protocol_get_post_cont( protocol, channel, &contrast );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_post_cont( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_post_cont( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_post_cont( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_post_cont( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // restore pre-test configuration
    res = ctrl_protocol_set_post_cont( protocol, channel, contrast );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_post_sat - assertion checks if post_sat commands work
 *****************************************************************************/
static void test_post_sat( void )
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

    int test_value1 = 031;
    int test_value2 = 256;
    
    uint16_t saturation;
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

    // initialize provideo protocol
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_cproc_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current saturation configuration
    res = ctrl_protocol_get_post_sat( protocol, channel, &saturation );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_post_sat( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_post_sat( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_post_sat( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_post_sat( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_post_sat( protocol, channel, saturation );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_post_hue - assertion checks if post_hue commands work
 *****************************************************************************/
static void test_post_hue( void )
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

    int16_t hue;
    int16_t v;

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

    res = provideo_protocol_cproc_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current hue configuration
    res = ctrl_protocol_get_post_hue( protocol, channel, &hue );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_post_hue( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_post_hue( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_post_hue( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_post_hue(  protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_post_hue( protocol, channel, hue );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef provideo_protocol_cproc_tests( void )
{
	EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "post_bright"          , test_post_bright ),
        new_TestFixture( "post_cont"            , test_post_cont ),
        new_TestFixture( "post_sat"             , test_post_sat ),
        new_TestFixture( "post_hue"             , test_post_hue ),
	};
	EMB_UNIT_TESTCALLER( provideo_protocol_cproc_test, "PROVIDEO-PROTOCOL-CPROC", setup, teardown, fixtures );

	return ( (TestRef)&provideo_protocol_cproc_test );
}


