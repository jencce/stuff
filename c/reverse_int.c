#include <stdio.h>

static int poww(int n)
{
	int i = 1;
	while (n--)
		i = i * 2;
	return i;
}

int main(int argc, char **argv)
{
	int i;
	int bits[64] = { 0 };
	int j;

	if (argc != 2)
		return -1;

	i = strtol(argv[1], NULL, 0);
	printf("before 0x%x\n", i);
	for (j = 0; j < 64; j++)
		if (i & (1 << j))
			bits[j] = 1;

	i = 0;
	for (j = 0; j < 64; j++)
		if (bits[j] == 1)
			i += poww(63 - j);
	printf("after 0x%x\n", i);

	return 0;
}
