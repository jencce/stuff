#include <stdio.h>
#include <sys/time.h>

int main()
{
	//unsigned int i=2147483644;
	int i=2147483644;
	//int i=4294967290;
	//unsigned int i=4294967290;
	while(1)
	{
		printf("i:0x%x,d%d,ud%u,size %d\n", i,i,i, sizeof(i));
		i++;
	}
	return 0;
}
