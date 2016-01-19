#include <stdio.h>

int main()
{
	write(4, "ok", 2);
	printf("%m\n");

	return 0;
}
