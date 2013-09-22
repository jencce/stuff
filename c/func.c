#include <stdio.h>

int main()
{
	func(1,2);
	funcd();
	// if no declaration before a func being called, compiler
	// takes it as a int-returned function, no other check.
	// so funcd get warned, func not
}

int func(void)
{
	printf("zx1\n");
	return 0;
}

void funcd(void)
{
	printf("zx\n");
}
