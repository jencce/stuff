#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <linux/rtnetlink.h>

#define MAX_PAYLOAD 1024

int main(int argc, char **argv)
{
	struct sockaddr_nl src_addr;
	struct sockaddr_nl dest_addr;
	struct nlmsghdr *nlh = NULL;
	struct iovec iov;
	int sock_fd, ret, len;
	struct msghdr msg;
	char buff[1024];

	sock_fd = socket( PF_NETLINK, SOCK_RAW, NETLINK_ROUTE );
	if( sock_fd == -1 )
	{
		perror("socket failed\n");
		return 1;
	}
	memset( &msg, 0, sizeof(msg) );
	memset( &buff, 0, sizeof(buff) );
	
	memset( &src_addr, 0, sizeof(src_addr) );
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = 0;
	src_addr.nl_groups = RTMGRP_LINK | RTMGRP_IPV4_IFADDR;

	ret = bind( sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr) );
	if( ret == -1 )
	{
		perror("bind failed\n");
		return 1;
	}

	nlh = (struct nlmsghdr *)malloc( NLMSG_SPACE(MAX_PAYLOAD) );
	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid = 0;
	nlh->nlmsg_flags = 0;
	//strcpy( NLMSG_DATA(nlh), "hello nl" );

	iov.iov_base = buff;
	iov.iov_len = sizeof(buff);

	msg.msg_name = (void *)&src_addr;
	msg.msg_namelen = sizeof(src_addr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	memset( nlh, 0, NLMSG_SPACE(MAX_PAYLOAD) );

	len = recvmsg( sock_fd, &msg, 0);
	if( len == -1 )
	{
		perror("sendmsg failed\n");
		return 1;
	}

	printf("received %d message: %s len %d\n", nlh->nlmsg_type, buff, nlh->nlmsg_len);

	close(sock_fd);

}

