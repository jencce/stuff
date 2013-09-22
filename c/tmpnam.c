#include <stdio.h>

int main()
{
	char s[20];
	printf("%s, s %s\n", tmpnam(s), s);
	return 0;
}
