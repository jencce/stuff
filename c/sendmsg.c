#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/in.h>
#include <linux/un.h>
#include <errno.h>


char data[] = "ABCDEFGHIJKLMNOPQ123TEST!";

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
		int fd, len;
		int va = 0;
		int cnt, scnt;
		char buff[1024];

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
		
		va = 0;
		setsockopt( sock, SOL_SOCKET, SO_OOBINLINE, &va, sizeof(va) );

		ret = listen( sock, 3 );
		if( ret == -1 )
		{
			perror("listen failed");
			printf("errno = %d(%s)\n", errno, strerror(errno));
			exit(-1);
		}

		len = (socklen_t)sizeof(sin);
		fd = accept( sock, (struct sockaddr *)&sin, &len ); 
		while( (cnt = recv( fd, buff, sizeof(buff), 0 )) != 0 )
		{
			if( cnt == -1 )
				continue;
#if 1
			scnt = send( fd, buff, cnt, 0 );
			if( scnt == -1 )
			{
				printf("server send failed %s\n", strerror(errno));
				return -1;
			}
#endif
		}
		sleep(100);
		exit(1);
	}

	sleep(3);
	//for( val = 0; val < 5; val++ )
	val = 0;
	{
		csk[val] = socket(AF_INET, SOCK_STREAM, 0);
		if( csk[val] == -1 )
		{
			perror("csk socket failed");
			exit(-1);
		}
	
		ret = connect( csk[val], (const struct sockaddr *)&sin, sizeof(struct sockaddr) );
		if( ret != 0 )
		{ printf("csk [%d] connect errno = %d(%s)\n", val, errno, strerror(errno)); exit(-1);
		}
		else
			printf("cn ok\n");

		int slen = strlen( data );
		ret = send( csk[val], data, slen, 0 );
		if( ret == -1 )
		{
			printf("send failed client errno = %d(%s)\n", errno, strerror(errno));
			exit(-1);
		}
		
		struct msghdr rhdr;
		struct iovec riov[5];
		char rbuf[1024];
		int i;

		memset( rbuf, 0, 1024);
		rhdr.msg_name = 0;
		rhdr.msg_namelen = 0;
		rhdr.msg_iov = riov;
		rhdr.msg_iovlen = 4;
		rhdr.msg_control = (void *)0;
		rhdr.msg_controllen = 0;
		for( i = 0; i < 4; i++ )
		{
			riov[i].iov_base = &rbuf[i*5];
			riov[i].iov_len = 5;
		}		

		ret = recvmsg( csk[val], (struct msghdr *)&rhdr, 0 );
		if( ret == -1 )
		{
			printf("recvmsg failed client errno = %d(%s)\n", errno, strerror(errno));
			exit(-1);
		}
		else
			printf("recvmsg %d(%s)\n", ret, rbuf);
	}
 
	return 0;
}
