#include <stdio.h>

int main()
{
	int a = 0;
	int n = 0;
	int cn = 0;
	int pn = 0;
	unsigned short b = 0;
	double f = 0;
	char s[12];
	
	memset( s, 0, 12 );
	//scanf( " %2d, %1hu, ss%s", &a, &b, s );
	//scanf( " %2d, %1hu, ss%sdd %n", &a, &b, s, &n );
	//printf( "%d, %hu, %s, n %d\n", a, b, s, n);
	cn = scanf( " %2d, %1hu, %lf%n", &a, &b, &f, &n );
	printf( "%#-+7.4x, %#e, %hu, n %d, cn = %d, %n\n", a, f, b,  n, cn, &pn);
	printf( "pn %d\n", pn );

	return 0;
}
