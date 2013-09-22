#include <stdio.h>

int main()
{
	int a = 3;
	int b = 0;
	b = a > 0 ? : 9;
	printf("%d\n", b);
	b = a < 0 ? : 2;
	printf("%d\n", b);
	return 0;
}
