#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void fmt( char *dst, char const * src )
{
	int l = strlen(src);
	int i, j = 0, k = 0, n = 0;

	strncpy( dst, src, l );
	
	for( i = l - 1; i >= 0; i-- )
	{ j++;
		if( j > 2 )
			k = j - 2;
		if( j == 3 )
		{
			dst[i+3] = dst[i+2];
			dst[i+2] = dst[i+1];
			dst[i+1] = '.';
			n++;
		}
		if( k != 0 && k%3 == 0 && i != 0 )
		{
			memmove( &dst[i+1], &dst[i], j+n );
			dst[i] = ',';
			n++;
		}
	}
	dst[l+n] = '\0';
	
#if 1
	if ( l == 1 )
		sprintf(dst, "0.0%c", src[0] ); 
	
	if ( l == 2 )
		sprintf(dst, "0.%c%c", src[0], src[1] ); 
#endif
}

void fmt2( char *dst, char const *src )
{
	int l = strlen(src);
	int i, j;

	if( l >= 3 )
	{
		for( i = l - 2; i > 0; i-- )
		{
			*dst++ = *src++;
			if( i%3 == 1 && i > 1 )
			{
				*dst++ = ','; 
			}
		}
	} else 
		*dst++ = '0';
	
	*dst++ = '.';
	*dst++ = l < 2 ? '0' : *src++;
	*dst++ = l < 1 ? '0' : *src++;

	*dst = '\0';
}

int main()
{
	char buff[20];
	char sb[20];

	//while( scanf( "%[0-9]", buff ) == 1 )
	// if use above partten, another getchar needed to comsume the \n
	// to make loop works well
	while( scanf( "%s", buff ) == 1 )
	{
		printf("%s\n", buff);
		fmt2( sb, buff );
		printf("%s\n", sb);
		memset( buff, 0, 20 );
		memset( sb, 0, 20 );
		//getchar();
	}
	return 0;
}
