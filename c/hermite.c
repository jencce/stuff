#include <stdio.h>
#include <stdlib.h>

int hermite( int n, int x )
{
	if ( n <= 0 )
		return 1;
	if ( n == 1 )
		return 2*x;
	if ( n >= 2 )
		return (2*x*hermite( n-1, x )) - (2*(n-1)*hermite( n-2, x ));
}

int main( int argc, char **argv )
{
	long n, x;
	if ( argc != 3 )
	{
		printf("n and x needed\n");
		return 1;
	}
	
	if ( argv[1] && argv[2] )
	{
		n = strtol( argv[1], NULL, 10 );
		x = strtol( argv[2], NULL, 10 );
	}
	else
	{
		printf("n and x needed\n");
		return 1;
	}
	printf( "Hermite(n,x) = %d\n", hermite(n, x) );
	return 0;
}
