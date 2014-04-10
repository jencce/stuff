#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	char name[BUFSIZ];
	int ret = 0;

	ret = gethostname(name, BUFSIZ);
	if (ret == -1) {
		perror("gethostname");
		return -1;
	}
	name[BUFSIZ-1] = '0';
	printf("%s\n", name);

	return 0;
}
