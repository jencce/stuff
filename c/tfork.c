#include <stdio.h>

int main()
{
	int i = 0;

	i++;
	if( fork() == 0 )
	{
		printf("from %d i = %d bbbbb\n", getpid(), i);
	}
	printf("from %d i = %d aaaaa\n", getpid(), i);
	

	//while(1) sleep(1);
	return 0;
}
