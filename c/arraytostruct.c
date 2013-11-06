#include <stdio.h>

struct hill {
	int a;
	int b;
};

int main()
{
	int array[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	struct hill *hs;
	struct hill *hs1;
	hs = (struct hill *)array;
	hs1 = hs + 4;
	printf("%d, %d\n", hs->a, hs1->b);
	return 0;
}
