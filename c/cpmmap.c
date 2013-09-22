#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	int src, dest;
	void *sm;
	void *dm;
	struct stat sb;

	if(argc != 3)
	{
		printf("usage:cpmm <src> <dest>\n");
		return 1;
	}

	if((src = open(argv[1], O_RDONLY)) < 0)
	{
		printf("open src failed %d\n", src);
		return 1;
	}

	if((dest = open(argv[2], O_RDWR|O_CREAT|O_TRUNC, S_IRWXU|S_IRWXG)) < 0)
	{
		printf("open dest failed %d\n", dest);
		return 1;
	}

	if((fstat(src, &sb)) < 0)
	{
		printf("fstat src failed\n");
		return 1;
	}

	if((lseek(dest, sb.st_size - 1, SEEK_SET)) < 0)
	{
		printf("lseek dest failed\n");
		return 1;
	}
	
	//write(dest, &sb, 1);

	sm = mmap(0, (size_t)sb.st_size, PROT_READ, MAP_PRIVATE|MAP_NORESERVE, 
			src, 0);

	if(sm == MAP_FAILED)
	{
		printf("sm map failed\n");
		return 1;
	}

	dm = mmap(0, (size_t)sb.st_size, PROT_WRITE, MAP_SHARED, dest, 0);

	if(dm == MAP_FAILED)
	{
		printf("dm map failed\n");
		return 1;
	}

	memcpy(dm, sm, (size_t)sb.st_size);

	printf("copy succeed\n");

	return 0;
}
