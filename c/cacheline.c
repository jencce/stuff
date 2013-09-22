#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define AYSIZE 67108864

//int arr[AYSIZE];

int main(int argc, char **argv)
{
	int j;
	unsigned long i, ret = 0;
	struct timeval start, end;
	double s, e;
	unsigned long k;
	int *arr = NULL;

	if (argc != 2) {
		printf("one arg needed\n");
		return 1;
	} else {
		k = strtoul(argv[1], NULL, 0);
		//printf("k = %lu\n", k);
	}

	while (arr == NULL)
		arr = (int *)malloc(AYSIZE*sizeof(int));

	ret = gettimeofday(&start, NULL);
	if (ret != 0) {
		perror("start gettime error\n");
		return 1;
	}

	for (i = 0; i < AYSIZE; i += k)
		//arr[i] *= 3;
		j = arr[i];

	ret = gettimeofday(&end, NULL);
	if (ret != 0) {
		perror("end gettime error\n");
		return 1;
	}

	s = (double)start.tv_sec + (double)start.tv_usec/1000000.0;
	e = (double)end.tv_sec + (double)end.tv_usec/1000000.0;
	printf("%lu\t%lf elapsed\n", k, e - s);

	return 0;

}
