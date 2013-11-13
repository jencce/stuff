#include <stdio.h>
#include <string.h>

#define _GNU_SOURCE

int main()
{
	char s[] = "asdf";
	//char *s = "asdf"; //segment fault,  const string

	printf("%s\n", strfry(s));
	return 0;
}
