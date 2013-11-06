#include <stdio.h>

void chan(char *s)
{
	char *d = "dd";
	s = d;
}
int main()
{
	char *s = "zx";
	chan(s);
	printf("%s\n", s);
}
