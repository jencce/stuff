#include <stdio.h>

int main(int argc, char **argv)
{
	printf("argc %d\n", argc);
	while (argc--)
		printf("%s\n", *(argv++));
	return 0;
}
