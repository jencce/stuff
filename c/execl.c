#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int     fildes[2];
char     buf[200];
char     reply[200];
int main()
{
	timer_t timerid;
	pid_t pid;
	int re, status;
	
	if(pipe(fildes) < 0) {
		printf("pipe error\n");
		exit(1);
	}
		
	pid = fork();
	if(pid < 0) {
		printf("fork error\n");
		exit(1);
	}

	if(pid == 0) {
	
		close(1);
		if(dup2(fildes[1], 1) < 0) {
			printf("dup2 error\n");
			exit(1);
		}
		re = timer_create(CLOCK_REALTIME, NULL, &timerid);
		if(re == -1) {
			printf("timer_create error\n");
			exit(1);
		}
	
		sprintf(reply, "%x", timerid);
		execl("/home/zx/c/execl_ck", "execl_ck", "timers", reply, NULL);
		//execl("/bin/ls", "ls", "/tmp", NULL);
		printf("timerid %d\n", timerid);
		(void *)timer_delete(timerid);
		exit(1);
	
	}
	pid = wait(&status);
	if(pid < 0) {
		printf("error status %d\n", status);
		exit(1);
	}
	printf("status %d\n", status);
	read(fildes[0], buf, 200);
	printf("*buf %s*\n", buf);
	close(fildes[0]);
	close(fildes[1]);
	return 0;
}
