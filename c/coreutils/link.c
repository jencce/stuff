#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int ret;

	if (argc != 3 || argv[1] == NULL || argv[2] == NULL) {
		printf("link: two args\n");
		return -1;
	}

	ret = link(argv[1], argv[2]);
	if (ret == -1) {
		perror("link:");
		return -1;
	}

	return 0;
}
