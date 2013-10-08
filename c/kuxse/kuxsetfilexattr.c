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
	char *cp;
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
		printf("xattr1 %s\n", buf);

#if 1
	cp = strstr(buf, "c");
	if (cp) {
		cp++;
		*cp = '2';
		printf("buf alt %s\n", buf);
	}
#endif
	if (strlen(buf) == 0)
		strcpy(buf, "2:1:c0:1");

	ret = setxattr(argv[1], "security.kse", buf, strlen(buf), XATTR_REPLACE);
	if (ret != 0) {
		perror("setxattr :");
		exit(-1);
	}

	memset(buf, 0, size);
	ret = getxattr(argv[1], "security.kse", buf, size - 1);
	if (ret == -1) {
		perror("getxattr: ");
	}
	else
		printf("xattr2 %s\n", buf);

	return ret;
}
