#include <stdio.h>
#include <stdlib.h>

int *rdin()
{
	int l, r;
	int *ay = NULL;
	int *ayp = NULL;

	r = scanf( "%d", &l );
	if( r != 1 || l <=0 )
	{
		printf("first value invalid\n");
		return NULL;
	}

	ay = (int *)malloc( (l + 1) * sizeof(int) );
	if( ay != NULL )
	{
		ayp = ay;
		*ayp++ = l;
		for( r = 0; r < l; r++ )
		{
			if( scanf( "%d", ayp++ ) != 1 )
			{
				printf("int value invalid\n");
				return NULL;
			}
		}
	}

	return ay;
}

int main()
{
	int *ay = NULL;
	int l;

	ay = rdin();
	if( ay == NULL )
	{
		printf("rd error\n");
		return -1;
	}
	
	l = ay[0];
	while( l-- >= 0 )
		printf("%d ", *ay++);
		
	printf("\n ");
	return 0;
}
