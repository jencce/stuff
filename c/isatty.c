#include <stdio.h>
#include <unistd.h>

int main()
{
	if (isatty(STDOUT_FILENO))
		printf("tty\n");
	else
		printf("other\n");

	return 0;
}
