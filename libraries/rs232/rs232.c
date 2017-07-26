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
 * @file    rs232.c
 *
 * @brief   Implementation of a RS232/RSUSB command interface
 *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdint.h>

#if defined(__linux__) || defined(__FreeBSD__)
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <dirent.h>
#include <linux/serial.h>   /* TIOCGSERIAL */
#endif

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#endif

#include <ctrl_channel/ctrl_channel.h>
#include <ctrl_channel/ctrl_channel_serial.h>

/* max number of supported serial ports */
#define RS232_NO_OF_PORTS   ( 32u )

/* number of serial ports on system */
static int cport_cnt = RS232_NO_OF_PORTS;

/* name-list of available serial ports */
static ctrl_channel_name_t comports[RS232_NO_OF_PORTS];

#if defined(__linux__) || defined(__FreeBSD__)   /* Linux & FreeBSD */

/* list of file handles */
static int cports[RS232_NO_OF_PORTS];  

/* list of port settings for restauration */
static struct termios old_settings[RS232_NO_OF_PORTS];

/******************************************************************************
 * rs232_init - refills the list of available serial ports on a linux host
 *****************************************************************************/
int rs232_init( void )
{
	char name[PATH_MAX], target[PATH_MAX];
	struct dirent entry, *result;
	char buf[sizeof(entry.d_name) + 23];
    int len;
	DIR *dir;
	struct stat statbuf;

    cport_cnt = 0;
    memset( comports, 0, sizeof(comports) );

    if ( !(dir = opendir("/sys/class/tty")) )
    {
        return ( -errno );
    }

	while ( !readdir_r(dir, &entry, &result) && result )
    {
		snprintf( buf, sizeof(buf), "/sys/class/tty/%s", entry.d_name );
		
        if ( lstat( buf, &statbuf ) == -1 )
        {
			continue;
        }
		
        if ( !S_ISLNK(statbuf.st_mode) )
        {
			snprintf( buf, sizeof(buf), "/sys/class/tty/%s/device", entry.d_name );
        }
		
        len = readlink( buf, target, sizeof(target) );
		if ( (len <= 0) || (len >= (int)(sizeof(target) - 1)) )
        {
			continue;
        }

        target[len] = '\0';
		if ( strstr( target, "virtual") )
        {
			continue;
        }

        snprintf( name, sizeof(name), "/dev/%s", entry.d_name );
        if ( strstr( target, "serial8250") )
        {
            int fd;

			/* The serial8250 driver has a hardcoded number of ports.
			 * The only way to tell which actually exist on a given
             * system is to try to open them and make an ioctl call.
			 */
	        struct serial_struct serial_info;
            int res;

			if ( (fd = open(name, O_RDWR | O_NONBLOCK | O_NOCTTY)) < 0 )
            {
                /* open failed => skip */
				continue;
			}

            res = ioctl( fd, TIOCGSERIAL, &serial_info );
            
            /* close file descriptor before evaluating response */
			close( fd );
			
            if ( res != 0 )
            {
                /* ioctl failed => skip */
				continue;
			}

            if ( serial_info.type == PORT_UNKNOWN )
            {
                /* port unknown => skip*/
				continue;
			}

		}

        /* copy data to comport-list */
		strcpy( comports[cport_cnt], name );

        /* increment counter */
		cport_cnt++;
    }

    return ( 0 );
}

/******************************************************************************
 * rs232_open - opens a serial connection on a linux host 
 *****************************************************************************/
int rs232_open
(
    int const   idx,
    int const   baudrate,
    int const   data,
    int const   parity,
    int const   stop
)
{
    struct termios new_settings;

    int cbits;
    int cpar;
    int ipar;
    int bstop;
    int speed;
    int status;
    int com;
    int res;

    if ( (idx > cport_cnt) || (idx < 0) )
    {
        printf( "illegal comport number\n" );
        return ( -EINVAL );
    }
    
    if ( cports[idx] > 0 )
    {
        printf( "port already in use\n" );
        return ( -EBUSY );
    }

    // set baudrate
    switch ( baudrate )
    {
       case 9600:
           speed = B9600;
           break;

        case 19200:
           speed = B19200;
           break;

        case 38400:
           speed = B38400;
           break;

        case 57600:
           speed = B57600;
           break;

        case 115200:
           speed = B115200;
           break;

        default:
           printf( "invalid baudrate: %d\n", baudrate );
           return ( -EINVAL );
    }

    // set number of data-bits
    switch ( data )
    {
        case CTRL_CHANNEL_DATA_BITS_8:
            cbits = CS8;
            break;

        case CTRL_CHANNEL_DATA_BITS_7:
            cbits = CS7;
            break;

        case CTRL_CHANNEL_DATA_BITS_6:
            cbits = CS6;
            break;

        case CTRL_CHANNEL_DATA_BITS_5:
            cbits = CS5;
            break;

        default :
            printf( "invalid number of data-bits '%d'\n", data );
            return ( -EINVAL );
    }

    // set parity
    switch ( parity )
    {
        case CTRL_CHANNEL_PARITY_NONE:
            cpar = 0;
            ipar = IGNPAR;
            break;

        case CTRL_CHANNEL_PARITY_ODD:
            cpar = PARENB;
            ipar = INPCK;
            break;

        case CTRL_CHANNEL_PARITY_EVEN:
            cpar = (PARENB | PARODD);
            ipar = INPCK;
            break;

        default :
            printf( "invalid parity '%d'\n", parity );
            return ( -EINVAL );
    }

    // set no of stop-bits
    switch ( stop )
    {
        case CTRL_CHANNEL_STOP_BITS_1:
            bstop = 0;
            break;

        case CTRL_CHANNEL_STOP_BITS_2:
            bstop = CSTOPB;
            break;

        default :
            printf( "invalid number of stop bits '%d'\n", stop );
            return ( -EINVAL );
    }

    com = open( comports[idx], O_RDWR | O_NOCTTY | O_NDELAY );
    if ( com == -1 )
    {
        printf( "failed to open port '%s'\n", comports[idx] );
        return ( -errno );
    }

    res = tcgetattr( com, &old_settings[idx] );
    if( res == -1 )
    {
        close( com );
        printf( "unable to read portsettings\n" );
        return( -errno );
    }

    // clear
    memset( &new_settings, 0, sizeof(new_settings) );

    new_settings.c_cflag        = cbits | cpar | bstop | CLOCAL | CREAD;
    new_settings.c_iflag        = ipar;
    new_settings.c_oflag        = 0;
    new_settings.c_lflag        = 0;
    new_settings.c_cc[VMIN]     = 0;      /* block untill n bytes are received */
    new_settings.c_cc[VTIME]    = 0;

    cfsetispeed( &new_settings, speed );
    cfsetospeed( &new_settings, speed );

    res = tcsetattr( com, TCSANOW, &new_settings );
    if ( res == -1 )
    {
        close( com );
        printf( "unable to configure port-settings\n");
        return( -errno );
    }

    res = ioctl( com, TIOCMGET, &status );
    if ( res < 0 )
    {
        close( com );
        printf( "unable to get port-status\n" );
        return ( res );
    }

    status |= TIOCM_DTR;    /* turn on DTR */
    status |= TIOCM_RTS;    /* turn on RTS */

    res = ioctl( com, TIOCMSET, &status );
    if ( res < 0 )
    {
        close( com );
        printf( "unable to set port-status\n" );
        return ( res );
    }

    cports[idx] = com;

    return ( 0 );
}

/******************************************************************************
 * rs232_close - close an existing connection on a linux host
 *****************************************************************************/
void rs232_close( int const idx )
{
    if ( cports[idx] )
    {
        int status;

        if ( ioctl( cports[idx], TIOCMGET, &status ) == -1 )
        {
            printf( "unable to get port-status\n" );
        }

        status &= ~TIOCM_DTR;    /* turn off DTR */
        status &= ~TIOCM_RTS;    /* turn off RTS */

        if ( ioctl( cports[idx], TIOCMSET, &status) == -1 )
        {
            printf( "unable to set port-status\n" );
        }
    
        tcsetattr( cports[idx], TCSANOW, &old_settings[idx] );
        memset( &old_settings[idx], 0, sizeof(old_settings[idx]) );
        
        close( cports[idx] );
        cports[idx] = 0;
    }
}

/******************************************************************************
 * rs232_poll -
 *****************************************************************************/
int rs232_poll( int const idx, char * const buf, int const size )
{
    return ( read( cports[idx], buf, size ) );
}


/******************************************************************************
 * rs232_send_byte -
 *****************************************************************************/
int rs232_send_byte( int const idx, char const byte )
{
    return ( write( cports[idx], &byte, 1) );
}


/******************************************************************************
 * rs232_send -
 *****************************************************************************/
int rs232_send( int idx, char * buf, int size )
{
    return ( write( cports[idx], buf, size) );
}

#else   /* windows */

static HANDLE cports[RS232_NO_OF_PORTS];
static char mode_string[128];

/******************************************************************************
 * rs232_init - refills the list of available comports on a windows host
 *****************************************************************************/
int rs232_init( void )
{
    int res = 0;

    HKEY key;
    TCHAR *value, *data;

    unsigned long max_value_len, max_data_size, max_data_len;
    unsigned long value_len, data_size, data_len;
    unsigned long type;

    cport_cnt = 0;
    memset( comports, 0, sizeof(comports) );

    if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DEVICEMAP\\SERIALCOMM" ),
            0, KEY_QUERY_VALUE, &key) != ERROR_SUCCESS )
    {
        printf( "RegOpenKeyEx() failed\n" );
        goto out_done;
    }

    if ( RegQueryInfoKey(key, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
            &max_value_len, &max_data_size, NULL, NULL) != ERROR_SUCCESS )
    {
        printf( "RegQueryInfoKey() failed\n" );
        goto out_close;
    }

    max_data_len = max_data_size / sizeof(TCHAR);
    if ( !(value = malloc((max_value_len + 1) * sizeof(TCHAR))) )
    {
        printf( "registry value malloc failed\n" );
        goto out_close;
    }
    if ( !(data = malloc((max_data_len + 1) * sizeof(TCHAR))) )
    {
        printf( "registry data malloc failed\n" );
        goto out_free_value;
    }

    while (
        value_len = max_value_len + 1,
        data_size = max_data_size,
        RegEnumValue(key, cport_cnt, value, &value_len,
            NULL, &type, (LPBYTE)data, &data_size) == ERROR_SUCCESS)
    {
        data_len = data_size / sizeof(TCHAR);
        data[data_len] = '\0';

        /* copy data to comport-list */
        sprintf( comports[cport_cnt], "%s", data );

        /* increment counter */
        cport_cnt++;
    }

    free ( data );

out_free_value:
    free( value );
out_close:
    RegCloseKey( key );
out_done:
    return ( res );
}

/******************************************************************************
 * rs232_open - open a connection to a device via RS232/RSUSB
 *****************************************************************************/
int rs232_open
(
    int const   idx,
    int const   baudrate,
    int const   data,
    int const   parity,
    int const   stop
)
{
    DCB port_settings;
    COMMTIMEOUTS Cptimeouts;

    char buffer[128];
    char port_name[128];

    HANDLE com;

    memset( buffer, 0, sizeof(buffer) );
    memset( port_name, 0, sizeof(port_name) );
    memset( mode_string, 0, sizeof(mode_string) );

    if ( (idx > cport_cnt) || (idx < 0) )
    {
        printf( "illegal comport number\n" );
        return ( -EINVAL );
    }

    if ( cports[idx] != 0 )
    {
        printf( "port already in use\n" );
        return ( -EBUSY );
    }

    // set baudrate
    switch ( baudrate )
    {
        case   9600:
        case  19200:
        case  38400:
        case  57600:
        case 115200:
            sprintf( mode_string, "baud=%i", baudrate );
            break;

        default:
            printf( "invalid baudrate: %d\n", baudrate );
            return ( -EINVAL );
    }

    // set number of data-bits
    switch ( data )
    {
        case CTRL_CHANNEL_DATA_BITS_8:
        case CTRL_CHANNEL_DATA_BITS_7:
        case CTRL_CHANNEL_DATA_BITS_6:
        case CTRL_CHANNEL_DATA_BITS_5:
            sprintf( buffer, " data=%i", data );
            strcat( mode_string, buffer );
            break;

        default :
            printf( "invalid number of data-bits '%d'\n", data );
            return ( -EINVAL );
    }

    // set parity
    switch ( parity )
    {
        case CTRL_CHANNEL_PARITY_NONE:
            strcat( mode_string, " parity=n" );
            break;

        case CTRL_CHANNEL_PARITY_ODD:
            strcat( mode_string, " parity=o" );
            break;

        case CTRL_CHANNEL_PARITY_EVEN:
            strcat( mode_string, " parity=e" );
            break;

        default :
            printf( "invalid parity '%d'\n", parity );
            return ( -EINVAL );
    }

    // set no of stop-bits
    switch ( stop )
    {
        case CTRL_CHANNEL_STOP_BITS_1:
            strcat( mode_string, " stop=1" );
            break;

        case CTRL_CHANNEL_STOP_BITS_2:
            strcat( mode_string, " stop=2" );
            break;

        default :
            printf( "invalid number of stop bits '%d'\n", stop );
            return ( -EINVAL );
    }

    strcat( mode_string, " dtr=on rts=on" );

    /*
     * http://msdn.microsoft.com/en-us/library/windows/desktop/aa363145%28v=vs.85%29.aspx
     * http://technet.microsoft.com/en-us/library/cc732236.aspx
     */

    sprintf( port_name, "\\\\.\\%s", comports[idx] );
    com = CreateFileA( port_name,
                        (GENERIC_READ | GENERIC_WRITE),
                        0,                                  /* no share  */
                        NULL,                               /* no security */
                        OPEN_EXISTING,
                        0,                                  /* no threads */
                        NULL );                             /* no templates */

    memset( &port_settings, 0, sizeof(port_settings) );     /* clear the new struct  */
    port_settings.DCBlength = sizeof(port_settings);

    if ( !BuildCommDCBA( mode_string, &port_settings ) )
    {
        printf( "unable to set comport dcb settings\n" );
        CloseHandle( com );
        return ( -EFAULT );
    }
    
    if ( !SetCommState( com, &port_settings) )
    {
        printf("unable to set comport cfg settings\n");
        CloseHandle( com );
        return ( -EFAULT );
    }

    Cptimeouts.ReadIntervalTimeout         = MAXDWORD;
    Cptimeouts.ReadTotalTimeoutMultiplier  = 0;
    Cptimeouts.ReadTotalTimeoutConstant    = 0;
    Cptimeouts.WriteTotalTimeoutMultiplier = 0;
    Cptimeouts.WriteTotalTimeoutConstant   = 0;

    if ( !SetCommTimeouts( com, &Cptimeouts) )
    {
        printf("unable to set comport time-out settings\n");
        CloseHandle( com );
        return ( -EFAULT );
    }

    cports[idx] = com;

    return ( 0 );
}

/******************************************************************************
 * rs232_close - close an open connection to a device
 *****************************************************************************/
void rs232_close( int const idx )
{
    if ( cports[idx] )
    {
        CloseHandle( cports[idx] );
        cports[idx] = 0;
    }
}

int rs232_poll( int const idx, char * const buf, int const size )
{
    int n;
    ReadFile( cports[idx], buf, size, (LPDWORD)((void *)&n), NULL );
    return ( n );
}


int rs232_send_byte( int idx, char byte )
{
    int n;
    if ( WriteFile( cports[idx], &byte, 1, (LPDWORD)((void *)&n), NULL ) ) 
    {
        return ( n );
    }
    return ( -EIO );
}

int rs232_send( int idx, char * buf, int size )
{
    int n;
    if ( WriteFile( cports[idx], buf, size, (LPDWORD)((void *)&n), NULL) )
    {
        return ( n );
    }
    return ( -EIO );
}

#endif  /* windows */


/******************************************************************************
 * rs232_get_no_ports - get number of serial ports on host
 *****************************************************************************/
int rs232_get_no_ports( void )
{
    return ( cport_cnt );
}

/******************************************************************************
 * rs232_get_port_name - get port-name
 *****************************************************************************/
int rs232_get_port_name( int const idx, ctrl_channel_name_t name )
{
    if ( name )
    {
        memset( name, 0, sizeof(ctrl_channel_name_t) );
        memcpy( name, &comports[idx], sizeof(ctrl_channel_name_t) );
        
        return ( 0 );
    }

    return ( -EINVAL );
}


