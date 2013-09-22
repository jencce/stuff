#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int genball(int base)
{
	struct timeval tv;
	int i, j;

	j = RAND_MAX % base;

	int ret = gettimeofday(&tv, NULL);
	if (ret == -1) {
		perror("gtm failed");
		return -1;
	}
	srand((unsigned int)tv.tv_usec);
	
	do
	{
		i = rand();
	} while (i >= (RAND_MAX - j));

	return (i % base + 1);
}

int ind( int j, int d[], int k)
{
	int i;
	for (i = 0; i < k; i++)
		if (d[i] == j)
			return 1;
	return 0;
}

int main()
{
	int i, j;
	int d[8];

	for(i = 0; i < 7; i++)
	{
		if (i != 6) {
			do
				j = genball(33);
			while (ind(j, d, 7) != 0);
			d[i] = j;
		} else {
			do
				j = genball(16);
			while (ind(j, d, 7) != 0);
			d[i] = j;
		}
	}

	printf("red ball ");
	for(i = 0; i < 7; i++)
	{
		if (i != 6)
			printf(" %d", d[i]);
		else
			printf("\nblue ball %d\n", d[i]);
	}
	return 0;
}
