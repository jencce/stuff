#include <stdio.h>
#include <string.h>

int main()
{
	char *s = "abcdefghijklmn";
	char a[5] = "";

	//strncpy( a, s, 6 );
	//a[4] = '\0'; 
		// strncpy does not add a NUL terminator to dst string.
		// without the above line, array 'a' wont have a right
		// nul terminator, so it may cause over bound problem.

	strncat( a, s, 6 );
		// strncat add a NUL terminator to dst string after all
		// 'n' chars has been copyed.

	// above two functions takes unconventional arguments.
	// when array is out of bound, gcc Stack Smashing mechanism
	// detects buffer overflow attack. this protection can be
	// disabled by option -fno-stack-protector while compiling.

	printf("%s len %d\n", a, strlen(a));
	printf("len %d\n",  strlen(a));
	
	return 0;
}
