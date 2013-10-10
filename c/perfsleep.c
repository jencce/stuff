#include <stdio.h>
#include <time.h>
#include <sys/select.h>

int main()
{
	struct timespec ts;
	struct timeval te;
	int i;

	for (i = 0; i < 10; i++) {
		ts.tv_sec = 0;
		ts.tv_nsec = 10000000;
		nanosleep(&ts, NULL);

		te.tv_sec = 0;
		te.tv_usec = 40000;
		select(0, NULL, NULL, NULL, &te);
	}
	return 0;
}
