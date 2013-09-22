#include <stdio.h>
#include <libgen.h>

int main(void)
{
	char *s = "/root/asdasd/asdasd/aaa";
	char *b = strdup(s);
	char *ss = dirname(b);
	printf("%s\n", ss );
	/*printf("%s\n", dirname("/root/asdasd/asdasd/aaaaaa/"));
	printf("%s\n", basename("/root/asdasd/asdasd/aaa"));
	printf("%s\n", basename("/root/asdasd/asdasd/aaaaaa/"));
*/
	return 0;
}
