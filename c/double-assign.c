#include <stdio.h>

int main()
{
	char b = 0;
	int a = 0;
	short y = 32767;

	a = b = y + 1;

	printf("a = %d, b = %d\n", a, b);

	return 0;
}
