#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <linux/un.h>
#include <errno.h>

int main()
{
	int sock = 0, ret = 0, val = 0, len = 0, tfd = 0;
	int csk[5];
	struct sockaddr_in      sin;
	pid_t pid;
	
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr("192.168.0.119");
	sin.sin_port = htons(10292);

	if( (pid = fork()) == 0 )
	{
		
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if( sock == -1 )
		{
			perror("socket failed");
			exit(-1);
		}
	
		ret = bind( sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in) );
		if( ret == -1 )
		{
			perror("bind failed");
			printf("errno = %d(%s)\n", errno, strerror(errno));
			exit(-1);
		}

		ret = listen( sock, 3 );
		if( ret == -1 )
		{
			perror("listen failed");
			printf("errno = %d(%s)\n", errno, strerror(errno));
			exit(-1);
		}

		sleep(100);
		exit(1);
	}

	sleep(3);
	for( val = 0; val < 5; val++ )
	{
		csk[val] = socket(AF_INET, SOCK_STREAM, 0);
		if( csk[val] == -1 )
		{
			perror("csk socket failed");
			exit(-1);
		}
	
		ret = connect( csk[val], (const struct sockaddr *)&sin, sizeof(struct sockaddr) );
		if( ret != 0 )
		{
			printf("csk [%d] connect errno = %d(%s)\n", val,
					errno, strerror(errno));
			//unlink(stnbf);
			//unlink(stnbf);
			//exit(-1);
		}
		else
			printf("cn ok\n");
	}
 
	return 0;
}
