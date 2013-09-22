#include <stdio.h>

const int d[20];
int const f[20];
int main()
{
#if 0
	d[1] = 1;
	f[1] = 2;

	d++; 
	f++;	
#endif                  //all compile error

	const int *a;
	int const *b;
	int * const d;
	int c;
	
	a = &c;
	*a = 1;  //error

	b = &c;
	*b = 2; //error

	*d = 3;
	d = &c;  //error

	return 0;
}
