#include <stdio.h>

int array[12] = {[0 ... 3] = 9, [1] = 1, [1] = 0};

void main()
{
	int i;

	for (i = 0; i < 4; i++)
		printf("%d\n", array[i]);
}
