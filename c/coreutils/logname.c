#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	printf("%s\n", getlogin());
	return 0;
}
