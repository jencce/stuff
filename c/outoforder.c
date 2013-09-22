#include <stdio.h>
#include <string.h>
#include <sys/time.h>

int p[1000000];

int main(int argc, char **argv)
{
	int i, l, a, b, c;
	struct timeval s, e;

	a = 0;
	b = 0;
	c = 0;
	printf("sec %ld, usec %ld\n", s.tv_sec, s.tv_usec);
	if(argv[1])
	{
		printf("1\n");
		gettimeofday(&s, NULL);
		for(i = 0; i < 1000000; i++)
		{
			a += 1;
			b += 1;
			c = p[i];
		}
		gettimeofday(&e, NULL);
		double s1, e1;
                s1 = (double)s.tv_sec + (double)(s.tv_usec/1000000.0);
                e1 = (double)e.tv_sec + (double)(e.tv_usec/1000000.0);
		printf("end sec %ld, usec %ld\n", e.tv_sec, e.tv_usec);
                printf("%lf\n", e1 - s1);
	} else
	{
		gettimeofday(&s, NULL);
		for(i = 0; i < 1000000; i++)
		{
			c = p[i];
			a += 1;
			b += 1;
		}
		gettimeofday(&e, NULL);
		double s1, e1;
                s1 = (double)s.tv_sec + (double)(s.tv_usec/1000000.0);
                e1 = (double)e.tv_sec + (double)(e.tv_usec/1000000.0);
		printf("end sec %ld, usec %ld\n", e.tv_sec, e.tv_usec);
                printf("%lf\n", e1 - s1);
	}

	return 0;
}
