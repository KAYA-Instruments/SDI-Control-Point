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
 * @file    provideo_protocol_fb_tests.c
 *
 * @brief   Test implementation of provideo protocol for frame buffer
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
#include <ctrl_protocol/ctrl_protocol_playback.h>

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
 * test_buffer_size - assertion checks if buffer_size command works
 *****************************************************************************/
static void test_buffer_size( void )
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

    ctrl_protocol_fb_t      fb_get;
    ctrl_protocol_fb_size_t fb_get_size;

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

    res = provideo_protocol_playback_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    fb_get.id    = 0u;
    fb_get.first = 0u;
    fb_get.last  = 0u;
    res = ctrl_protocol_get_buffer( protocol, channel, sizeof(fb_get), (uint8_t *)&fb_get );
    TEST_ASSERT_EQUAL_INT( 0, res );

    fb_get_size.id   = 0u;
    fb_get_size.size = 0u;
    res = ctrl_protocol_get_buffer_size( protocol, channel, sizeof(fb_get_size), (uint8_t *)&fb_get_size );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( (fb_get.last - fb_get.first + 1) , fb_get_size.size );

    fb_get.id    = 1u;
    fb_get.first = 0u;
    fb_get.last  = 0u;
    res = ctrl_protocol_get_buffer( protocol, channel, sizeof(fb_get), (uint8_t *)&fb_get );
    TEST_ASSERT_EQUAL_INT( 0, res );

    fb_get_size.id   = 1u;
    fb_get_size.size = 0u;
    res = ctrl_protocol_get_buffer_size( protocol, channel, sizeof(fb_get_size), (uint8_t *)&fb_get_size );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( (fb_get.last - fb_get.first + 1) , fb_get_size.size );

    fb_get.id    = 2u;
    fb_get.first = 0u;
    fb_get.last  = 0u;
    res = ctrl_protocol_get_buffer( protocol, channel, sizeof(fb_get), (uint8_t *)&fb_get );
    TEST_ASSERT_EQUAL_INT( 0, res );

    fb_get_size.id   = 2u;
    fb_get_size.size = 0u;
    res = ctrl_protocol_get_buffer_size( protocol, channel, sizeof(fb_get_size), (uint8_t *)&fb_get_size );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( (fb_get.last - fb_get.first + 1) , fb_get_size.size );

    fb_get.id    = 3u;
    fb_get.first = 0u;
    fb_get.last  = 0u;
    res = ctrl_protocol_get_buffer( protocol, channel, sizeof(fb_get), (uint8_t *)&fb_get );
    TEST_ASSERT_EQUAL_INT( 0, res );

    fb_get_size.id   = 3u;
    fb_get_size.size = 0u;
    res = ctrl_protocol_get_buffer_size( protocol, channel, sizeof(fb_get_size), (uint8_t *)&fb_get_size );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( (fb_get.last - fb_get.first + 1) , fb_get_size.size );

    // restore pre-test configuration
    // none, nothing changed in this test-case

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_buffer_default - assertion checks if buffer_default command works
 *****************************************************************************/
static void test_buffer_default( void )
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

    uint8_t test_value1 = 1u;
    uint8_t test_value2 = 2u;
    uint8_t test_value3 = 3u;
    uint8_t test_value4 = 4u;

    uint8_t test_value5 = 5u;
    uint8_t test_value6 = 0u;
    
    uint32_t max;
    uint32_t step;
    uint8_t  id;

    ctrl_protocol_fb_t      fb[4];
    ctrl_protocol_fb_size_t fb_size[4];
    
    ctrl_protocol_fb_t      fb_get;
    ctrl_protocol_fb_size_t fb_get_size;
    
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

    res = provideo_protocol_playback_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current fb configuration
    printf("\n");
    for ( id = 0u; id < ARRAY_SIZE(fb); id++ )
    {
        fb[id].id    = id;
        fb[id].first = 0u;
        fb[id].last  = 0u;
        res = ctrl_protocol_get_buffer( protocol, channel, sizeof(fb[id]), (uint8_t *)&fb[id] );
        TEST_ASSERT_EQUAL_INT( 0, res );

        fb_size[id].id   = id;
        fb_size[id].size = 0u;
        res = ctrl_protocol_get_buffer_size( protocol, channel, sizeof(fb_size[id]), (uint8_t *)&fb_size[id] );
        TEST_ASSERT_EQUAL_INT( 0, res );

        printf("buffer%1d: %3d %3d (size=%d)\n", fb[id].id, fb[id].first, fb[id].last, fb_size[id].size );
    }

    res = ctrl_protocol_get_max_frames( protocol, channel, &max );
    TEST_ASSERT_EQUAL_INT( 0, res );

    printf( "max = %d\n", max );
    
    // TEST CASE FUNCTIONAL

    /* test_value1 */
    res = ctrl_protocol_set_buffer_default( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );

    for ( id=0u; id<ARRAY_SIZE(fb); id++ )
    {
        fb_get.id    = id;
        fb_get.first = 0u;
        fb_get.last  = 0u;
        
        res = ctrl_protocol_get_buffer( protocol, channel, sizeof(fb_get), (uint8_t *)&fb_get );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( 0, fb_get.first );
        TEST_ASSERT_EQUAL_INT( max, fb_get.last );
    
        fb_get_size.id   = id;
        fb_get_size.size = 0u;
        res = ctrl_protocol_get_buffer_size( protocol, channel, sizeof(fb_get_size), (uint8_t *)&fb_get_size );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( (uint32_t)(max + 1), fb_get_size.size );
    }

    /* test_value2 */
    res = ctrl_protocol_set_buffer_default( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );

    step = (max + 1)  / test_value2;
    for ( id=0u; id<test_value2; id++ )
    {
        fb_get.id    = id;
        fb_get.first = 0u;
        fb_get.last  = 0u;
        
        res = ctrl_protocol_get_buffer( protocol, channel, sizeof(fb_get), (uint8_t *)&fb_get );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( (id * step), fb_get.first );
        TEST_ASSERT_EQUAL_INT( (((id+1)*step) - 1u), fb_get.last );
    
        fb_get_size.id   = id;
        fb_get_size.size = 0u;
        res = ctrl_protocol_get_buffer_size( protocol, channel, sizeof(fb_get_size), (uint8_t *)&fb_get_size );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( step, fb_get_size.size );
    }

    for ( id=test_value2; id<no; id++ )
    {
        fb_get.id    = id;
        fb_get.first = 0u;
        fb_get.last  = 0u;
        
        res = ctrl_protocol_get_buffer( protocol, channel, sizeof(fb_get), (uint8_t *)&fb_get );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( 0, fb_get.first );
        TEST_ASSERT_EQUAL_INT( max, fb_get.last );
    
        fb_get_size.id   = id;
        fb_get_size.size = 0u;
        res = ctrl_protocol_get_buffer_size( protocol, channel, sizeof(fb_get_size), (uint8_t *)&fb_get_size );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( (uint32_t)(max + 1), fb_get_size.size );
    }

    /* test_value3 */
    res = ctrl_protocol_set_buffer_default( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( 0, res );

    step = (max + 1)  / test_value3;
    for ( id=0u; id<test_value3; id++ )
    {
        fb_get.id    = id;
        fb_get.first = 0u;
        fb_get.last  = 0u;
        
        res = ctrl_protocol_get_buffer( protocol, channel, sizeof(fb_get), (uint8_t *)&fb_get );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( (id * step), fb_get.first );
        TEST_ASSERT_EQUAL_INT( (((id+1)*step) - 1u), fb_get.last );
    
        fb_get_size.id   = id;
        fb_get_size.size = 0u;
        res = ctrl_protocol_get_buffer_size( protocol, channel, sizeof(fb_get_size), (uint8_t *)&fb_get_size );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( step, fb_get_size.size );
    }

    for ( id=test_value3; id<no; id++ )
    {
        fb_get.id    = id;
        fb_get.first = 0u;
        fb_get.last  = 0u;
        
        res = ctrl_protocol_get_buffer( protocol, channel, sizeof(fb_get), (uint8_t *)&fb_get );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( 0, fb_get.first );
        TEST_ASSERT_EQUAL_INT( max, fb_get.last );
    
        fb_get_size.id   = id;
        fb_get_size.size = 0u;
        res = ctrl_protocol_get_buffer_size( protocol, channel, sizeof(fb_get_size), (uint8_t *)&fb_get_size );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( (uint32_t)(max + 1), fb_get_size.size );
    }

    /* test_value4 */
    res = ctrl_protocol_set_buffer_default( protocol, channel, test_value4 );
    TEST_ASSERT_EQUAL_INT( 0, res );

    step = (max + 1)  / test_value4;
    for ( id=0u; id<test_value4; id++ )
    {
        fb_get.id    = id;
        fb_get.first = 0u;
        fb_get.last  = 0u;
        
        res = ctrl_protocol_get_buffer( protocol, channel, sizeof(fb_get), (uint8_t *)&fb_get );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( (id * step), fb_get.first );
        TEST_ASSERT_EQUAL_INT( (((id+1)*step) - 1u), fb_get.last );
    
        fb_get_size.id   = id;
        fb_get_size.size = 0u;
        res = ctrl_protocol_get_buffer_size( protocol, channel, sizeof(fb_get_size), (uint8_t *)&fb_get_size );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( step, fb_get_size.size );
    }

    // TEST CASE ANTI-FUNCTIONAL

    /* test_value5 */
    res = ctrl_protocol_set_buffer_default( protocol, channel, test_value5 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );

    step = (max + 1)  / test_value4;
    for ( id=0u; id<test_value4; id++ )
    {
        fb_get.id    = id;
        fb_get.first = 0u;
        fb_get.last  = 0u;
        
        res = ctrl_protocol_get_buffer( protocol, channel, sizeof(fb_get), (uint8_t *)&fb_get );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( (id * step), fb_get.first );
        TEST_ASSERT_EQUAL_INT( (((id+1)*step) - 1u), fb_get.last );
    
        fb_get_size.id   = id;
        fb_get_size.size = 0u;
        res = ctrl_protocol_get_buffer_size( protocol, channel, sizeof(fb_get_size), (uint8_t *)&fb_get_size );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( step, fb_get_size.size );
    }

    /* test_value6 */
    res = ctrl_protocol_set_buffer_default( protocol, channel, test_value6 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );

    step = (max + 1)  / test_value4;
    for ( id=0u; id<test_value4; id++ )
    {
        fb_get.id    = id;
        fb_get.first = 0u;
        fb_get.last  = 0u;
        
        res = ctrl_protocol_get_buffer( protocol, channel, sizeof(fb_get), (uint8_t *)&fb_get );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( (id * step), fb_get.first );
        TEST_ASSERT_EQUAL_INT( (((id+1)*step) - 1u), fb_get.last );
    
        fb_get_size.id   = id;
        fb_get_size.size = 0u;
        res = ctrl_protocol_get_buffer_size( protocol, channel, sizeof(fb_get_size), (uint8_t *)&fb_get_size );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( step, fb_get_size.size );
    }

    // restore pre-test configuration
    for ( id = 0u; id < ARRAY_SIZE(fb); id++ )
    {
        res = ctrl_protocol_set_buffer( protocol, channel, sizeof(fb[id]), (uint8_t *)&fb[id] );
        TEST_ASSERT_EQUAL_INT( 0, res );
    }

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_play_mode - assertion checks if play_mode command work
 *****************************************************************************/
static void test_play_mode( void )
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

    int test_value1[16] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 };
    int test_value2[2] = { 0, 17 };

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

    res = provideo_protocol_playback_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current play mode
    res = ctrl_protocol_get_play_mode( protocol, channel, &mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    for ( i = 0u; i<ARRAY_SIZE(test_value1); i++ )
    {
        res = ctrl_protocol_set_play_mode( protocol, channel, test_value1[i] );
        TEST_ASSERT_EQUAL_INT( 0, res );
        res = ctrl_protocol_get_play_mode( protocol, channel, &v );
        // first check value, than we know that mode = value + 1 failed
        TEST_ASSERT_EQUAL_INT( test_value1[i], v );
        TEST_ASSERT_EQUAL_INT( 0, res );
    }

    res = ctrl_protocol_set_play_mode( protocol, channel, mode );
    TEST_ASSERT_EQUAL_INT( 0, res );    

    // TEST CASE ANTI-FUNCTIONAL
    for ( i = 0U; i<ARRAY_SIZE(test_value2); i++ )
    {
        res = ctrl_protocol_set_play_mode( protocol, channel, test_value2[i] );
        TEST_ASSERT_EQUAL_INT( -EINVAL, res );
        res = ctrl_protocol_get_play_mode( protocol, channel, &v );
        TEST_ASSERT_EQUAL_INT( 0, res );
        TEST_ASSERT_EQUAL_INT( mode, v );
    }
    
    // restore pre-test configuration
    res = ctrl_protocol_set_play_mode( protocol, channel, mode );
    TEST_ASSERT_EQUAL_INT( 0, res );    

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_rec_mode - assertion checks if rec_mode command work
 *****************************************************************************/
static void test_rec_mode( void )
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
    uint8_t fb;
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

    res = provideo_protocol_playback_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current record mode
    res = ctrl_protocol_get_record_mode( protocol, channel, &mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    /* to change recording mode
     * a running record needs to be stopped first */
    res = ctrl_protocol_get_record( protocol, channel, &fb );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_stop_record( protocol, channel );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_get_record( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_record_mode( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_record_mode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_record_mode( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_record_mode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_record_mode( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_record_mode( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_record_mode( protocol, channel, mode );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_set_record( protocol, channel, fb );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    res = ctrl_protocol_live( protocol, channel );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_rec_auto_live - assertion checks if rec_auto_live command work
 *****************************************************************************/
static void test_rec_auto_live( void )
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
    uint8_t fb;
    uint8_t fb_play;
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

    res = provideo_protocol_playback_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    // read auto-live and record configuration 
    res = ctrl_protocol_get_auto_live( protocol, channel, &mode );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_record( protocol, channel, &fb );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // TEST CASE FUNCTIONAL
    
    ///////////////////////////////
    // I.) disable auto-live
    res = ctrl_protocol_set_auto_live( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_auto_live( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    // switch record to buffer 2
    res = ctrl_protocol_set_record( protocol, channel, 2 );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // get current playback-buffer has to fail
    res = ctrl_protocol_get_play( protocol, channel, &fb_play );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );

    ///////////////////////////////
    // II.) enable auto-live
    res = ctrl_protocol_set_auto_live( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_auto_live( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    // switch record to buffer 2
    res = ctrl_protocol_set_record( protocol, channel, 2 );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // get current playback-buffer
    res = ctrl_protocol_get_play( protocol, channel, &fb_play );
    TEST_ASSERT_EQUAL_INT( 0, res );
   
    // playback buffer hast to be the same
    TEST_ASSERT( fb_play == 2 );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_auto_live( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_auto_live( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );
    
    // restore pre-test configuration
    res = ctrl_protocol_set_auto_live( protocol, channel, mode );
    TEST_ASSERT_EQUAL_INT( 0, res );    

    res = ctrl_protocol_set_record( protocol, channel, fb );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_live( protocol, channel );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}
/******************************************************************************
 * test_rec - assertion checks if rec commands work
 *****************************************************************************/
static void test_rec(void)
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

    uint8_t test_value0 = 0u;   // frame-buffer 0
    uint8_t test_value1 = 1u;   // frame-buffer 1
    uint8_t test_value2 = 2u;   // frame-buffer 2
    uint8_t test_value3 = 3u;   // frame-buffer 3

    uint8_t test_value4 = 4u;   // frame-buffer 4 (not available)
    uint8_t test_value5 = 5u;   // frame-buffer 5 (not available)

    uint8_t fb;
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

    res = provideo_protocol_playback_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current record buffer 
    res = ctrl_protocol_get_record( protocol, channel, &fb );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_stop_record( protocol, channel );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_get_record( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_record( protocol, channel, test_value0 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_record( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value0, v );

    res = ctrl_protocol_set_record( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_record( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_record( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_record( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    res = ctrl_protocol_set_record( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_record( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_record( protocol, channel, test_value4 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_record( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );

    res = ctrl_protocol_set_record( protocol, channel, test_value5 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_record( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );

    // restore pre-test configuration
    res = ctrl_protocol_set_record( protocol, channel, fb );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    res = ctrl_protocol_live( protocol, channel );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test_play - assertion checks if play commands work
 *****************************************************************************/
static void test_play(void)
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

    uint8_t test_value0 = 0u;   // frame-buffer 0
    uint8_t test_value1 = 1u;   // frame-buffer 1
    uint8_t test_value2 = 2u;   // frame-buffer 2
    uint8_t test_value3 = 3u;   // frame-buffer 3

    uint8_t test_value4 = 4u;   // frame-buffer 4 (not available)
    uint8_t test_value5 = 5u;   // frame-buffer 5 (not available)

    uint8_t fb;
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

    res = provideo_protocol_playback_init( protocol, NULL );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // read current record buffer 
    res = ctrl_protocol_get_record( protocol, channel, &fb );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_get_play( protocol, channel, &fb );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_stop_play( protocol, channel );
    TEST_ASSERT_EQUAL_INT( 0, res );

    res = ctrl_protocol_get_play( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );

    // TEST CASE FUNCTIONAL
    res = ctrl_protocol_set_play( protocol, channel, test_value0 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_play( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value0, v );

    res = ctrl_protocol_set_play( protocol, channel, test_value1 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_play( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value1, v );

    res = ctrl_protocol_set_play( protocol, channel, test_value2 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_play( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value2, v );

    res = ctrl_protocol_set_play( protocol, channel, test_value3 );
    TEST_ASSERT_EQUAL_INT( 0, res );
    res = ctrl_protocol_get_play( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );

    // TEST CASE ANTI-FUNCTIONAL
    res = ctrl_protocol_set_play( protocol, channel, test_value4 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_play( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );

    res = ctrl_protocol_set_play( protocol, channel, test_value5 );
    TEST_ASSERT_EQUAL_INT( -EINVAL, res );
    res = ctrl_protocol_get_play( protocol, channel, &v );
    TEST_ASSERT_EQUAL_INT( 0, res );
    TEST_ASSERT_EQUAL_INT( test_value3, v );

    // restore pre-test configuration
    res = ctrl_protocol_set_record( protocol, channel, fb );
    TEST_ASSERT_EQUAL_INT( 0, res );
    
    res = ctrl_protocol_live( protocol, channel );
    TEST_ASSERT_EQUAL_INT( 0, res );

    // close control channel
    res = ctrl_channel_close( channel );
    TEST_ASSERT_EQUAL_INT( 0, res );
}

/******************************************************************************
 * test group definition used in all_tests.c
 *****************************************************************************/
TestRef provideo_protocol_playback_tests( void )
{
    EMB_UNIT_TESTFIXTURES( fixtures )
    {
        new_TestFixture( "buffer_size"   , test_buffer_size ),
        new_TestFixture( "buffer_default", test_buffer_default ),
        new_TestFixture( "play_mode"     , test_play_mode ),
        new_TestFixture( "rec_mode"      , test_rec_mode ),
        new_TestFixture( "rec_auto_live" , test_rec_auto_live ),
        new_TestFixture( "rec"           , test_rec ),
        new_TestFixture( "play"          , test_play ),
    };
    EMB_UNIT_TESTCALLER( provideo_protocol_playback_test, "PLAYBACK", setup, teardown, fixtures );

    return ( (TestRef)&provideo_protocol_playback_test );
}

