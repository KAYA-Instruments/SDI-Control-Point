
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


