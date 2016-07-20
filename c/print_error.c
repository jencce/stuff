#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

int main(int argc, char **argv)
{
	unsigned long err = 0;

	if (argc != 2) {
		printf("Usage: ./%s errno\n", argv[0]);
		return 1;
	}

	err = strtoul(argv[1], NULL, 0);
	if (err == ULONG_MAX) {
		printf("strtoul failed: %s\n", strerror(errno));
		return 1;
	}
	
	printf("%s represents: %s\n", argv[0], strerror(err));

	return 0;
}
