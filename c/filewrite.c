#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd = 0;
	int ret;

	fd = open("/tmp/tt", O_RDWR);
	if (fd == -1)
	{
		perror("fopen failed");
		return -1;
	}
	
	ret = write (fd, "hello", 5);
	if( ret == -1 )
	{
		perror("fread error");
		return -1;
	}

	close(fd);

	return 0;
}
