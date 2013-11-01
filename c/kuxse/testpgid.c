#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("pgid %d\n", getpgid(0)); // self not checked in kernel
	printf("pgid %d\n", getpgid(12)); // startup process
	printf("pgid %d\n", getpgid(2170)); // root logined bash process
	perror("getpgid");
	return 0;
}
