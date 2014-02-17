#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int ret = 0;
	int fd ;
	
	if (argc != 2) {
		printf("one arg needed for lock\n");
		return -1;
	}

	fd = open(argv[1], O_RDWR);
	if (fd == -1) {
		perror("open failed");
		return -1;
	}

	ret = lseek(fd, 0, 0);
	if (ret == -1) {
		perror("lseek failed");
		return -1;
	}

	ret = lockf(fd, F_TLOCK, 0);
	if (ret == -1) {
		perror("lockf failed");
		return -1;
	}
	
	ret = lockf(fd, F_ULOCK, 0);
	if (ret == -1) {
		perror("ulockf failed");
		return -1;
	}
	
	ret = close(fd);
	if (ret == -1) {
		perror("close failed");
		return -1;
	}
	
	return 0;
}
