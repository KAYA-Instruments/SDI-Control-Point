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
 * @file    provideo_protocol_cam_tests.c
 *
 * @brief   Test implementation of provideo protocol for cam module
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
#include <ctrl_protocol/ctrl_protocol_cam.h>

// specific control channel implementation
#include <rs232/ctrl_channel_rs232.h>

// specific control protocol implementation
#include <provideo_protocol/provideo_protocol.h>

/******************************************************************************
 * global variables
 *****************************************************************************/
extern uint8_t g_com_port;

ctrl_protocol_cam_info_t cam_info;

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
 * test_cam_info - assertion checks if cam_info commands work
 *****************************************************************************/
static void test_cam_info( void )
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

    res = provideo_protocol_cam_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_get_cam_info( protocol, channel, sizeof(cam_info), (uint8_t *)&cam_info );
    TEST_ASSERT_EQUAL_INT( 0, res );

    printf("\n");
    printf("min gain          = %d\n", cam_info.min_gain );
    printf("max gain          = %d\n", cam_info.max_gain );
    printf("min exposure time = %d\n", cam_info.min_exposure_time );
    printf("min exposure time = %d\n", cam_info.max_exposure_time  );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_cam_gain - assertion checks if cam_gain commands work
 *****************************************************************************/
static void test_cam_gain( void )
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

    uint16_t test_value1 = 35u;
    uint16_t test_value2 = 513u;
    
    uint16_t gain;
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

    res = provideo_protocol_cam_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current camera gain configuration
    res = ctrl_protocol_get_cam_gain( protocol, channel, &gain );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_cam_gain( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_cam_gain( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_cam_gain( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_cam_gain( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_cam_gain( protocol, channel, gain );
    TEST_ASSERT_EQUAL_INT( 0, res );    

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_cam_exposure - assertion checks if cam_exposure commands work
 *****************************************************************************/
static void test_cam_exposure( void )
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

    uint32_t test_value1 = 3u;
    uint32_t test_value2 = 5025u;

    uint32_t time;
    uint32_t v;

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

    res = provideo_protocol_cam_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current camera exposure configuration
    res = ctrl_protocol_get_cam_exposure( protocol, channel, &time );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_cam_exposure( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_cam_exposure( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_cam_exposure( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_cam_exposure( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_cam_exposure( protocol, channel, time );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef provideo_protocol_cam_tests( void )
{
    EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "cam_info"     , test_cam_info ),
        new_TestFixture( "cam_gain"     , test_cam_gain ),
        new_TestFixture( "cam_exposure" , test_cam_exposure ),
    };
    EMB_UNIT_TESTCALLER( provideo_protocol_cam_test, "PROVIDEO-PROTOCOL-CAM", setup, teardown, fixtures );

    return ( (TestRef)&provideo_protocol_cam_test );
}

