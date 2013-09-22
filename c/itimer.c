#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int cnt = 0;

void sig_hand()
{
	if (cnt++ == 10)
		exit(0);
	printf("siged\n");
}

int main()
{
	struct timeval tv;
	struct timeval tv_i;
	struct itimerval itv;
	struct itimerval itv_o;
	int ret = 0;
	struct sigaction act;

	memset(&act, 0, sizeof(act));
	act.sa_handler = sig_hand;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	ret = sigaction(SIGALRM, &act, NULL);
	if (ret != 0) {
		perror("sigaction error:");
		return -1;
	}

	tv.tv_sec = 2;
	tv.tv_usec = 0;
	tv_i.tv_sec = 3;
	tv_i.tv_usec = 0;

	itv.it_value = tv;
	itv.it_interval = tv_i;

	ret = setitimer(ITIMER_REAL, &itv, NULL);
	if (ret != 0) {
		perror("setitimer error");
		return -1;
	}

	while (1) sleep(1);
	return 0;
}
