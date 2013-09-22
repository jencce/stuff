#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <setjmp.h>

struct sigaction sig;
static sigjmp_buf env;
int setjmp_called = 0;
int got_sigsegv = 0;

void func(s)
{
	if(s == SIGSEGV) {
		got_sigsegv++;
		printf("got segsegv\n");
	}

	if(setjmp_called) {
		setjmp_called = 0;
		siglongjmp(env, 1);
	}
}

int main(int argc, char **argv)
{
	int *adr;
	pid_t pid;
	int i, status;
	
	adr = (int *)strtoul(*(++argv), NULL, 16);
	printf("adr %d\n", adr);	

	sig.sa_handler = func;
	sig.sa_flags = 0;
	if(sigaction(SIGSEGV, &sig, (struct sigaction *)NULL) == -1) {
		printf("sigaction error\n");
		exit(1);
	}

	pid = fork();
	if(pid < 0) {
		printf("fork error\n");
		exit(1);
	}

	if(pid == 0) {
		setjmp_called = 1;
		if(sigsetjmp(env, 1) == 0)
			i = *adr;
		if(got_sigsegv == 0)
			printf("Fail gs%d\n", got_sigsegv);
		else
			printf("Pass gs%d\n", got_sigsegv);
		exit(0);
	}
	pid = wait(&status);
	if(pid < 0) {
		printf("error status %d\n", status);
		exit(1);
	}
	printf("status %d\n", status);
	return 0;
}
