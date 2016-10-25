#include <stdio.h>
#include <math.h>

struct hs {
	int a;
	int b;
	int t;
	int y;
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
	printf("pp %p\n", pp);

	for (j = 0; j < i; j++) {
		k = k | (1 << j);
	}

	pp = (unsigned long)pp & (unsigned long)~k;
	printf("pp1 %p\n", pp);
	
	printf("result: %d\n\n", ((struct hs *)pp)->b);
}

void print_b1(int *p)
{
	int size = sizeof(struct hs);
	unsigned long i, j, k = 0;
	int *pp = p;
	
	i = log(size)/log(2) + 1;
	printf("pp %p\n", pp);

	for (j = 0; j < i; j++)
		pp = (long)pp & (long)(~(1 << j));

	printf("pp1 %p\n", pp);
	
	printf("result: %d\n\n", ((struct hs *)pp)->b);
}

void print_b3(int *p)
{
#define container_of(p, type, member)\
	(type *) ((char *)p - (size_t)(&((type *)0)->member))
	struct hs* pp = container_of(p, struct hs, v);
	//struct hs* pp = (struct hs *)((char *)p - (long)(&((struct hs *)0)->v));
	printf("offset %ld\n", (long)(&((struct hs *)0)->v));
	printf("p - offset %x\n", ((char *)p - (long)(&((struct hs *)0)->v)));
	printf("p - offset1 %x\n", (p - (long)(&((struct hs *)0)->v)));
	// without char * transform, p cannot just goes back by offset,
	// it just goes back by offset*sizeof(int)
	printf("p %p, pp %p\n", p, pp);
	printf("result: %d\n", pp->b);
}

int main()
{
	struct hs hs;
	hs.b = 129;
	hs.v = 12;

	printf("hs.t offset %d\n", (size_t)(&((struct hs *)0)->t));
	printf("hs.t offset %d\n", (&((struct hs *)0)->t));
	print_b(&hs.v);
	print_b1(&hs.v);
	print_b3(&hs.v);
	return 0;
}
