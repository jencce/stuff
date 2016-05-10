#include <stdio.h>
int main()
{
	printf("__alignof__(int) = %d\n", __alignof__(int));
	printf("__alignof__(unsigned int) = %d\n", __alignof__(unsigned int));
	printf("__alignof__(long) = %d\n", __alignof__(long));
	printf("__alignof__(unsigned long) = %d\n", __alignof__(unsigned long));
	printf("__alignof__(float) = %d\n", __alignof__(float));
	printf("__alignof__(double) = %d\n", __alignof__(double));
	return 0;
}
