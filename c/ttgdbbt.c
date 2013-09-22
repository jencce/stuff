#include <stdio.h>

void f4(int i)
{
	printf("%d\n", i);
}

void f3(int i)
{
	printf("%d\n", i);
	i = i + 4;
	printf("%d\n", i);
	i = i + 4;
	printf("%d\n", i);
	i = i + 4;
	printf("%d\n", i);
	f4(i);
}

void f2(int i)
{
	printf("%d\n", i);
	i = i + 3;
	printf("%d\n", i);
	i = i + 3;
	printf("%d\n", i);
	i = i + 3;
	printf("%d\n", i);
	f3(i);
}

void f1(int i)
{
	printf("%d\n", i);
	i = i + 2;
	printf("%d\n", i);
	i = i + 2;
	printf("%d\n", i);
	i = i + 2;
	printf("%d\n", i);
	f2(i);
}

int main()
{
	f1(1);
	return 0;
}
