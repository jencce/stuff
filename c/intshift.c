#include <stdio.h>

int main()
{
	unsigned long l = 4294901762;
	unsigned long sl = l & 0x0000ffff;
	printf("lu %lu, ld %ld, slu %lu, sld %ld\n", l, l, sl, sl);
	printf("lu %lx, ls %lu lf %lu\n", l, (l<<16)>>16, l&65535);
	return 0;
}
