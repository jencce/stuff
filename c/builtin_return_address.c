#include <stdio.h>

void f1()
{
	printf("f1 0:%p\n", __builtin_return_address(0));
	printf("f1 1:%p\n", __builtin_return_address(1));
}

int main()
{
here1:
	printf("here1: %p\n", &&here1);
	f1();	
	printf("main: %p\n", __builtin_return_address(0));

	//__label__ here;
here2:
	printf("here2: %p\n", &&here2);
	return 0;
}
