#include <stdio.h>

extern char **environ;

int main(int argc, char **argv)
{
	char **env = environ;

	while(*env)
	{
		printf("env %p, %p, %s\n", env, *env, *env);
		env++;
	}
	printf("pid:%d, environ%p\n", getpid(), environ);
	printf("argc:%p, argv %p, *argv %p\n", &argc, argv, *argv);

	setenv("zx", "jjjjssss", 1);
	env = environ;

	while(*env)
	{
		printf("env %p, %p, %s\n", env, *env, *env);
		env++;
	}
	printf("pid:%d, environ%p\n", getpid(), environ);
	printf("argc:%p, argv %p, *argv %p\n", &argc, argv, *argv);

	setenv("PATH", "jjjjssss", 1);
	env = environ;

	while(*env)
	{
		printf("env %p, %p, %s\n", env, *env, *env);
		env++;
	}
	printf("pid:%d, environ%p\n", getpid(), environ);
	printf("argc:%p, argv %p, *argv %p\n", &argc, argv, *argv);

	unsetenv("zx");
	env = environ;

	while(env)
	{
		printf("env %p, %p, %s\n", env, *env, *env);
		env++;
	}
	printf("pid:%d, environ%p\n", getpid(), environ);
	printf("argc:%p, argv %p, *argv %p\n", &argc, argv, *argv);
	pause();

	return 0;
}

