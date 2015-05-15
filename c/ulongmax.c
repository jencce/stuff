#include <stdio.h>

#define ULONG_MAX (~0UL)
#define U_MAX (~0U)

int main()
{
	printf("%lu\n", ULONG_MAX);
	printf("%lx\n", ULONG_MAX);
	printf("%u\n", ULONG_MAX);
	printf("%x\n", ULONG_MAX);
	printf("%lu\n", U_MAX);
	printf("%lx\n", U_MAX);
	printf("%u\n", U_MAX);
	printf("%x\n", U_MAX);
	return 0;
}
