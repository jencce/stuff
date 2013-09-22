#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd, len, offset, pn;
	void *s;

	fd = open( "scanff.c", O_RDWR );
	if( fd == -1 )
	{
		perror("open failed");
		return -1;
	}

	s = mmap( NULL, 10, PROT_EXEC, MAP_PRIVATE, fd, 0 );
	if( s == (void *)-1 )
	{
		perror("mmap failed");
		return -1;
	}
	printf("%10s%n", (char *)s, &pn);
	printf("%d\n", pn);

	return 0;
}
