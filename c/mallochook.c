#include <stdio.h>
#include <malloc.h>

static void *my_malloc_hook(size_t size, const void *caller)
{
	printf("%d alloced from %p\n", size, caller);
}

int main()
{
	__malloc_hook = my_malloc_hook;
	char *s;
	s = malloc(20);
	malloc_stats();
	printf("main %p\n", main);
	return 0;
}
