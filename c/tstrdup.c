#include <stdio.h>

struct aa {
	char *s;
};

int main()
{
	struct aa a;
	a.s = strndup("aaaaa",5);
	printf("%s\n", a.s);
	return 0;
}
