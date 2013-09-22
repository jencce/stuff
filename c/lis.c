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
	struct sockaddr_in      sin5;
	struct sockaddr_un      sinu;
	pid_t pid;
	char stnbf[100];
	
	strcpy( stnbf, "/tmp/unix.XXXXXX" );
	tfd = mkstemp( stnbf );
	if( tfd < 0 )
	{
		perror("mkstemp error");
		exit(-1);
	}
	close( tfd );

	sinu.sun_family = AF_UNIX;
	strcpy( sinu.sun_path, stnbf );
	len = sizeof(struct sockaddr_un) - sizeof(sinu.sun_path) + strlen(sinu.sun_path);
	unlink(stnbf);
	unlink(stnbf);

	if( (pid = fork()) == 0 )
	{
		
		sock = socket(AF_UNIX, SOCK_STREAM, 0);
		if( sock == -1 )
		{
			perror("socket failed");
			unlink(stnbf);
			exit(-1);
		}
	
		ret = bind( sock, (struct sockaddr *)&sinu, len );
		if( ret == -1 )
		{
			perror("bind failed");
			printf("errno = %d(%s)\n", errno, strerror(errno));
			unlink(stnbf);
			exit(-1);
		}

		ret = listen( sock, 3 );
		if( ret == -1 )
		{
			perror("listen failed");
			unlink(stnbf);
			printf("errno = %d(%s)\n", errno, strerror(errno));
			exit(-1);
		}

		sleep(200);
		printf("chld weak up\n");
		//unlink(stnbf);
		exit(1);
	}

	sleep(2);
	for( val = 0; val < 5; val++ )
	{
		csk[val] = socket(AF_UNIX, SOCK_STREAM, 0);
		if( csk[val] == -1 )
		{
			perror("csk socket failed");
			unlink(stnbf);
			exit(-1);
		}
	
		int val1 = 2;
		int vasize = 10;
		ret = setsockopt( csk[val], SOL_SOCKET, SO_SNDTIMEO, &val1, sizeof(val1) );
		if( ret == -1 )
		{
			perror("set failed");
			printf("errno = %d(%s)\n", errno, strerror(errno));
			exit(-1);
		}
	
		ret = connect( csk[val], (const struct sockaddr *)&sinu, (socklen_t)len );
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
 
	unlink(stnbf);
	return 0;
}
