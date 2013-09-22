#include <stdio.h>

void f( const int a, int const b )
{
	a++;
	b++; // two error
}
int main()
{
	int a = 1, b = 2;
	f(1, 2);
	//const char * const * const p;
	const char * const * p;
	p++;
	(*p)++;
	(**p)++;
	
	return 0;
}
