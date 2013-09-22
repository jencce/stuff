#include <stdio.h>

int main()
{
	printf("%d, e %d\n", getuid(), geteuid());
	return 0;
}
