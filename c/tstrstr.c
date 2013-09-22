#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	char *tmp;
	char *tmps;
	tmp = strstr( argv[1], argv[2] );
	tmps = tmp;
	if( tmps != NULL )
		printf("%s\n", tmps);
	else
		printf("NULL\n");
			
	return 0;
}
