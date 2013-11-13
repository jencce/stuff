#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define KUXTFIFO "/tmp/ffo"

int main(int argc, char **argv)
{
	pid_t pid;
	int ret;
	int fd;
	char buf[1];
	struct stat statbuf;

	ret = mkfifo(KUXTFIFO, S_IRWXU);
	if (ret == -1) {
		if (errno == EEXIST && !S_ISFIFO(statbuf.st_mode)) {
			ret = unlink(KUXTFIFO);
			if (ret == -1) {
				perror("unlink");
				return -1;
			}
			ret = mkfifo(KUXTFIFO, S_IRWXU);
			if (ret == -1) {
				perror("mkfifo");
				return -1;
			}
		} else {
			perror("mkfifo");
			return -1;
		}
	}
	return 0;
}
