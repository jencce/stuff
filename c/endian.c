#include <stdio.h>

int main()
{
	unsigned int i = 1;
	unsigned char c = (unsigned char)i;
	printf("%d\n", c);
	if (c)
		printf("little\n");
	else
		printf("big\n");
	
	return 0;
}
