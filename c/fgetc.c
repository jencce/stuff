#include <stdio.h>
#include <errno.h>

int main()
{
	int ret;
	FILE *fp = fopen( "gn", "rw" );
	ret = fseek( fp, 10, SEEK_SET );
	ret = fgetc(fp);
	printf("%d, %s\n", ret, strerror(errno));
	return 0;
}
