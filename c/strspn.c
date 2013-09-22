#include <stdio.h>
#include <string.h>

int main()
{
	char bf[25] = "you shall not pass";
	char *s = "yous";

	printf("s %d\n", strspn(bf, s) );
	printf("cs %d\n", strcspn(bf, s) );

	return 0;
}
