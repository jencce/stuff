#include <stdio.h>

#define MX	1000001
#define DEV 100000

void mys( int n )
{
	n /= DEV;
	printf("%s", "**********" + 9-n );   // use string constant
}

int f1( int d )
{
	printf( "%d\n", d + 1 );
	return 0;
}

void phex( int v )
{
	int t = v / 16;

	if( t != 0 )
		phex( t );
	printf( "%c", "0123456789ABCDEF"[v%16] );
}

int main()
{
	//int (*pf)( int ) = f1;
	int (*pf)( int ) = &f1;

	(*pf)(1);
	pf(4);

	printf( "%s\n", "xyz" );
	printf( "%s\n", "xyz"+1 );

	phex(10);
	printf("\n");
	phex(102);
	printf("\n");
	printf("YYYYY");
	fflush(stdout);
	sleep(1);
	printf("\b\b\b=");
	fflush(stdout);
	printf("1000");
	sleep(1);
	printf("\n");

#if 1
	int i = 0;
	while( i < MX )
	{
		if( i == MX-1 )
		{
			i = 0;
			printf("                 \r");
			continue;
		}
		mys(i);
		//printf("%d/10", i + 1);
		//printf("%s", "**********" + 9-i );
		fflush(stdout);  // use this when printing \b\r
		//sleep(1);
		printf("\r");
		i++;
	}
#else
	mys(0);
	printf("\n");
	mys(2);
	printf("\n");
	mys(5);
	printf("\n");
#endif
	
	return 0;
}
