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
#include <ctrl_protocol/ctrl_protocol_mcc.h>

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
 * test_mcc - assertion checks if mcc commands works
 *****************************************************************************/
static void test_mcc( void )
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

    res = provideo_protocol_mcc_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current mcc-enable configuration
    res = ctrl_protocol_get_mcc_enable( protocol, channel, &mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_mcc_enable( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_mcc_enable( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_mcc_enable( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_mcc_enable( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_mcc_enable( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_mcc_enable( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_mcc_enable( protocol, channel, mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_mcc_opmode - assertion checks if mcc_opmode command works
 *****************************************************************************/
static void test_mcc_opmode( void )
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

    int test_value1 = 0;    // opmode 0: 12 color phases
    int test_value2 = 1;    // opmode 1: 16 color phases
    int test_value3 = 2;    // opmode 2: 24 color phases
    int test_value4 = 3;    // opmode 3: 32 color phases

    int test_value5 = 4;
    int test_value6 = -1;

    uint8_t mode;
    uint8_t op_mode;
    uint8_t v;
    uint8_t no_seg;

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

    res = provideo_protocol_mcc_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current mcc-enable and mcc-opmode configuration
    res = ctrl_protocol_get_mcc_enable( protocol, channel, &mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_get_mcc_opmode( protocol, channel, &op_mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_mcc_opmode( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_mcc_opmode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );
    res = ctrl_protocol_get_mcc_no_phases( v, &no_seg );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( 12u, no_seg );

    res = ctrl_protocol_set_mcc_opmode( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_mcc_opmode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );
    res = ctrl_protocol_get_mcc_no_phases( v, &no_seg );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( 16u, no_seg );

    res = ctrl_protocol_set_mcc_opmode( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_mcc_opmode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );
    res = ctrl_protocol_get_mcc_no_phases( v, &no_seg );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( 24u, no_seg );

    res = ctrl_protocol_set_mcc_opmode( protocol, channel, test_value4 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_mcc_opmode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value4, v );
    res = ctrl_protocol_get_mcc_no_phases( v, &no_seg );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( 32u, no_seg );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_mcc_opmode( protocol, channel, test_value5 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_mcc_opmode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value4, v );

    res = ctrl_protocol_set_mcc_opmode( protocol, channel, test_value6 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_mcc_opmode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value4, v );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_mcc_opmode( protocol, channel, op_mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_set_mcc_enable( protocol, channel, mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_mcc_phases - assertion checks if mcc_set command works
 *****************************************************************************/
static void test_mcc_phases( void )
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

    int test_value_sat_0 = 0;
    int test_value_sat_1 = 32767;
    int test_value_sat_2 = 65535;
    
    int test_value_hue_0 = 0;
    int test_value_hue_1 = 32767;
    int test_value_hue_2 = -32768;

    uint8_t mode;
    uint8_t op_mode;
    uint8_t no_seg;

    unsigned i;

    ctrl_protocol_mcc_phase_t phases[MAX_MCC_NO_PHASES];

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

    res = provideo_protocol_mcc_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current mcc configuration
    res = ctrl_protocol_get_mcc_enable( protocol, channel, &mode );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_mcc_opmode( protocol, channel, &op_mode );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_mcc_no_phases( op_mode, &no_seg );
    TEST_ASSERT_EQUAL_INT( 0, res );
    for ( i=0u; i<no_seg; i++ )
    {
        phases[i].id = i;
        res = ctrl_protocol_get_mcc_phase( protocol, channel, sizeof(phases[i]), (uint8_t *)&phases[i] );
        TEST_ASSERT_EQUAL_INT( 0, res );
    }

    // TEST CASE FUNCTIONAL
    for ( i=0u; i<no_seg; i++ )
    {
        ctrl_protocol_mcc_phase_t phase = { .id = i, .saturation = test_value_sat_0, .hue = test_value_hue_0 };
        ctrl_protocol_mcc_phase_t value = { .id = i, .saturation = 0u, .hue = 0 };

        res = ctrl_protocol_set_mcc_phase( protocol, channel, sizeof(phase), (uint8_t *)&phase );
        TEST_ASSERT_EQUAL_INT( 0, res );
        
        res = ctrl_protocol_get_mcc_phase( protocol, channel, sizeof(value), (uint8_t *)&value );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value_sat_0, value.saturation );
        TEST_ASSERT_EQUAL_INT( test_value_hue_0, value.hue );
    }

    for ( i=0u; i<no_seg; i++ )
    {
        ctrl_protocol_mcc_phase_t phase = { .id = i, .saturation = test_value_sat_1, .hue = test_value_hue_1 };
        ctrl_protocol_mcc_phase_t value = { .id = i, .saturation = 0u, .hue = 0 };

        res = ctrl_protocol_set_mcc_phase( protocol, channel, sizeof(phase), (uint8_t *)&phase );
        TEST_ASSERT_EQUAL_INT( 0, res );

        res = ctrl_protocol_get_mcc_phase( protocol, channel, sizeof(value), (uint8_t *)&value );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value_sat_1, value.saturation );
        TEST_ASSERT_EQUAL_INT( test_value_hue_1, value.hue );
    }
    
    for ( i=0u; i<no_seg; i++ )
    {
        ctrl_protocol_mcc_phase_t phase = { .id = i, .saturation = test_value_sat_2, .hue = test_value_hue_2 };
        ctrl_protocol_mcc_phase_t value = { .id = i, .saturation = 0u, .hue = 0 };

        res = ctrl_protocol_set_mcc_phase( protocol, channel, sizeof(phase), (uint8_t *)&phase );
        TEST_ASSERT_EQUAL_INT( 0, res );

        res = ctrl_protocol_get_mcc_phase( protocol, channel, sizeof(value), (uint8_t *)&value );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value_sat_2, value.saturation );
        TEST_ASSERT_EQUAL_INT( test_value_hue_2, value.hue );
    }

    // TEST CASE ANTI-FUNCTIONAL
    
    // restore pre-test configuration
    res = ctrl_protocol_set_mcc_opmode( protocol, channel, op_mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    for ( i=0u; i<no_seg; i++ )
    {
        res = ctrl_protocol_set_mcc_phase( protocol, channel, sizeof(phases[i]), (uint8_t *)&phases[i] );
        TEST_ASSERT_EQUAL_INT( 0, res );
    }

    res = ctrl_protocol_set_mcc_enable( protocol, channel, mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef provideo_protocol_mcc_tests( void )
{
    EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "mcc"       , test_mcc ),
        new_TestFixture( "mcc_opmode", test_mcc_opmode ),
        new_TestFixture( "mcc_phases", test_mcc_phases ),
    };
    EMB_UNIT_TESTCALLER( provideo_protocol_mcc_test, "PROVIDEO-PROTOCOL-MCC", setup, teardown, fixtures );

    return ( (TestRef)&provideo_protocol_mcc_test );
}


