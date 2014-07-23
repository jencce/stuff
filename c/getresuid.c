#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>

int main()
{
	uid_t r, e, s;
	int ret = 0;

	ret = getresuid(&r, &e, &s);
	if (ret == -1) {
		perror("getresuid:");
		return -1;
	}

	printf("r %d, e %d, s %d\n", r, e, s);
	return 0;
}
