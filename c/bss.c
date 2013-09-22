#include <stdio.h>

int array[1024*1024] = {1};
int main()
{
	char s[16];
	int i = getpid();
	printf("%d\n", i);
	sprintf(s, "cat /proc/%d/maps", i);
	printf("%s\n",s);
	i = system(s); 
	pause();
	return 0;
}
