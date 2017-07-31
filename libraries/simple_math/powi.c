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
 
static int pow_rec( int x, unsigned int y )
{
    if ( y == 1 )
    {
        return ( x );
    }

    return ( x * pow_rec( x, (y-1) ) );
}

int powi( int x, int y )
{
    if ( y == 0 )
    {
        return ( 1 );
    }

    if ( y == 1 )
    {
        return ( x );
    }

    if ( (x == 0) || (y < 0) )
    {
        // (y<0) not supported
        return ( 0 );
    }

    if ( x == 2 )
    {
        return ( 1 << y );
    }

    return ( x * pow_rec( x, y - 1) );
}


