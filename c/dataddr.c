#include <stdio.h>
#include <stdlib.h>

int data[1024*1024] = {1};

void a1()
{
	printf("data:%p\n", data);
	return;
}
