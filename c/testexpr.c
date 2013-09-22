#include <stdio.h>
#include <unistd.h>

int main()
{
	char *argv[4] = {"/usr/bin/expr", "abcdefg", ":", "\([a-c]*\)\{2,\}"};
	int ret;

	ret = execve("expr", argv, NULL);
	printf("%d\n", ret);
	return 0;
}
