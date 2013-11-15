#include <stdio.h>
#include <sys/time.h>

struct t_node {
	int a;
	char *p;
	char b;
};

int main()
{
	struct timeval tm;
	int i = 1;
	char a[i++];
	int x = 2, y = 6, z = 5;

	printf("t:%d\n", sizeof(struct t_node));

	printf("xx:%d\n", !x);
	printf("xx:%d\n", !x+y);
	printf("xx:%d\n", !x+y>z);

	if ( !x + y > z )
		printf("1\n");
	else
		printf("0\n");

	gettimeofday(&tm, NULL);
	char *s = malloc(10);
//	printf("sec:%ld, usec:%ld\n", tm.tv_sec, tm.tv_usec);
//	printf("int %d, l %d, i %d, a %d\n", sizeof(int), sizeof(long), i, sizeof(a));
	free(s);
	//pause();
	return 0;
}
