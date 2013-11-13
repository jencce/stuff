#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "kuxse.h"

#define MAC_CAT_MAX 256
#define __NR_mac_task_ctl 349

int main(int argc, char **argv)
{
	pid_t pid;
	int ret;
	int fd;
	int i;
	char buf[1];
	struct task_security_struct tss;

	if (argc != 2 || argv[1] == NULL) {
		errno = EINVAL;
		perror("one arg string is needed");
		return -1;
	}

	pid = fork();
	if (pid == -1) {
		perror("fork");
		return -1;
	}
	
	if (pid == 0) { /* child */
		get_task_xattr(getpid());
		fd = open("/tmp/ffo", O_RDONLY);
		if (fd == -1) {
			perror("open error in child");
			return -1;
		}
		printf("child start\n");
		while (read(fd, buf, 1) > 0) {
			ret = write(STDOUT_FILENO, buf, 1);
			if (ret == -1) {
				perror("write error in child");
				return -1;
			}
		}
		close(fd);
		printf("\nchild end\n");
		return 0; // without this return, parent write get badfd error
	}
	
	init_tss(&tss);
	tss.mlevel.level_type = 1;
	tss.mlevel.level_value = 1;
	tss.mlevel.level_catsum = 1;
	tss.mlevel.level_flag = 0;
	for (i = 0; i < MAC_CAT_MAX; i++)
		tss.mlevel.level_category[i] = 0;
	tss.mlevel.level_category[4] = 1;
	tss.ilevel.level_value = 7;
	
	set_task_xattr(&tss);
	
	sleep(1);
	get_task_xattr(getpid());

	fd = open("/tmp/ffo", O_WRONLY);
	if (fd == -1) {
		perror("open error in parent");
		printf("killing child\n");
		ret = kill(pid, SIGKILL);
		if (ret == -1) {
			perror("kill error in parent");
			return -1;
		}
		return -1;
	}
	ret = write(fd, argv[1], strlen(argv[1]));
	if (ret == -1) {
		perror("write error in parent");
		printf("killing child\n");
		ret = kill(pid, SIGKILL);
		if (ret == -1) {
			perror("kill error in parent");
			return -1;
		}
		return -1;
	}
	close(fd);
	wait();

	return 0;
}
