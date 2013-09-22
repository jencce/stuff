#include <stdio.h>

#define PR( fmt, val ) printf( "value of " #val " is " fmt"\n", val)
// #arg translates to arg

//#define ASIZE 1
#define ASIZE 10

#ifndef ASIZE
	#error dm11esg     // generate error mesg
#endif

// the last define taken on multi defines
// cmd line define via -DASIZE=x is before these defines

int main()
{
	printf("%s %d\n", __FILE__, __LINE__ );
	printf("STDC %d. %s %s\n", __STDC__, __DATE__, __TIME__ );
	// preprocessor defined variable

	printf("STDC %d. %s %s\n", __STDC__, __DATE__, __TIME__ );
	PR( "%d", __LINE__ );
	PR( "%d", ASIZE );
	perror( "this" );
	return 0;
}
