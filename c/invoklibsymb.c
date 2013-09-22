#include <stdio.h>
#include <stdlib.h>

extern int b;
extern int c;
int main()
{
	a1();
	a1();
	while(1)
	{
		printf("pid:%d, b:%p, b=%d, a1:%p, c=%d\n", getpid(), &b, b, a1, c);
		sleep(1);
	}
	pause();
	return 0;
}
