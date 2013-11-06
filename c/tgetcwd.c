#include <stdio.h>
#include <unistd.h>

int main()
{
	printf("%s\n", get_current_dir_name());
	//printf("pm %d\n", fpathconf(STDOUT_FILENO, _PC_PATH_MAX));
	//printf("pm %d\n", fpathconf(0, _PC_PATH_MAX));
	printf("pm %d\n", pathconf(NULL, _PC_PATH_MAX));// error
	return 0;
}
