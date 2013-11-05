#include <stdio.h>
#include <math.h>

struct hs {
	int a;
	int b;
	int c;
	int d;
	int v;
	int z;
};

void print_b(int *p)
{
	int size = sizeof(struct hs);
	unsigned long i, j, k = 0;
	int *pp = p;
	
	i = log(size)/log(2) + 1;
	printf("i %d\n", i);
	printf("pp %p\n", pp);

	for (j = 0; j < i; j++) {
		k = k | (1 << j);
	}

	printf("%lp\n", (unsigned long)pp & (unsigned long)~k);
	pp = (unsigned long)pp & (unsigned long)~k;
	printf("pp1 %p\n", pp);
	
	printf("%d\n", ((struct hs *)pp)->b);
}

int main()
{
	struct hs hs;
	hs.b = 129;
	hs.v = 12;

	print_b(&hs.v);
	return 0;
}
