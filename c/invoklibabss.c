#include <stdio.h>
#include <stdlib.h>

extern int bss[];
int main()
{
	printf("pid:%d, bss:%p\n", getpid(), bss);
	a1();
	pause();
}
