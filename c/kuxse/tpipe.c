#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
	int pipefd[2];
	pid_t pid;
	int ret;
	char buf[1];

	if (argc != 2 || argv[1] == NULL) {
		errno = EINVAL;
		perror("one arg string is needed");
		return -1;
	}

	ret = pipe(pipefd);
	if (ret == -1) {
		perror("pipe");
		return -1;
	}

	pid = fork();
	if (pid == -1) {
		perror("fork");
		return -1;
	}
	
	if (pid == 0) { /* child */
		close(pipefd[1]);
		printf("child start\n");
		while (read(pipefd[0], buf, 1) > 0) {
			ret = write(STDOUT_FILENO, buf, 1);
			if (ret == -1) {
				perror("write error in child");
				return -1;
			}
		}
		close(pipefd[0]);
		printf("\nchild end\n");
		return 0; // without this return, parent write get badfd error
	}
	
	close(pipefd[0]);
	ret = write(pipefd[1], argv[1], strlen(argv[1]));
	if (ret == -1) {
		perror("write error in parent");
		return -1;
	}
	close(pipefd[1]);
	wait();

	return 0;
}
