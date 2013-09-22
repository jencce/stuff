#include <stdio.h>
#include <string.h>

int main()
{
	char *s="";
	printf("%d\n", strlen(s));
	s=NULL;
	printf("%d\n", strlen(s));
}
