#include <stdio.h>

int main()
{
	int a = 0x110;
	int b = 0x100;

	printf("a = %x; b = %x\n", a, b);
	printf("a & b = %x\n", a & b);
	printf("a | b = %x\n", a | b);
	printf("a ^ b = %x\n", a ^ b);

	return 0;
}
