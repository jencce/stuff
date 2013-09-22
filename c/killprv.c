#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define STIME 5

int p1_pipe[2];
int p2_pipe[2];
int p3_pipe[2];

sigset_t sig_empty;

int sig_rc;

void sig_catch(sg_num)
{
	sig_rc = sg_num;
	printf("sig_rc ed %d\n", sig_rc);
}

int main()
{
	int ret, p1_rc, p2_rc, p3_rc;
	pid_t pid;
	struct sigaction act;

	sigemptyset(&sig_empty);
	act.sa_flags = 0;
	act.sa_mask = sig_empty;
	act.sa_handler = sig_catch;

	ret = sigaction(SIGUSR1, &act, (struct sigaction *)NULL);
	if( ret == -1 )
		printf("sigaction fail\n");
	
	if(pipe(p1_pipe) != 0 || pipe(p2_pipe) != 0 || pipe(p3_pipe) != 0)
		printf("pipe error\n");	

	pid = fork();

	if(pid == 0)
	{
		/* p1 */
		if( setuid((uid_t)-999) == -1)
			printf("setuid fail in p1\n");
		if(getuid != -999 || geteuid != -999)
			printf("setuid fail in p1 u %d,eu %d\n", getuid(), geteuid());
		
		pid = fork();
		if(pid == 0)
		{
			/* p2 */
			ret = setsid();
			if( ret == -1 )
				printf("setsid fail in p2 \n");
			
			pid = fork();
			if(pid == 0)
			{
				/* p3 */
				ret = setsid();
				if( ret == -1 )
					printf("setsid fail in p3 \n");
				sig_rc = 0;
				sleep(5*STIME/2);
				write(p3_pipe[1], (char *)&sig_rc, sizeof(int));
			} else
			{
				/* p3 pa */
				sig_rc = 0;
				sleep(3*STIME/2);
	
				kill((pid_t)-1, SIGUSR1);
				write(p2_pipe[1], (char *)&sig_rc, sizeof(int));
				
			}
		} else
		{
			/* p2 pa */
			sig_rc = 0;
			sleep(3*STIME/2);

			write(p1_pipe[1], (char *)&sig_rc, sizeof(int));
		}
	} else
	{
		sig_rc = 0;
		sleep(STIME);

		read(p1_pipe[0], (char *)&p1_rc, sizeof(int));
		sleep(STIME/2);

		read(p2_pipe[0], (char *)&p2_rc, sizeof(int));
		sleep(STIME/2);

		read(p3_pipe[0], (char *)&p3_rc, sizeof(int));
		sleep(STIME/2);

		printf("p %d, p1 %d, p2 %d, p3 %d\n", sig_rc, p1_rc, p2_rc, p3_rc);

	}
}
