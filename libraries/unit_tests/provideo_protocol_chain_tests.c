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
 * @file    provideo_protocol_chain_tests.c
 *
 * @brief   Test implementation of provideo protocol for chain
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
#include <ctrl_protocol/ctrl_protocol_chain.h>

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
 * test_out - assertion checks if out command work
 *****************************************************************************/
static void test_out( void )
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
    int test_value3 = 3;

    uint8_t chain;
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

    res = provideo_protocol_chain_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current output chain configuration
    res = ctrl_protocol_get_output_chain( protocol, channel, &chain );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_output_chain( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_output_chain( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_output_chain( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_output_chain( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_output_chain( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_output_chain( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    // restore pre-test configuration
    res = ctrl_protocol_set_output_chain( protocol, channel, chain );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_video_mode - assertion checks if video_mode command work
 *****************************************************************************/
static void test_video_mode( void )
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

    int test_value1[(VIDEO_MODE_MAX-VIDEO_MODE_FIRST)] =
    { 
        VIDEO_MODE_720p60    , VIDEO_MODE_720p50    , VIDEO_MODE_720p59_94,
        VIDEO_MODE_1080p30   , VIDEO_MODE_1080p25   , VIDEO_MODE_1080p24  ,
        VIDEO_MODE_1080p23_98, VIDEO_MODE_1080p29_97, VIDEO_MODE_1080p50  ,
        VIDEO_MODE_1080p60   , VIDEO_MODE_1080i60   , VIDEO_MODE_1080i50  ,
        VIDEO_MODE_1080i59_94
    };

    int test_value2[2]  = { 0, 14 };

    uint8_t mode;
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

    res = provideo_protocol_chain_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current video mode configuration
    res = ctrl_protocol_get_video_mode( protocol, channel, &mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    /* TEST CASE FUNCTIONAL */
    printf( "\n" );
    for ( i = 0u; i<ARRAY_SIZE(test_value1); i++ )
    {
        res = ctrl_protocol_set_video_mode( protocol, channel, test_value1[i] );
        printf( "video mode = %d\n", test_value1[i] );
        TEST_ASSERT_EQUAL_INT( 0, res );
        res = ctrl_protocol_get_video_mode( protocol, channel, &v );
        // first check value, than we know the failing video-mode = value + 1
        TEST_ASSERT_EQUAL_INT( test_value1[i], v );
        TEST_ASSERT_EQUAL_INT( 0, res );
    
        /* delay 5s, because system blocks fast video_mode switching */
        sleep( 5 );
    }
    
    // restore pre-test configuration
    res = ctrl_protocol_set_video_mode( protocol, channel, mode );
    TEST_ASSERT_EQUAL_INT( 0, res );    

    /* delay 5s, because system blocks fast video_mode switching */
    sleep( 5 );

    /* TEST CASE ANTI-FUNCTIONAL */
    for ( i = 0u; i<ARRAY_SIZE(test_value2); i++ )
    {
        res = ctrl_protocol_set_video_mode( protocol, channel, test_value2[i] );
        TEST_ASSERT_EQUAL_INT( -EINVAL, res );
        res = ctrl_protocol_get_video_mode( protocol, channel, &v );
        // first check value, than we know the failing video-mode = value + 1
        TEST_ASSERT_EQUAL_INT( mode, v );
        TEST_ASSERT_EQUAL_INT( 0, res );

        /* delay 5s, because system blocks fast video_mode switching */
        sleep( 5 );
    }

    // restore pre-test configuration
    res = ctrl_protocol_set_video_mode( protocol, channel, mode );
    TEST_ASSERT_EQUAL_INT( 0, res );    

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_raw - assertion checks if raw command work
 *****************************************************************************/
static void test_raw( void )
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
    int test_value4 = 3;
    int test_value5 = 4;
    int test_value6 = -1;

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

    res = provideo_protocol_chain_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current output chain configuration
    res = ctrl_protocol_get_flip_mode( protocol, channel, &flag );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_flip_mode( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_flip_mode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_flip_mode( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_flip_mode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    res = ctrl_protocol_set_flip_mode( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_flip_mode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );

    res = ctrl_protocol_set_flip_mode( protocol, channel, test_value4 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_flip_mode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value4, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_flip_mode( protocol, channel, test_value5 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_flip_mode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value4, v );

    res = ctrl_protocol_set_flip_mode( protocol, channel, test_value6 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_flip_mode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value4, v );
     
    // restore pre-test configuration
    res = ctrl_protocol_set_flip_mode( protocol, channel, flag );
    TEST_ASSERT_EQUAL_INT( 0, res );    

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_genlock - assertion checks if raw command work
 *****************************************************************************/
static void test_genlock( void )
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
    //int test_value1 = 1;
    int test_value2 = 2;

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

    res = provideo_protocol_chain_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current output chain configuration
    res = ctrl_protocol_get_genlock_mode( protocol, channel, &mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_genlock_mode( protocol, channel, test_value0 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_genlock_mode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value0, v );

#if 0   // this test needs an external sync source 
    /* delay 5s, because system blocks fast video_mode switching */
    sleep( 5 );

    res = ctrl_protocol_set_genlock_mode( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_genlock_mode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );
#endif    
    
    /* delay 5s, because system blocks fast video_mode switching */
    sleep( 5 );

    res = ctrl_protocol_set_genlock_mode( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_genlock_mode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );
    
    // TEST CASE ANTI-FUNCTIONAL
    
    /* delay 5s, because system blocks fast video_mode switching */
    sleep( 5 );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_genlock_mode( protocol, channel, mode );
    TEST_ASSERT_EQUAL_INT( 0, res );    

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_sdi_range - assertion checks if sdi_range commands work
 *****************************************************************************/
static void test_sdi_range( void )
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
    int test_value3 = 5;

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

    res = provideo_protocol_chain_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current sdi range configuration
    res = ctrl_protocol_get_sdi_range( protocol, channel, &flag );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_sdi_range( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_sdi_range( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_sdi_range( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_sdi_range( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_sdi_range( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_sdi_range( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_sdi_range( protocol, channel, flag );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_sdi_black - assertion checks if sdi_black commands work
 *****************************************************************************/
static void test_sdi_black( void )
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

    int8_t test_value0 =  -10;
    int8_t test_value1 =  +10;
    int8_t test_value2 =  -60;
    int8_t test_value3 =  +60;
    int8_t test_value4 = -100;
    int8_t test_value5 = +100;

    uint8_t flag;
    int8_t black;
    int8_t v;

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

    res = provideo_protocol_chain_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current sdi range and black configuration
    res = ctrl_protocol_get_sdi_range( protocol, channel, &flag );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_get_sdi_black( protocol, channel, &black );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_sdi_range( protocol, channel, SDI_RANGE_LEGAL );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    res = ctrl_protocol_set_sdi_black( protocol, channel, test_value0 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_sdi_black( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value0, v );

    res = ctrl_protocol_set_sdi_black( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_sdi_black( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_sdi_black( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_sdi_black( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    res = ctrl_protocol_set_sdi_black( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_sdi_black( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_sdi_black( protocol, channel, test_value4 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_sdi_black( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );

    res = ctrl_protocol_set_sdi_black( protocol, channel, test_value5 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_sdi_black( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_sdi_range( protocol, channel, flag );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_set_sdi_black( protocol, channel, black );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_sdi_white - assertion checks if sdi_white commands work
 *****************************************************************************/
static void test_sdi_white( void )
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

    int8_t test_value0 =  -10;
    int8_t test_value1 =  +10;
    int8_t test_value2 =  -80;
    int8_t test_value3 =  +79;
    int8_t test_value4 = -100;
    int8_t test_value5 = +100;

    uint8_t flag;
    int8_t white;
    int8_t v;

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

    res = provideo_protocol_chain_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current sdi range and white configuration
    res = ctrl_protocol_get_sdi_range( protocol, channel, &flag );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_get_sdi_white( protocol, channel, &white );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_sdi_range( protocol, channel, SDI_RANGE_LEGAL );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    res = ctrl_protocol_set_sdi_white( protocol, channel, test_value0 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_sdi_white( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value0, v );

    res = ctrl_protocol_set_sdi_white( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_sdi_white( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_sdi_white( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_sdi_white( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    res = ctrl_protocol_set_sdi_white( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_sdi_white( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_sdi_white( protocol, channel, test_value4 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_sdi_white( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );

    res = ctrl_protocol_set_sdi_white( protocol, channel, test_value5 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_sdi_white( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_sdi_range( protocol, channel, flag );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_set_sdi_white( protocol, channel, white );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef provideo_protocol_chain_tests( void )
{
	EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "out"          , test_out ),
        new_TestFixture( "video_mode"   , test_video_mode ),
        new_TestFixture( "raw"          , test_raw ),
        new_TestFixture( "genlock"      , test_genlock ),
        new_TestFixture( "sdi_range"    , test_sdi_range ),
        new_TestFixture( "sdi_black"    , test_sdi_black ),
        new_TestFixture( "sdi_white"    , test_sdi_white ),
	};
	EMB_UNIT_TESTCALLER( provideo_protocol_chain_test, "PROVIDEO-PROTOCOL-CHAIN", setup, teardown, fixtures );

	return ( (TestRef)&provideo_protocol_chain_test );
}


