#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/xattr.h>

int main(int argc, char **argv)
{
	char *buf;
	ssize_t size;
	ssize_t ret;

	if (argc < 2 || argv[1] == NULL) {
		printf("one arg needed\n");
		exit(-1);
	}

	size = 256;
	buf = malloc(size);
	if (!buf)
		return -1;
	memset(buf, 0, size);

	ret = getxattr(argv[1], "security.kse", buf, size - 1);
	if (ret == -1) {
		perror("getxattr: ");
	}
	else
		printf("xattr %s\n", buf);

	return ret;
}
