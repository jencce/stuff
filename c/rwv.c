#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	int fd, ret;
	struct iovec iov[3];
	struct iovec iov_r[3];
	
	if (argc != 5) {
		printf("Usage: ./xxx path/to/file string0 string1 string2\n");
		return -1;
	}

	fd = open(argv[1], O_RDWR|O_CREAT, S_IRWXU|S_IWOTH);
	if (fd == -1) {
		printf("open %s failed\n", argv[1]);
		return -1;
	}

	iov[0].iov_base = argv[2];
	iov[0].iov_len = strlen(argv[2]);
	iov[1].iov_base = argv[3];
	iov[1].iov_len = strlen(argv[3]);
	iov[2].iov_base = argv[4];
	iov[2].iov_len = strlen(argv[4]);

	ret = writev(fd, iov, 3);
	if (ret == -1) {
		printf("writev failed\n");
		close(fd);
		return -1;
	}

	sync();

	ret = lseek(fd, 0, SEEK_SET);
	if (ret == -1) {
		printf("lseek failed\n");
		close(fd);
		return -1;
	}

	iov_r[0].iov_len = 1;
	iov_r[0].iov_base = (char *)malloc(1);
	iov_r[1].iov_len = 2;
	iov_r[1].iov_base = (char *)malloc(2);
	iov_r[2].iov_len = 3;
	iov_r[2].iov_base = (char *)malloc(3);

	ret = readv(fd, iov_r, 3);
	if (ret == -1) {
		printf("readv failed\n");
		close(fd);
		return -1;
	} else {
		printf("v0 %s\n", iov_r[0].iov_base);
		printf("v1 %s\n", iov_r[1].iov_base);
		printf("v2 %s\n", iov_r[2].iov_base);
		printf("argv2 %s\n", argv[2]);
		printf("argv3 %s\n", argv[3]);
		printf("argv4 %s\n", argv[4]);
	}

	ret = lseek(fd, 0, SEEK_SET);
	if (ret == -1) {
		printf("lseek failed\n");
		close(fd);
		return -1;
	}

	iov[0].iov_base = argv[2];
	iov[0].iov_len = strlen(argv[4]);
	iov[1].iov_base = argv[3];
	iov[1].iov_len = strlen(argv[2]);
	iov[2].iov_base = argv[4];
	iov[2].iov_len = strlen(argv[3]);

	ret = readv(fd, iov, 3);
	if (ret == -1) {
		printf("readv failed\n");
		close(fd);
		return -1;
	} else {
		printf("v0 %s\n", iov[0].iov_base);
		printf("v1 %s\n", iov[1].iov_base);
		printf("v2 %s\n", iov[2].iov_base);
		printf("argv2 %s\n", argv[2]);
		printf("argv3 %s\n", argv[3]);
		printf("argv4 %s\n", argv[4]);       // argv[] alters
	}

	close(fd);
	return 0;
}
