#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("pgid %d\n", getpgid(0));
	printf("pgid %d\n", getpgid(12));
	printf("pgid %d\n", getpgid(2041));
	return 0;
}
