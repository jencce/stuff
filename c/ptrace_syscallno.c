#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <asm/ptrace-abi.h>

int main()
{
	pid_t ch;
	long oeax, eax;
	long params[3];
	int status;
	int insys = 0;

	ch = fork();
	if( ch == 0 )
	{
		ptrace(PTRACE_TRACEME, 0, NULL, NULL);
		printf("ch pid %d\n", getpid());
		execl("/bin/date", "date", NULL);
	}else
	{
		while(1) {
			wait(&status);
			if(WIFEXITED(status)) break;
			oeax = ptrace(PTRACE_PEEKUSER, ch, 4*ORIG_EAX, NULL);
			//printf("%d made a %ld syscall\n", ch, oeax);
			if(oeax == SYS_write) {
				if( insys == 0 ){
					insys = 1;
					params[0] = ptrace(PTRACE_PEEKUSER, ch, 4*EBX, NULL);
					params[1] = ptrace(PTRACE_PEEKUSER, ch, 4*ECX, NULL);
					params[2] = ptrace(PTRACE_PEEKUSER, ch, 4*EDX, NULL);
					printf("write called with %ld, %ld, %ld\n", params[0], params[1], params[2]);
			}else {
				eax = ptrace(PTRACE_PEEKUSER, ch, 4*EAX, NULL);
				printf("write return with %ld\n", eax);
				insys = 0;
			}
			}
			//ptrace(PTRACE_CONT, ch, NULL, NULL);
			ptrace(PTRACE_SYSCALL, ch, NULL, NULL);
		}
	}
	return 0;
}
