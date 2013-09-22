#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main()
{
	int err;
	err = symlink("aa", "stoaa");
	printf("err=%d, errno=%d\n", err, errno);
	
	err = 0;
	err = mkdir("aa", "777");
	printf("err=%d, errno=%d\n", err, errno);

	err = 0;
	err = rmdir("stoaa");
	printf("err=%d, errno=%d\n", err, errno);
/*
	err = 0;
	err = rmdir("aa");
	printf("err=%d, errno=%d\n", err, errno);

	err = 0;
	err = unlink("stoaa");
	printf("err=%d, errno=%d\n", err, errno);
*/
	return 0;
}
