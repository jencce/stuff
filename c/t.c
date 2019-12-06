#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

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
	time_t t = time(NULL);
	struct tm *tmp;
	char str[50];
	tmp = localtime(&t);
	printf("time %d\n", t);
	strftime(str, sizeof(str), "%T", tmp);
	printf("ctime %s t2 %s\n", ctime(&t), str);
	char *s = malloc(10);
//	printf("sec:%ld, usec:%ld\n", tm.tv_sec, tm.tv_usec);
//	printf("int %d, l %d, i %d, a %d\n", sizeof(int), sizeof(long), i, sizeof(a));
	free(s);
	//pause();
	return 0;
}
