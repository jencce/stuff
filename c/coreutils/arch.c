#include <sys/utsname.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	struct utsname utsn;
	int ret, i;

	if (argc != 1) {
		printf("arch: extra operand");
		for (i = 1; i < argc; i++)
			printf(" '%s'", argv[i]);
		printf("\n");
		return -1;
	}

	ret = uname(&utsn);
	if (ret != 0) {
		printf("uname error: %s\n", strerror(errno));
		return -1;
	}

	printf("%s\n", utsn.machine);

	return 0;
}
