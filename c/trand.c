#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main()
{
	struct timeval tv;

	int ret = gettimeofday(&tv, NULL);
	if (ret == -1) {
		perror("gtm failed");
		return -1;
	}
	srand((unsigned int)tv.tv_usec);
	
	printf("%d\n", rand());
	printf("%d\n", rand());
	printf("%d\n", rand());
	printf("%d\n", rand());
	printf("%d\n", rand());

	srandom((unsigned int)tv.tv_usec);
	
	printf("%d\n", random());
	printf("%d\n", random());
	printf("%d\n", random());
	printf("%d\n", random());
	printf("%d\n", random());
	return 0;
}
