#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//#define N 10

double fibo_rec(int n)
{
	if (n == 0)
		return 0;
	if (n == 1 || n == 2)
		return 1;
	else
		return fibo_rec(n-1) + fibo_rec(n-2);
}

//double da[N];

double fibo_ite(int n)
{
	double *da = NULL;
	double i;
	int j;
	
	#if 1
	if (n == 0)
		return 0;
	if (n == 1 || n == 2)
		return 1;
	#endif

	while (da == NULL)
		da = (double *)malloc((n + 1)*sizeof(double));

	da[0] = 0;
	da[1] = da[2] = 1;

	for (j = 3; j <= n; j++) 
		da[j] = da[j-1] + da[j-2];

	i = da[n];
	free((void *)da);
	return i;
}

int main(int argc, char **argv)
{
	int i = 0;
	int ret = 0;
	struct timeval start, end;
	double s, e;
	unsigned long int n;
	
	if (argc != 0 && argv[1])
		n = strtoul(argv[1], NULL, 0);
	else
		n = 40;

	ret = gettimeofday(&start, NULL);
	if (ret != 0) {
		perror("start gettime error\n");
		return 1;
	}

	#if 0
	for (i = 0; i <= N; i++) {
		#ifdef REC
		printf("fibo_rec %d: %lf\n", i, fibo_rec(i));
		#else
		printf("fibo_ite %d: %lf\n", i, fibo_ite(i));
		#endif
	}
	#else
	#ifdef REC
	printf("fibo_rec %lu: %lf\n", n, fibo_rec(n));
	#else
	printf("fibo_ite %lu: %lf\n", n, fibo_ite(n));
	#endif
	#endif
		
	ret = gettimeofday(&end, NULL);
	if (ret != 0) {
		perror("end gettime error\n");
		return 1;
	}

	s = (double)start.tv_sec + (double)start.tv_usec/1000000.0;
	e = (double)end.tv_sec + (double)end.tv_usec/1000000.0;
	printf("%lf elapsed\n", e - s);

	return 0;
}
