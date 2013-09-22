#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

void *thread_routine(void *arg)
{
	//int argv = *(int *)arg;
	int i = 30;
	pid_t pid = getpid();
	
	printf("pid %d\n", pid);
	while(i--)
		sleep(1);
	//argv++;
	//return &argv;
	(*(int *)arg)++;
	return arg;
}

void pr_err(char *s)
{
	printf("error: %s\n", s);
	exit(-1);
}

int main(int argc, char **argv)
{
	pthread_t pt[10];
	int i, ret;
	pthread_attr_t attr;
	void *rs = NULL;

	ret  = pthread_attr_init(&attr);
	if (ret != 0)
		pr_err("attr init");

	for (i = 0; i < 10; i++)
	{
		ret = pthread_create(&pt[i], &attr, &thread_routine, &i);
		if (ret != 0)
			pr_err("pthread create");
		printf("create pthread %d with %d\n", pt[i], i);
	}

	ret  = pthread_attr_destroy(&attr);
	if (ret != 0)
		pr_err("attr destroy");

	for (i = 0; i < 10; i++)
	{
		ret = pthread_join(pt[i], &rs);
		if (ret != 0)
			pr_err("pthread join");
		printf("joined pthread %d with %d\n", pt[i], *(int *)rs);
		rs = NULL;
	}

}
