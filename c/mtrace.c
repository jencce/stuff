#include <stdio.h>
#include <mcheck.h>

int main()
{
	char *s;
	mtrace();
	s = malloc(20);
	printf("0x%x\n", *(s-4));
	free(s);
	s = malloc(10);
	muntrace();
	return 0;
}
