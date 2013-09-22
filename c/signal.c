#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void sig_usr1(int sig)
{
	printf("SIGUSR1 %d \n", sig);
	//signal( SIGUSR1, sig_usr1 );
	exit(1);
}
int main()
{
	int ret;
	ret = signal( SIGUSR1, sig_usr1 );
	if( ret == SIG_ERR )
	{
		perror("signal error");
		exit(-1);
	}

	ret = atexit( sig_usr1 );
	if( ret != 0 )
	{
		perror("atexit error");
		exit(-1);
	}
	while(1)
		sleep(1);

	return 0;
}
