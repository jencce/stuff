#include <stdio.h>

int main()
{
	long l = 0;
	int sec = 123345;
	int usec = 1234;

	printf("sec %d usec %d\n", sec, usec);

	l = sec*10000000 + usec;
	printf("l %ld\n", l);

	int rusec = l%10000000;
	int rsec = l/10000000;

	printf("rsec %d rusec %d\n", rsec, rusec);
	return 0;
}
