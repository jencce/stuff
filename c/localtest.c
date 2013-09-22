#include <stdio.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
	int i, j, l;
	struct timeval s, e;
	int d[200][200];
	int t = 200;

	if(argv[1])
		l = atoi(argv[1]);
	for(i = 0; i < 200; i++)
	for(j = 0; j < 200; j++)
		d[i][j] = i*j;
	if(l)
	{
		gettimeofday(&s, NULL);
		printf("sec %ld, usec %ld\n", s.tv_sec, s.tv_usec);
		for(i = 0; i < 200; i++)
		for(j = 0; j < 200; j++)
			t = t + d[i][j];
		gettimeofday(&e, NULL);
		printf("end sec %ld, usec %ld\n", e.tv_sec, e.tv_usec);
	} else
	{
		gettimeofday(&s, NULL);
		printf("sec %ld, usec %ld\n", s.tv_sec, s.tv_usec);
		for(j = 0; j < 200; j++)
		for(i = 0; i < 200; i++)
			t = t + d[i][j];
		gettimeofday(&e, NULL);
		printf("end sec %ld, usec %ld\n", e.tv_sec, e.tv_usec);
	}
	return 0;
}
