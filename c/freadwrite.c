#include <stdio.h>

struct value {
	int a;
	long b;
	char c[16];
} values[20];

int main()
{
	FILE *file = NULL;
	int ret;

	file = fopen( "dataf", "r" );
	if( file == NULL )
	{
		perror("fopen failed");
		return -1;
	}
	
	fread( values, sizeof(struct value), 19, file );
	ret = ferror( file );
	if( ret != 0 )
	{
		perror("fread error");
		clearerr(file);
		return -1;
	}

	printf("EEE%d\nH%s%n\n", sizeof(values), values, &ret);
	printf("FFF%d\n", ret);

	for( ret = 0; ret < 20; ret++ )
		printf("value %d, %d, %ld, %s\n", ret, values[ret].a, values[ret].b, values[ret].c);

	return 0;
}
