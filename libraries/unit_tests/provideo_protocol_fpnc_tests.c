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
 * @file    provideo_protocol_fpnc_tests.c
 *
 * @brief   Test implementation of provideo protocol for fpnc
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
#include <ctrl_protocol/ctrl_protocol_fpnc.h>

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
 * test_fpnc - assertion checks if fpnc commands work
 *****************************************************************************/
static void test_fpnc( void )
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

    res = provideo_protocol_fpnc_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current fpnc configuration
    res = ctrl_protocol_get_fpnc_enable( protocol, channel, &mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_fpnc_enable( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_fpnc_enable( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_fpnc_enable( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_fpnc_enable( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_fpnc_enable( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_fpnc_enable( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_fpnc_enable( protocol, channel, mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_fpnc_inv_gains - assertion checks if fpnc_inv_gains commands work
 *****************************************************************************/
static void test_fpnc_inv_gains( void )
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
    int test_value3 = 0xFFFF;

    ctrl_protocol_fpnc_inv_gains_t  inv_gains;
    ctrl_protocol_fpnc_inv_gains_t  v;
    ctrl_protocol_fpnc_inv_gains_t  value;

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

    res = provideo_protocol_fpnc_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current fpnc configuration
    res = ctrl_protocol_get_fpnc_inv_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_inv_gains_t), (uint8_t *)&inv_gains );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    v.inv_gain_0 = test_value1;
    v.inv_gain_1 = test_value1;
    v.inv_gain_2 = test_value1;
    v.inv_gain_3 = test_value1;
    res = ctrl_protocol_set_fpnc_inv_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_inv_gains_t), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_fpnc_inv_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_inv_gains_t), (uint8_t *)&value );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, value.inv_gain_0 );
    TEST_ASSERT_EQUAL_INT( test_value1, value.inv_gain_1 );
    TEST_ASSERT_EQUAL_INT( test_value1, value.inv_gain_2 );
    TEST_ASSERT_EQUAL_INT( test_value1, value.inv_gain_3 );

    v.inv_gain_0 = test_value2;
    v.inv_gain_1 = test_value2;
    v.inv_gain_2 = test_value2;
    v.inv_gain_3 = test_value2;
    res = ctrl_protocol_set_fpnc_inv_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_inv_gains_t), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_fpnc_inv_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_inv_gains_t), (uint8_t *)&value );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, value.inv_gain_0 );
    TEST_ASSERT_EQUAL_INT( test_value2, value.inv_gain_1 );
    TEST_ASSERT_EQUAL_INT( test_value2, value.inv_gain_2 );
    TEST_ASSERT_EQUAL_INT( test_value2, value.inv_gain_2 );

    v.inv_gain_0 = test_value3;
    v.inv_gain_1 = test_value3;
    v.inv_gain_2 = test_value3;
    v.inv_gain_3 = test_value3;
    res = ctrl_protocol_set_fpnc_inv_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_inv_gains_t), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_fpnc_inv_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_inv_gains_t), (uint8_t *)&value );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, value.inv_gain_0 );
    TEST_ASSERT_EQUAL_INT( test_value3, value.inv_gain_1 );
    TEST_ASSERT_EQUAL_INT( test_value3, value.inv_gain_2 );
    TEST_ASSERT_EQUAL_INT( test_value3, value.inv_gain_2 );

    // TEST CASE ANTI-FUNCTIONAL
    
    // restore pre-test configuration
    res = ctrl_protocol_set_fpnc_inv_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_inv_gains_t), (uint8_t *)&inv_gains );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_fpnc_gains - assertion checks if fpnc_gains commands work
 *****************************************************************************/
static void test_fpnc_gains( void )
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
    int test_value3 = 0xFFFF;

    ctrl_protocol_fpnc_gains_t  gains;
    ctrl_protocol_fpnc_gains_t  v;
    ctrl_protocol_fpnc_gains_t  value;

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

    res = provideo_protocol_fpnc_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current fpnc configuration
    res = ctrl_protocol_get_fpnc_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_gains_t), (uint8_t *)&gains );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    v.gain_0 = test_value1;
    v.gain_1 = test_value1;
    v.gain_2 = test_value1;
    v.gain_3 = test_value1;
    res = ctrl_protocol_set_fpnc_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_gains_t), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_fpnc_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_gains_t), (uint8_t *)&value );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, value.gain_0 );
    TEST_ASSERT_EQUAL_INT( test_value1, value.gain_1 );
    TEST_ASSERT_EQUAL_INT( test_value1, value.gain_2 );
    TEST_ASSERT_EQUAL_INT( test_value1, value.gain_3 );

    v.gain_0 = test_value2;
    v.gain_1 = test_value2;
    v.gain_2 = test_value2;
    v.gain_3 = test_value2;
    res = ctrl_protocol_set_fpnc_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_gains_t), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_fpnc_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_gains_t), (uint8_t *)&value );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, value.gain_0 );
    TEST_ASSERT_EQUAL_INT( test_value2, value.gain_1 );
    TEST_ASSERT_EQUAL_INT( test_value2, value.gain_2 );
    TEST_ASSERT_EQUAL_INT( test_value2, value.gain_2 );

    v.gain_0 = test_value3;
    v.gain_1 = test_value3;
    v.gain_2 = test_value3;
    v.gain_3 = test_value3;
    res = ctrl_protocol_set_fpnc_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_gains_t), (uint8_t *)&v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_fpnc_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_gains_t), (uint8_t *)&value );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, value.gain_0 );
    TEST_ASSERT_EQUAL_INT( test_value3, value.gain_1 );
    TEST_ASSERT_EQUAL_INT( test_value3, value.gain_2 );
    TEST_ASSERT_EQUAL_INT( test_value3, value.gain_2 );

    // TEST CASE ANTI-FUNCTIONAL
    
    // restore pre-test configuration
    res = ctrl_protocol_set_fpnc_gains( protocol, channel,
            sizeof(ctrl_protocol_fpnc_gains_t), (uint8_t *)&gains );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_fpnc_set_values - assertion checks if fpnc_set_values commands work
 *****************************************************************************/
static void test_fpnc_set_values( void )
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

    uint32_t testvalue1 = 0x123456u;
    uint32_t testvalue2 = 0x654321u;
    uint32_t testvalue3 = 0x789ABCu;
    uint32_t testvalue4 = 0xCBA987u;

    uint32_t value[16];
    uint32_t v[16];

    ctrl_protocol_fpnc_data_t data;
    
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

    res = provideo_protocol_fpnc_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current fpnc configuration
    memset( value, 0, sizeof(value) );

    data.page   = 0u;
    data.column = 0u;
    data.offset = 0x0u;
    res = ctrl_protocol_get_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    value[ 0] = data.v0;
    value[ 1] = data.v1;
    value[ 2] = data.v2;
    value[ 3] = data.v3;
    
    data.offset = 0x4u;
    res = ctrl_protocol_get_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    value[ 4] = data.v0;
    value[ 5] = data.v1;
    value[ 6] = data.v2;
    value[ 7] = data.v3;

    data.offset = 0x8u;
    res = ctrl_protocol_get_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    value[ 8] = data.v0;
    value[ 9] = data.v1;
    value[10] = data.v2;
    value[11] = data.v3;
    
    data.offset = 0xCu;
    res = ctrl_protocol_get_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    value[12] = data.v0;
    value[13] = data.v1;
    value[14] = data.v2;
    value[15] = data.v3;
    
    // TEST CASE FUNCTIONAL

    // I.1) write correction values
    data.page   = 0u;
    data.column = 0u;
    data.offset = 0x0u;
    data.v0     = testvalue1;
    data.v1     = testvalue2;
    data.v2     = testvalue3;
    data.v3     = testvalue4;
    res = ctrl_protocol_set_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    data.offset = 0x4u;
    res = ctrl_protocol_set_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );

    data.offset = 0x8u;
    res = ctrl_protocol_set_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    data.offset = 0xCu;
    res = ctrl_protocol_set_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // I.2) read back correction values
    data.page   = 0u;
    data.column = 0u;
    data.offset = 0x0u;
    res = ctrl_protocol_get_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    v[ 0] = data.v0;
    v[ 1] = data.v1;
    v[ 2] = data.v2;
    v[ 3] = data.v3;
    
    data.offset = 0x4u;
    res = ctrl_protocol_get_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    v[ 4] = data.v0;
    v[ 5] = data.v1;
    v[ 6] = data.v2;
    v[ 7] = data.v3;

    data.offset = 0x8u;
    res = ctrl_protocol_get_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    v[ 8] = data.v0;
    v[ 9] = data.v1;
    v[10] = data.v2;
    v[11] = data.v3;
    
    data.offset = 0xCu;
    res = ctrl_protocol_get_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    v[12] = data.v0;
    v[13] = data.v1;
    v[14] = data.v2;
    v[15] = data.v3;

    // I.3) compare values
    TEST_ASSERT_EQUAL_INT( testvalue1, v[ 0] );
    TEST_ASSERT_EQUAL_INT( testvalue2, v[ 1] );
    TEST_ASSERT_EQUAL_INT( testvalue3, v[ 2] );
    TEST_ASSERT_EQUAL_INT( testvalue4, v[ 3] );
    TEST_ASSERT_EQUAL_INT( testvalue1, v[ 4] );
    TEST_ASSERT_EQUAL_INT( testvalue2, v[ 5] );
    TEST_ASSERT_EQUAL_INT( testvalue3, v[ 6] );
    TEST_ASSERT_EQUAL_INT( testvalue4, v[ 7] );
    TEST_ASSERT_EQUAL_INT( testvalue1, v[ 8] );
    TEST_ASSERT_EQUAL_INT( testvalue2, v[ 9] );
    TEST_ASSERT_EQUAL_INT( testvalue3, v[10] );
    TEST_ASSERT_EQUAL_INT( testvalue4, v[11] );
    TEST_ASSERT_EQUAL_INT( testvalue1, v[12] );
    TEST_ASSERT_EQUAL_INT( testvalue2, v[13] );
    TEST_ASSERT_EQUAL_INT( testvalue3, v[14] );
    TEST_ASSERT_EQUAL_INT( testvalue4, v[15] );


    // II.1) write correction values
    data.page   = 0u;
    data.column = 0u;
    data.offset = 0x0u;
    data.v0     = testvalue1;
    data.v1     = testvalue1;
    data.v2     = testvalue1;
    data.v3     = testvalue1;
    res = ctrl_protocol_set_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    data.offset = 0x4u;
    data.v0     = testvalue2;
    data.v1     = testvalue2;
    data.v2     = testvalue2;
    data.v3     = testvalue2;
    res = ctrl_protocol_set_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );

    data.offset = 0x8u;
    data.v0     = testvalue3;
    data.v1     = testvalue3;
    data.v2     = testvalue3;
    data.v3     = testvalue3;
    res = ctrl_protocol_set_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    data.offset = 0xCu;
    data.v0     = testvalue4;
    data.v1     = testvalue4;
    data.v2     = testvalue4;
    data.v3     = testvalue4;
    res = ctrl_protocol_set_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // II.2) read back correction values
    data.page   = 0u;
    data.column = 0u;
    data.offset = 0x0u;
    res = ctrl_protocol_get_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    v[ 0] = data.v0;
    v[ 1] = data.v1;
    v[ 2] = data.v2;
    v[ 3] = data.v3;
    
    data.offset = 0x4u;
    res = ctrl_protocol_get_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    v[ 4] = data.v0;
    v[ 5] = data.v1;
    v[ 6] = data.v2;
    v[ 7] = data.v3;

    data.offset = 0x8u;
    res = ctrl_protocol_get_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    v[ 8] = data.v0;
    v[ 9] = data.v1;
    v[10] = data.v2;
    v[11] = data.v3;
    
    data.offset = 0xCu;
    res = ctrl_protocol_get_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    v[12] = data.v0;
    v[13] = data.v1;
    v[14] = data.v2;
    v[15] = data.v3;

    // II.3) compare values
    TEST_ASSERT_EQUAL_INT( testvalue1, v[ 0] );
    TEST_ASSERT_EQUAL_INT( testvalue1, v[ 1] );
    TEST_ASSERT_EQUAL_INT( testvalue1, v[ 2] );
    TEST_ASSERT_EQUAL_INT( testvalue1, v[ 3] );
    TEST_ASSERT_EQUAL_INT( testvalue2, v[ 4] );
    TEST_ASSERT_EQUAL_INT( testvalue2, v[ 5] );
    TEST_ASSERT_EQUAL_INT( testvalue2, v[ 6] );
    TEST_ASSERT_EQUAL_INT( testvalue2, v[ 7] );
    TEST_ASSERT_EQUAL_INT( testvalue3, v[ 8] );
    TEST_ASSERT_EQUAL_INT( testvalue3, v[ 9] );
    TEST_ASSERT_EQUAL_INT( testvalue3, v[10] );
    TEST_ASSERT_EQUAL_INT( testvalue3, v[11] );
    TEST_ASSERT_EQUAL_INT( testvalue4, v[12] );
    TEST_ASSERT_EQUAL_INT( testvalue4, v[13] );
    TEST_ASSERT_EQUAL_INT( testvalue4, v[14] );
    TEST_ASSERT_EQUAL_INT( testvalue4, v[15] );

    // TEST CASE ANTI-FUNCTIONAL

    // restore pre-test configuration
    data.page   = 0u;
    data.column = 0u;
    data.offset = 0x0u;
    data.v0     = value[ 0];
    data.v1     = value[ 1];
    data.v2     = value[ 2];
    data.v3     = value[ 3];
    res = ctrl_protocol_set_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    data.offset = 0x4u;
    data.v0     = value[ 4];
    data.v1     = value[ 5];
    data.v2     = value[ 6];
    data.v3     = value[ 7];
    res = ctrl_protocol_set_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );

    data.offset = 0x8u;
    data.v0     = value[ 8];
    data.v1     = value[ 9];
    data.v2     = value[10];
    data.v3     = value[11];
    res = ctrl_protocol_set_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    data.offset = 0xCu;
    data.v0     = value[12];
    data.v1     = value[13];
    data.v2     = value[14];
    data.v3     = value[15];
    res = ctrl_protocol_set_fpnc_correction_data( protocol, channel, sizeof(ctrl_protocol_fpnc_data_t), (uint8_t *)&data );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef provideo_protocol_fpnc_tests( void )
{
    EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "fpnc"             , test_fpnc ),
        new_TestFixture( "fpnc_inv_gains"   , test_fpnc_inv_gains ),
        new_TestFixture( "fpnc_gains"       , test_fpnc_gains ),
        new_TestFixture( "fpnc_set_values"  , test_fpnc_set_values ),
#if 0            
        new_TestFixture( "fpnc_dump"        , test_fpnc_dump ),
#endif        
    };
    EMB_UNIT_TESTCALLER( provideo_protocol_fpnc_test, "FPNC_PROCESSING", setup, teardown, fixtures );

    return ( (TestRef)&provideo_protocol_fpnc_test );
}

