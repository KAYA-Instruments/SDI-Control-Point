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
#include <ctrl_protocol/ctrl_protocol_system.h>
#include <ctrl_protocol/ctrl_protocol_osd.h>

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
 * test_test_pattern - assertion checks if test_pattern commands work
 *****************************************************************************/
static void test_test_pattern( void )
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

    int test_value1 = 0;
    int test_value2 = 1;
    int test_value3 = 2;

    uint32_t hw_mask = 0u;

    uint8_t  mode;
    uint8_t  v;

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

    // initialize provideo protocol osd-instance
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_osd_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol sys-instance
    protocol_sys = (ctrl_protocol_handle_t)protocol_mem_sys;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol_sys, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_sys_init( protocol_sys, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_get_feature_mask_hw( protocol_sys, channel, &hw_mask );
    TEST_ASSERT_EQUAL_INT( 0, res );

    if ( IS_SYSTEM_FEATURE_TEST_PATTERN(hw_mask) )
    {
        // read current osd test-pattern configuration
        res = ctrl_protocol_get_test_pattern( protocol, channel, &mode );
        TEST_ASSERT_EQUAL_INT( 0, res );

        // TEST CASE FUNCTIONAL
        res = ctrl_protocol_set_test_pattern( protocol, channel, test_value1 );
        TEST_ASSERT_EQUAL_INT( 0, res );
        res = ctrl_protocol_get_test_pattern( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value1, v );

        res = ctrl_protocol_set_test_pattern( protocol, channel, test_value2 );
        TEST_ASSERT_EQUAL_INT( 0, res );
        res = ctrl_protocol_get_test_pattern( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value2, v );

        // TEST CASE ANTI-FUNCTIONAL
        res = ctrl_protocol_set_test_pattern( protocol, channel, test_value3 );
        TEST_ASSERT_EQUAL_INT( -EINVAL, res );
        res = ctrl_protocol_get_test_pattern( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value2, v );

        // restore pre-test configuration
        res = ctrl_protocol_set_test_pattern( protocol, channel, mode );
        TEST_ASSERT_EQUAL_INT( 0, res );
    }

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_center_marker - assertion checks if center_marker commands work
 *****************************************************************************/
static void test_center_marker( void )
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

    int test_value1 = 0;
    int test_value2 = 1;
    int test_value3 = 2;
    int test_value4 = 3;
    int test_value5 = 4;
    
    uint32_t hw_mask = 0u;

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

    // initialize provideo protocol osd-instance
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_osd_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol sys-instance
    protocol_sys = (ctrl_protocol_handle_t)protocol_mem_sys;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol_sys, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_sys_init( protocol_sys, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_get_feature_mask_hw( protocol_sys, channel, &hw_mask );
    TEST_ASSERT_EQUAL_INT( 0, res );

    if ( IS_SYSTEM_FEATURE_CENTER_MARKER(hw_mask) )
    {
        // read current osd center-marker configuration
        res = ctrl_protocol_get_center_marker( protocol, channel, &mode );
        TEST_ASSERT_EQUAL_INT( 0, res );

        // TEST CASE FUNCTIONAL
        res = ctrl_protocol_set_center_marker( protocol, channel, test_value1 );
        TEST_ASSERT_EQUAL_INT( 0, res );
        res = ctrl_protocol_get_center_marker( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value1, v );

        res = ctrl_protocol_set_center_marker( protocol, channel, test_value2 );
        TEST_ASSERT_EQUAL_INT( 0, res );
        res = ctrl_protocol_get_center_marker( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value2, v );

        res = ctrl_protocol_set_center_marker( protocol, channel, test_value3 );
        TEST_ASSERT_EQUAL_INT( 0, res );
        res = ctrl_protocol_get_center_marker( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value3, v );

        res = ctrl_protocol_set_center_marker( protocol, channel, test_value4 );
        TEST_ASSERT_EQUAL_INT( 0, res );
        res = ctrl_protocol_get_center_marker( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value4, v );

        // TEST CASE ANTI-FUNCTIONAL
        res = ctrl_protocol_set_center_marker( protocol, channel, test_value5 );
        TEST_ASSERT_EQUAL_INT( -EINVAL, res );
        res = ctrl_protocol_get_center_marker( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value4, v );
        
        // restore pre-test configuration
        res = ctrl_protocol_set_center_marker( protocol, channel, mode );
        TEST_ASSERT_EQUAL_INT( 0, res );
    }

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_zebra - assertion checks if zebra commands work
 *****************************************************************************/
static void test_zebra( void )
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

    int test_value_mode0 = 0;
    int test_value0 = 3;

    int test_value_mode2 = 2;
    int test_value2 = -5;

    uint32_t hw_mask = 0u;

    ctrl_protocol_osd_zebra_t mode;
    ctrl_protocol_osd_zebra_t v_set;
    ctrl_protocol_osd_zebra_t v;

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

    // initialize provideo protocol osd-instance
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_osd_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol sys-instance
    protocol_sys = (ctrl_protocol_handle_t)protocol_mem_sys;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol_sys, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_sys_init( protocol_sys, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_get_feature_mask_hw( protocol_sys, channel, &hw_mask );
    TEST_ASSERT_EQUAL_INT( 0, res );

    if ( IS_SYSTEM_FEATURE_ZEBRA(hw_mask) )
    {
        memset( &mode, 0, sizeof(mode) );

        // read current osd zebra configuration
        res = ctrl_protocol_get_zebra( protocol, channel, sizeof(mode), (uint8_t *)&mode );
        TEST_ASSERT_EQUAL_INT( 0, res );

        // TEST CASE FUNCTIONAL
        v_set.mode = test_value_mode0;
        v_set.min  = test_value0;
        v_set.max  = test_value0;

        res = ctrl_protocol_set_zebra( protocol, channel, sizeof(v_set), (uint8_t *)&v_set );
        TEST_ASSERT_EQUAL_INT( 0, res );
        
        memset( &v, 0, sizeof(v) );
        res = ctrl_protocol_get_zebra( protocol, channel, sizeof(v), (uint8_t *)&v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value_mode0, v.mode );
        TEST_ASSERT_EQUAL_INT( test_value0     , v.min  );
        TEST_ASSERT_EQUAL_INT( test_value0     , v.max  );

        // TEST CASE ANTI-FUNCTIONAL
        v_set.mode = test_value_mode2;
        v_set.min  = test_value2;
        v_set.max  = test_value2;

        res = ctrl_protocol_set_zebra( protocol, channel, sizeof(v_set), (uint8_t *)&v_set );
        TEST_ASSERT_EQUAL_INT( -EINVAL, res );

        memset( &v, 0, sizeof(v) );
        res = ctrl_protocol_get_zebra( protocol, channel, sizeof(v), (uint8_t *)&v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value_mode0, v.mode );
        TEST_ASSERT_EQUAL_INT( test_value0     , v.min  );
        TEST_ASSERT_EQUAL_INT( test_value0     , v.max  );
    
        // restore pre-test configuration
        res = ctrl_protocol_set_zebra( protocol, channel, sizeof(mode), (uint8_t *)&mode );
        TEST_ASSERT_EQUAL_INT( 0, res );
    }

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_logo - assertion checks if logo commands work
 *****************************************************************************/
static void test_logo( void )
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

    int test_value1 = 0;
    int test_value2 = 1;
    int test_value3 = 2;
    int test_value4 = 4;
    int test_value5 = -1;

    uint32_t hw_mask = 0u;

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

    // initialize provideo protocol osd-instance
    protocol = (ctrl_protocol_handle_t)protocol_mem;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_osd_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // initialize provideo protocol sys-instance
    protocol_sys = (ctrl_protocol_handle_t)protocol_mem_sys;
    TEST_ASSERT( ctrl_protocol_get_instance_size() > 0 );
    memset( protocol_sys, 0, ctrl_protocol_get_instance_size() );

    res = provideo_protocol_sys_init( protocol_sys, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_get_feature_mask_hw( protocol_sys, channel, &hw_mask );
    TEST_ASSERT_EQUAL_INT( 0, res );

    if ( IS_SYSTEM_FEATURE_LOGO(hw_mask) )
    {
        // read current osd logo configuration
        res = ctrl_protocol_get_logo( protocol, channel, &mode );
        TEST_ASSERT_EQUAL_INT( 0, res );

        // TEST CASE FUNCTIONAL
        res = ctrl_protocol_set_logo( protocol, channel, test_value1 );
        TEST_ASSERT_EQUAL_INT( 0, res );
        res = ctrl_protocol_get_logo( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value1, v );

        res = ctrl_protocol_set_logo( protocol, channel, test_value2 );
        TEST_ASSERT_EQUAL_INT( 0, res );
        res = ctrl_protocol_get_logo( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value2, v );

        res = ctrl_protocol_set_logo( protocol, channel, test_value3 );
        TEST_ASSERT_EQUAL_INT( 0, res );
        res = ctrl_protocol_get_logo( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value3, v );

        // TEST CASE ANTI-FUNCTIONAL
        res = ctrl_protocol_set_logo( protocol, channel, test_value4 );
        TEST_ASSERT_EQUAL_INT( -EINVAL, res );
        res = ctrl_protocol_get_logo( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value3, v );

        res = ctrl_protocol_set_logo( protocol, channel, test_value5 );
        TEST_ASSERT_EQUAL_INT( -EINVAL, res );
        res = ctrl_protocol_get_logo( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( test_value3, v );
         
        // restore pre-test configuration
        res = ctrl_protocol_set_logo( protocol, channel, mode );
        TEST_ASSERT_EQUAL_INT( 0, res );
    }

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef provideo_protocol_osd_tests( void )
{
    EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "test_pattern"  , test_test_pattern ),
        new_TestFixture( "center_marker" , test_center_marker ),
        new_TestFixture( "zebra"         , test_zebra ),
        new_TestFixture( "logo"          , test_logo ),
    };
    EMB_UNIT_TESTCALLER( provideo_protocol_osd_test, "OSD_PROCESSING", setup, teardown, fixtures );

    return ( (TestRef)&provideo_protocol_osd_test );
}


