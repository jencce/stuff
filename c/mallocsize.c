#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	char *s;
	int i, fd;

	for(i = 1; i < 45; i++)
	{
		s = malloc(i);
		printf("%d,0x%x %d, usable %d\n", i,*(s-4), *(s-4)&0xfc,malloc_usable_size(s));
		free(s);
	}
	i = open("tt", O_SYNC);
	s = (char *)mmap(NULL, 16, PROT_READ, MAP_PRIVATE, i, 0);
	printf("%d, %p\n", i,s);
	memset(s, 0, 10);

	return 0;
}
