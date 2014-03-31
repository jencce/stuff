#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	char *s = NULL;
	if (argc != 1) {
		printf("No args, thanks\n");
		return -1;
	}

	s = getlogin();
	if (s != NULL)
		printf("%s\n", s);
	return 0;
}
