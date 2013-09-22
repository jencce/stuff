#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* test memorylayout of executable file
 * 
 * ulimit -s unlimited; ./memorylayout 
 * stack size not limited, so typical layout
 * fixed heap size, fixed map and nonymous area
 * 
 * 
 * ulimit -s 100; ./memorylayout
 * stack size limited, so flexable layout
 * not fixed heap size, and not fixed map and anoy area
 * 
 * 
 * ulk p811 
*/

int main()
{
	char cmd[32];
	brk((void *)0x8051000);
	sprintf(cmd, "cat /proc/self/maps");
	system(cmd);
	return 0;
}
