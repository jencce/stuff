#include <stdio.h>

void funca(void)
{
	int i, n;
	for(i = 0; i < 100000; i++)
	{
		n++;
		n--;
	}
}

void funcb(void)
{
	int i, n;
	for(i = 0; i < 1000000; i++)
	{
		n++;
		n--;
	}
}

int main()
{
	int j;
	for(j = 0; j < 100; j++)
		funca();
	for(j = 0; j < 100; j++)
		funcb();
	funcb();
	return 0;
}
