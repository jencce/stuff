#include <stdio.h>
#include <string.h>
#include <sys/time.h>

int main(int argc, char **argv)
{
	int i, l;
	struct timeval s, e;

	gettimeofday(&s, NULL);
	printf("sec %ld, usec %ld\n", s.tv_sec, s.tv_usec);
	if(argv[1])
		l = strlen(argv[1]);
	if(argv[2])
	{
	for(i = 0; argv[1] && i < l; i++)
		if(argv[1][i] > 'A' && argv[1][i] < 'Z')
			argv[1][i] -= 1;
	gettimeofday(&e, NULL);
	printf("end sec %ld, usec %ld\n", e.tv_sec, e.tv_usec);
	} else
	{
	for(i = 0; argv[1] && i < strlen(argv[1]); i++)
		if(argv[1][i] > 'A' && argv[1][i] < 'Z')
			argv[1][i] -= 1;
	gettimeofday(&e, NULL);
	printf("end sec %ld, usec %ld\n", e.tv_sec, e.tv_usec);
	}
	return 0;
}
