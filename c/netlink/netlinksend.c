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

#define NETLINK_TEST 17
#define MAX_PAYLOAD 1024

int main(int argc, char **argv)
{
	struct sockaddr_nl src_addr;
	struct sockaddr_nl dest_addr;
	struct nlmsghdr *nlh = NULL;
	struct iovec iov;
	int sock_fd, ret;
	struct msghdr msg;

	sock_fd = socket( AF_NETLINK, SOCK_RAW, NETLINK_TEST );
	if( sock_fd == -1 )
	{
		perror("socket failed\n");
		return 1;
	}
	memset( &msg, 0, sizeof(msg) );
	
	memset( &src_addr, 0, sizeof(src_addr) );
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = getpid();
	src_addr.nl_groups = 0;

	ret = bind( sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr) );
	if( ret == -1 )
	{
		perror("bind failed\n");
		return 1;
	}

	memset( &dest_addr, 0, sizeof(dest_addr) );
	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid = 0;
	dest_addr.nl_groups = 0;

	nlh = (struct nlmsghdr *)malloc( NLMSG_SPACE(MAX_PAYLOAD) );
	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid = getpid();
	nlh->nlmsg_flags = 0;
	strcpy( NLMSG_DATA(nlh), "hello nl" );

	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;

	msg.msg_name = (void *)&dest_addr;
	msg.msg_namelen = sizeof(dest_addr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	ret = sendmsg( sock_fd, &msg, 0);
	if( ret == -1 )
	{
		perror("sendmsg failed\n");
		return 1;
	}

	memset( nlh, 0, NLMSG_SPACE(MAX_PAYLOAD) );
	ret = recvmsg( sock_fd, &msg, 0 );
	if( ret == -1 )
	{
		perror("recvmsg failed\n");
		return 1;
	}

	printf("received message: %s\n", (char *)NLMSG_DATA(nlh));

	close(sock_fd);

}

