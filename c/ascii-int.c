#include <stdio.h>
#include <string.h>

int ascii_to_integer( char *string )
{
	int i, l = strlen( string );
	int res = 0;	

	for( i = 0; i < l; i++ )
	{
		if ( string[i] < '0' || string[i] > '9' )
			return 0;

		res = res*10 + (string[i] - '0');
	}
	
	return res;
}

int main( int argc, char **argv  )
{
	if ( argc != 2 )
	{
		printf("string arg needed\n");
		return 1;
	}

	if ( argv[1] )
		printf( "ascii %s to int %d\n", argv[1], ascii_to_integer(argv[1]) );
	return 0;
}
