#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int ret;

	if (argc != 2 || argv[1] == NULL) {
		printf("unlink: missing one arg\n");
		return -1;
	}

	ret = unlink(argv[1]);
	if (ret == -1) {
		perror("unlink:");
		return -1;
	}

	return 0;
}
