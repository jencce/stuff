#include <stdio.h>
#include <string.h>

#define _GNU_SOURCE

int main()
{
	char *s = "asdf";
	printf("%s\n", strfry(s));
	return 0;
}
