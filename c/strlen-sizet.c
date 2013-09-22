#include <stdio.h>
#include <string.h>

char *t = "asdasdasda";
int main()
{
	char *s = "helwsd";

	printf("%u\n", strlen(s) - strlen(t) );
y
	if ( strlen(s) - strlen(t) >=0 )
		printf("ok\n");
	// strlen returns a size_t type data which is a unsigned
	// type, so cals between these results always be positive.
	// ok is always printed above.

	return 0;
}
