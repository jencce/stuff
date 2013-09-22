#include <stdio.h>
#include <errno.h>


int main(int argv, char** args)
{
	int i = 0;
	//char *s = "asdfgh";
	char s[] = "asdfgh";
	char *a[] = {"qw", "ewe", "sasd", "gggg"};
	char **b = {"qw", "ewe", "sasd", "gggg"};
	char c[][5] = {"qw", "ewe", "sasd", "gggg"};

	printf("a[0]: %s\n", a[0]);
	printf("a[0]+1: %s\n", a[0] + 1);
	printf("a+1: %s\n", a + 1);
	
/*
	printf("b[0]: %s\n", b[0]);
	printf("*b+1: %s\n", *b + 1);
	printf("**b+1: %s\n", **b + 1);
*/
	
	printf("c[0]: %s\n", c[0]);
	printf("c[0]+1: %s\n", c[0] + 1);
	printf("c+1: %s\n", c + 1);
	

	printf("s old: %s\n", s);
	s[i] = 'g';
	printf("s new: %s\n", s);
	//s++; error

	return 0;
}
