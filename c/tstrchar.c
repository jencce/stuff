#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	char *tmp;
	char *tmps;
	tmp = strchr( argv[1], '(' );
	tmps = tmp;
	printf("%s\n", tmps);
	return 0;
}
