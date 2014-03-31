#include <stdio.h>

int main(int argc, char **argv)
{
	if (argc > 2) {
		printf("none or one arg\n");
		return -1;
	}

	if (argv[1] != NULL && argv[1][0] == '-') {
		printf("no options\n");
		return -1;
	}

	if (argv[1] == NULL) {
		while (1)
			printf("y\n");
	} else {
		while (1)
			printf("%s\n", argv[1]);
	}

	return 0;
}
