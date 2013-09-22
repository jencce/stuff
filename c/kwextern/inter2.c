#include <stdio.h>
#include "inter.h"

const char *s[10];
const char *s[10] = {"12"};

int main()
{
	printf("1%s\n", names[0]);
	printf("1%s\n", s[0]);
	return 0;
}
