#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/xattr.h>

void dis_issbuf(char *buf)
{
	char *s = buf;
	int i = 0;
	s = strtok(s, ":");
	while (s != NULL) {
		//printf("%s\n", s);

		switch (i) {
		case 0:
			printf("multi level security type: %s\n", s);
			break;
		case 1:
			printf("                    value: %s\n", s);
			break;
		case 2:
			printf("                 category: %s\n", s);
			break;
		case 3:
			printf("multi level integrity type: %s\n", s);
			break;
		default:
			break;
		}
		s = strtok(NULL, ":");
		i++;
	}
}

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

	ret = getxattr(argv[1], "security.kse", buf, size);
	if (ret == -1) {
		perror("getxattr: ");
	}
	else
		printf("xattr %s\n", buf);
	dis_issbuf(buf);

	return ret;
}
