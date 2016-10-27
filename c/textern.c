#include "textern.h"

int main()
{
	printf("p %p, p[1] %c\n", p, p[1]);
#if 1
	//extern char *p; // build error, type conflict
	char *p = "12";
#else
	extern char p[];
#endif
	printf("p %p, p[1] %c\n", p, p[1]);

	return 0;
}
