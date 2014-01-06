#include <sys/utsname.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

int main()
{
	struct utsname utsn;
	int ret;

	ret = uname(&utsn);
	if (ret != 0) {
		printf("uname error: %s\n", strerror(errno));
		return -1;
	}

	printf("%s\n", utsn.machine);

	return 0;
}
