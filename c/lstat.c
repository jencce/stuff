#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	struct stat buf;
	struct stat buf1;
	struct stat buf3;
	char buf2[20];
	int ret = lstat(argv[1], &buf);
	printf("lstat ret%d, mod %x, isreg %d, islink %d, isfifo %d, err %d size %d\n", ret, buf.st_mode, S_ISREG(buf.st_mode), S_ISLNK(buf.st_mode), S_ISFIFO(buf.st_mode), errno, buf.st_size);
	ret = stat(argv[1], &buf1);
	if( errno == ENOENT)
		printf("noent\n");
	printf("stat ret%d, mod %x, isreg %d, islink %d, err&noent %d, %s\n", ret, buf1.st_mode, S_ISREG(buf1.st_mode), S_ISLNK(buf1.st_mode), errno&-ENOENT, strerror(errno));
	ret = readlink(argv[1], buf2, sizeof(buf2));
	if (ret > 0)
		buf2[ret] = '\0';
	printf("iii %s\n", buf2);
	ret = stat(buf2, &buf3);
	if( errno == ENOENT)
		printf("noent\n");
	printf("stat ret%d, isreg %d, islink %d, err&noent %d, %s\n", ret, S_ISREG(buf3.st_mode), S_ISLNK(buf3.st_mode), errno&-ENOENT, strerror(errno));
	return 0;
}
