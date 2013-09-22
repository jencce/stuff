#include <stdio.h>

extern int *a;
extern int b[];

int main()
{
	int x, y;
	int ay[10] = { 10, 9, 8 };
	printf("ay = %d\n", *ay);

	printf("a = %d\n", a);
	y = b[3];
	x = a[3];
	printf("y = %d\n", y);

	return 0;
}
