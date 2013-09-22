#include <stdio.h>
#include <stdlib.h>

extern int data[];
int main()
{
	printf("pid:%d, data:%p\n", getpid(), data);
	a1();
	pause();
}
