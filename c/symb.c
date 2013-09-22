#include <stdio.h>
#include <stdlib.h>

int b = 10;
int c = 1;

void a1()
{
	printf("a1 b:%p c:%p\n", &b, &c);
	b++;
	return;
}
