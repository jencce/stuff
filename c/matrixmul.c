#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define DMAX 8

void nochunks(int e[DMAX][DMAX], int f[DMAX][DMAX], int g[DMAX][DMAX])
{
	int i, j, k;
	int sum = 0;

	for(i = 0; i < DMAX; i++)
		for(j = 0; j < DMAX; j++)
		{
			sum = 0;
			for(k = 0; k < DMAX; k++)
			{
				sum += e[i][k] * f[k][j];
			}
			g[i][j] = sum;
		}
}

/*
 * spliting matrix to several sub-matrix makes better locality
 * in temporal, coz hit cache more frequently.
 *
*/
void chunks(int e[DMAX][DMAX], int f[DMAX][DMAX], int g[DMAX][DMAX])
{
	int i, j, k, ii, jj, kk;
	int sum = 0;

	for(i = 0; i < DMAX; i++)
	for(j = 0; j < DMAX; j++)
		g[i][j] = 0;

	for(ii = 0; ii < DMAX; ii = ii + DMAX/2)
	{
		for(jj = 0; jj < DMAX; jj = jj + DMAX/2)
		{
			for(k = 0; k < DMAX; k++) 
			{
				for(i = ii; i < ii + DMAX/2; i++)
				{
					sum = g[k][i];
					for(j = jj; j < jj + DMAX/2; j++)
					{
						sum += e[k][j] * f[j][i];
					}
					g[k][i] = sum;
				}
			}
		}
	}
}

void display(int d[DMAX][DMAX])
{
	int i, j;
	printf("\n");
	for(i = 0; i < DMAX; i++)
	{
		for(j = 0; j < DMAX; j++)
			printf("%d\t", d[i][j]);
		printf("\n");
	}
}

int a[DMAX][DMAX];
int b[DMAX][DMAX];
int c[DMAX][DMAX];
int main(int argc, char **argv)
{
	int i, j, k, l, m;
	struct timeval s, e;
	int t = DMAX;

	if(argv[1])
		l = atoi(argv[1]);
	if(argv[2])
		m = atoi(argv[2]);
	for(i = 0; i < DMAX; i++)
	for(j = 0; j < DMAX; j++)
	{
		a[i][j] = i%4;
		b[i][j] = j%4;
		c[i][j] = 0;
	}

	if(l)
	{
		gettimeofday(&s, NULL);
		printf("sec %ld, usec %ld\n", s.tv_sec, s.tv_usec);
		chunks(a, b, c);
		gettimeofday(&e, NULL);
		printf("end sec %ld, usec %ld\n", e.tv_sec, e.tv_usec);
		double s1, e1;
		s1 = (double)s.tv_sec + (double)(s.tv_usec/1000000.0);
		e1 = (double)e.tv_sec + (double)(e.tv_usec/1000000.0);
		printf("%lf  %lf\n", s1, e1);
		printf("%lf\n", e1 - s1);
	} else
	{
		gettimeofday(&s, NULL);
		printf("no sec %ld, usec %ld\n", s.tv_sec, s.tv_usec);
		nochunks(a, b, c);
		gettimeofday(&e, NULL);
		printf("end sec %ld, usec %ld\n", e.tv_sec, e.tv_usec);
		double s1, e1;
		s1 = (double)s.tv_sec + (double)(s.tv_usec/1000000.0);
		e1 = (double)e.tv_sec + (double)(e.tv_usec/1000000.0);
		printf("%lf  %lf\n", s1, e1);
		printf("%lf\n", e1 - s1);
	}
	//display(a);
	//display(b);
	if(m)
		display(c);

	printf("clock %d\n", clock());
	return 0;
}
