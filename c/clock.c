#include <stdio.h>
#include <math.h>
#include <time.h>

int main()
{
	time_t tmt;
	printf("%u\n", clock());
	printf("%s\n", ctime(&tmt));
	printf("tmt %u\n", tmt);
	return 0;
}
