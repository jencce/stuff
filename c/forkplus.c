#include <stdio.h>
#include <unistd.h>

int a;

int main()
{
	int b;
	pid_t p;

	a = 0;
	b = 0;
	
	p = fork();
	if (p == 0) {
		a++;
		b++;
	}

	printf("pid %d, %d, %d\n", getpid(), a, b);
	return 0;
}
