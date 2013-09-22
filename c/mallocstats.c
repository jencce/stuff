#include <stdio.h>
#include <malloc.h>

int main()
{
	char *s;
	s = malloc(20);
	malloc_stats();
	return 0;
}
