#include <stdio.h>

#define ULONG_MAX (~0UL)

int main()
{
	printf("%lu\n", ULONG_MAX);
	printf("%lx\n", ULONG_MAX);
	return 0;
}
