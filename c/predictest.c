#include <stdio.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
	int i, l;
	struct timeval s, e;
	int d[200];
	int t = 200;

	if(argv[1])
		l = atoi(argv[1]);
	for(i = 0; i < 200; i++)
		d[i] = i*200;
	if(l)
	{
		gettimeofday(&s, NULL);
		printf("sec %ld, usec %ld\n", s.tv_sec, s.tv_usec);
		for(i = 0; i < 200; i++)
			d[i] *= d[i];
		gettimeofday(&e, NULL);
		printf("end sec %ld, usec %ld\n", e.tv_sec, e.tv_usec);
	} else
	{
		gettimeofday(&s, NULL);
		printf("sec %ld, usec %ld\n", s.tv_sec, s.tv_usec);
		while(t--)
			d[i] *= d[i];
		gettimeofday(&e, NULL);
		printf("end sec %ld, usec %ld\n", e.tv_sec, e.tv_usec);
	}
	return 0;
}
