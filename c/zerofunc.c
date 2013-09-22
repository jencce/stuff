#include <stdio.h>
#include <errno.h>

int main()
{
	(*(void(*)())0)();
	return 0;
}
