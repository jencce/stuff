#include <stdio.h>

int main()
{
	int a = 0xfafa;
	unsigned int b = 0xfafa;

	// highest bit 0, fill bits with 0
	printf("0x%x, %d\n", a, a);
	printf("0x%x, %u\n", b, b);
	printf("0x%x, %d\n", a >> 8, a >> 8);
	printf("0x%x, %d\n", b >> 8, b >> 8);

	a = 0xfffffafa;
	b = 0xfffffafa;
	// highest bit 1, signed fill bits with 1
	// unsigned fill bits with 0
	printf("0x%x, %d\n", a, a);
	printf("0x%x, %u\n", b, b);
	printf("0x%x, %d\n", a >> 8, a >> 8);
	printf("0x%x, %u\n", b >> 8, b >> 8);
	return 0;
}
