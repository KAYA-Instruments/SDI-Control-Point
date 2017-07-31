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
 * @file    all_tests.c
 *
 * @brief   implementation of test-runner
 *
 *****************************************************************************/
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <embUnit/embUnit.h>

#include <rs232/ctrl_channel_rs232.h>

/******************************************************************************
 * test-references
 *****************************************************************************/

extern TestRef xyz_tests(void);                         /* implemented in xyz_tests.c */
extern TestRef conv_tests(void);                        /* implemented in conv_tests.c */
extern TestRef cubic_tests(void);                       /* implemented in cubic_tests.c */
extern TestRef rgb2ycbcr_tests(void);                   /* implemented in rgb2ycbcr_tests.c */
extern TestRef ctrl_channel_tests(void);                /* implemented in ctrl_channel_test.c */
extern TestRef ctrl_protocol_tests(void);               /* implemented in ctrl_protocol_test.c */
extern TestRef provideo_protocol_system_tests(void);    /* implemented in provideo_protocol_system_tests.c */
extern TestRef provideo_protocol_isp_tests(void);       /* implemented in provideo_protocol_isp_tests.c */
extern TestRef provideo_protocol_cproc_tests(void);     /* implemented in provideo_protocol_cproc_tests.c */
extern TestRef provideo_protocol_auto_tests(void);      /* implemented in provideo_protocol_auto_tests.c */
extern TestRef provideo_protocol_mcc_tests(void);       /* implemented in provideo_protocol_mcc_tests.c */
extern TestRef provideo_protocol_cam_tests(void);       /* implemented in provideo_protocol_cam_tests.c */
extern TestRef provideo_protocol_chain_tests(void);     /* implemented in provideo_protocol_chain_tests.c */
extern TestRef provideo_protocol_lut_tests(void);       /* implemented in provideo_protocol_gamma_tests.c */
extern TestRef provideo_protocol_tflt_tests(void);      /* implemented in provideo_protocol_tflt_tests.c */
extern TestRef provideo_protocol_fpnc_tests(void);      /* implemented in provideo_protocol_fpnc_tests.c */
extern TestRef provideo_protocol_playback_tests(void);  /* implemented in provideo_protocol_playback_tests.c */
extern TestRef provideo_protocol_osd_tests(void);       /* implemented in provideo_protocol_osd_tests.c */
extern TestRef provideo_protocol_dpcc_tests(void);      /* implemented in provideo_protocol_dpcc_tests.c */

uint8_t g_com_port = 4;
uint32_t g_com_speed = 57600u;

/******************************************************************************
 * dump_port_list
 *****************************************************************************/
static void dump_port_list( void )
{
    // reserve memory for control channel instance
    uint8_t channel_mem[ctrl_channel_get_instance_size()];

    ctrl_channel_rs232_context_t    priv;
    ctrl_channel_handle_t           ctx;

    ctrl_channel_name_t             name;

    int res;
    int no;
    int i;
 
    ctx = (ctrl_channel_handle_t)channel_mem;
    memset( ctx, 0, ctrl_channel_get_instance_size() );

    res = ctrl_channel_rs232_init( ctx, &priv );
    if ( res )
    {
        exit ( EXIT_FAILURE );
    }

    no = ctrl_channel_get_no_ports( ctx );
    if ( no <= 0 )
    {
        exit ( EXIT_FAILURE );
    }

    printf( "------------------------------\n" );
    printf( "list of available ports\n" );
    printf( "------------------------------\n" );
    for ( i = 0; i<no; i++ )
    {
        memset( name, 0, sizeof(ctrl_channel_name_t) );
        res = ctrl_channel_get_port_name( ctx, i, name );
        printf("%02d.) %s\n", i, (char *)name );
        TEST_ASSERT( strlen(name) > 0 );
    }
    printf( "------------------------------\n" );
}

/******************************************************************************
 * main function
 *****************************************************************************/
int main( int argc, char ** argv )
{
    int c;

    opterr = 0;

    while ( (c = getopt(argc, argv, "lp:")) != -1 )
    {
        switch ( c )
        {
            case 'p':
                g_com_port = atoi( optarg );
                break;
            
            case 'l':
                dump_port_list();
                exit( 0 );
                break;

            default:
                break;
        }
    }


    printf( "------------------------------\n" );
    printf( "Start testing (port=%d).\n", g_com_port );
    printf( "------------------------------\n" );

    TestRunner_start();

    // test some math library
    //TestRunner_runTest( xyz_tests() );
    //TestRunner_runTest( conv_tests() );
    //TestRunner_runTest( cubic_tests() );
    //TestRunner_runTest( rgb2ycbcr_tests() );

    // test control channel library
    //TestRunner_runTest( ctrl_channel_tests() );
    
    // test control protocol library
    //TestRunner_runTest( ctrl_protocol_tests() );
   
    // test provideo protocol
    //TestRunner_runTest( provideo_protocol_system_tests() );
    //TestRunner_runTest( provideo_protocol_isp_tests() );
    //TestRunner_runTest( provideo_protocol_cproc_tests() );
    //TestRunner_runTest( provideo_protocol_auto_tests() );
    //TestRunner_runTest( provideo_protocol_mcc_tests() );
    //TestRunner_runTest( provideo_protocol_chain_tests() );
    //TestRunner_runTest( provideo_protocol_lut_tests() );
    //TestRunner_runTest( provideo_protocol_cam_tests() );
    //TestRunner_runTest( provideo_protocol_tflt_tests() );
    //TestRunner_runTest( provideo_protocol_fpnc_tests() );
    //TestRunner_runTest( provideo_protocol_playback_tests() );
    //TestRunner_runTest( provideo_protocol_osd_tests() );
    TestRunner_runTest( provideo_protocol_dpcc_tests() );
    
    TestRunner_end();

    return ( 0 );
}

