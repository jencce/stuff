#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include "get_pcieaer_msg.h"

#define MAX_PAYLOAD 1024  /* maximum payload size*/
struct sockaddr_nl src_addr, dest_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;
/*
struct fm_kevt {
	int type;
	char buf[248];
};
static struct fm_kevt kmsg;

#define NETLINK_KFMD 25
*/
char *get_pcieaer_msg(void)
{
	int ret;
	int group = 1;
	struct fm_kevt *kevt = &kmsg;

	sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_KFMD);
	memset(&msg, 0, sizeof(msg));
	memset(kevt, 0, sizeof(*kevt));
	memset(&src_addr, 0, sizeof(src_addr));
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = getpid();  /* self pid */
	src_addr.nl_groups = 1;  /* not in mcast groups */
	bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));

	ret = setsockopt(sock_fd, 270, 1, &group, sizeof(group));
	if (ret < 0)
		fprintf(stderr,"setsockopt error\n");	

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid = 0;   /* For Linux Kernel */
	dest_addr.nl_groups = 0; /* unicast */

	nlh=(struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
	/* Fill the netlink message header */
	do{
		nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
		nlh->nlmsg_pid = getpid();  /* self pid */
		nlh->nlmsg_flags = 1;

		iov.iov_base = (void *)nlh;
		iov.iov_len = nlh->nlmsg_len;
		msg.msg_name = (void *)&dest_addr;
		msg.msg_namelen = sizeof(dest_addr);
		msg.msg_iov = &iov;
		msg.msg_iovlen = 1;

		/* Read message from kernel */
		memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
		ret = recvmsg(sock_fd, &msg, 0);
	    kevt = (struct fm_kevt *)NLMSG_DATA(nlh);
	}while(0);
	/* Close Netlink Socket */
	close(sock_fd);
    if (kevt->buf[0] != '\0')
        return kevt->buf;
    return NULL;
}

int main(int argc, char* argv[])
{
    char *msg = NULL;
    msg = get_pcieaer_msg();
    /*
     */
    return 0;
}

