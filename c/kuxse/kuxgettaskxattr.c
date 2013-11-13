#include <stdio.h>
#include "kuxse.h"


int main(int argc, char **argv)
{
	if (argc > 1 && argv[1]) {
		int pid = strtol(argv[1], NULL, 0);
		printf("to get attr of pid %d\n", pid);
		get_task_xattr(pid);
		
	} else {
		get_task_xattr(getpid());
	}

	return 0;
}
