#include <stdio.h>

int main()
{
	char buf[BUFSIZ*2];
	setbuf (stdin, buf);
	printf("hello\n");
	return 0;
}
