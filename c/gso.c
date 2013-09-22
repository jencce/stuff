#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <errno.h>

int main()
{
	int sock = 0, ret = 0, val = 0;
	struct sockaddr_in      sin5;
	
	sin5.sin_family = AF_INET;
	sin5.sin_addr.s_addr = inet_addr("10.151.12.112");
	sin5.sin_port = htons(10292);
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if( sock == -1 )
	{
		perror("socket failed");
		exit(-1);
	}

	ret = bind( sock, (struct sockaddr *)&sin5, sizeof(struct sockaddr_in) );
	if( ret == -1 )
	{
		perror("bind failed");
		printf("errno = %d(%s)\n", errno, strerror(errno));
		exit(-1);
	}

	val = 0;
	ret = setsockopt( sock, SOL_SOCKET, SO_DEBUG, &val, sizeof(val) );
	if( ret == -1 )
	{
		perror("set failed");
		printf("errno = %d(%s)\n", errno, strerror(errno));
		exit(-1);
	}
 	
	int val_size = 100;
	val = -1;
	ret = getsockopt( sock, SOL_SOCKET, SO_DEBUG, &val, &val_size );
	if( ret == -1 )
	{
		perror("get failed");
		printf("errno = %d(%s)\n", errno, strerror(errno));
		exit(-1);
	}
	printf("val = %d\n", val);
 
	val = 1;
	ret = setsockopt( sock, SOL_SOCKET, SO_DEBUG, &val, sizeof(val) );
	if( ret == -1 )
	{
		perror("set2 failed");
		printf("errno = %d(%s)\n", errno, strerror(errno));
		//exit(-1);
	}
 	
	val = 128;
	ret = setsockopt( sock, SOL_SOCKET, SO_SNDBUF, &val, sizeof(val) );
	if( ret == -1 )
	{
		perror("set failed");
		printf("errno = %d(%s)\n", errno, strerror(errno));
		exit(-1);
	}
 	
	val_size = 100;
	val = -1;
	ret = getsockopt( sock, SOL_SOCKET, SO_SNDBUF, &val, &val_size );
	if( ret == -1 )
	{
		perror("get failed");
		printf("errno = %d(%s)\n", errno, strerror(errno));
	}
	printf("SNDBUF val = %d\n", val);
 
	val = 128;
	ret = setsockopt( sock, SOL_SOCKET, SO_RCVBUF, &val, sizeof(val) );
	if( ret == -1 )
	{
		perror("set failed");
		printf("errno = %d(%s)\n", errno, strerror(errno));
	}
 	
	val_size = 100;
	val = -1;
	ret = getsockopt( sock, SOL_SOCKET, SO_RCVBUF, &val, &val_size );
	if( ret == -1 )
	{
		perror("get failed");
		printf("errno = %d(%s)\n", errno, strerror(errno));
	}
	printf("REVBUF val = %d\n", val);
 
	return 0;
}
