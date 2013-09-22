#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
	char a;
	int b;
	char c;
}a;

typedef struct {
	int b;
	char a;
	char c;
}b;

int main()
{
	printf("%d\n", offsetof(a, c)); // 8
	printf("%d\n", offsetof(b, c)); // 5

	char *s = NULL;
	free(s);         // free NULL, nothing happened
	
	return 0;
}
