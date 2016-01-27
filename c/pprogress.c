#include <stdio.h>

int main()
{
	int i = 10000;
	while (i < 10010) {
		printf("i = %d", i++);
		sleep(1);
		printf("\b\b\b\b\b\b\b\b\b\b\b\b");
		fflush(stdout);
	}

	return 0;
}
