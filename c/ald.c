#include <stdio.h>
#include <stdlib.h>

int bss[1024*1024] = {0};

void a1()
{
	printf("bss:%p\n", bss);
	return;
}
