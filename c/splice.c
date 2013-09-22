#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
	int pipe_fd[2];
	int in_fd;
	int ret;
	pid_t pid;

	ret = pipe(pipe_fd);
	if (ret == -1) {
		printf("pipe error: %s\n", strerror(errno));
		return 1;
	}

	pid = fork();
	if (pid == -1) {
		printf("fork error: %s\n", strerror(errno));
		return 1;
	}

	if (pid == 0) {   /* child process */
		close(pipe_fd[1]); /* close write end  */
		char buff[23];
		
		memset(buff, 0, 23);
		ret = read(pipe_fd[0], buff, 20);
		if (ret == -1) {
			printf("read error: %s\n", strerror(errno));
			return 1;
		}

		close(pipe_fd[0]);
		printf("buff: %s\n", buff);
		//abort();
		//return -3;
		exit(-3);

	} else { /* parent process */
		int status = 0;
		close(pipe_fd[0]); /* close read end  */

		in_fd = open("splice.c", O_RDONLY);
		if (in_fd == -1) {
			printf("open error: %s\n", strerror(errno));
			return 1;
		}

		ret = splice(in_fd, NULL, pipe_fd[1], NULL, 20, SPLICE_F_MOVE);
		if (ret == -1) {
			printf("splice error: %s\n", strerror(errno));
			return 1;
		}

		printf("spliced\n");
		close(pipe_fd[1]);

		wait(&status);
		printf("status:%x\n", status);
		if (WIFEXITED(status))
			printf("child exit normally, status %d\n", (int)WEXITSTATUS(status));
		else
			printf("child exit not normally\n");
	}
	
	return 0;
}
