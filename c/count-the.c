#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buff[100];

int main()
{
	char *tk;
	char *sp;
	char *p;
	int r = 0;

	scanf( "%[^\n]", buff );
	p = buff;
#if 0
	if((tk = strtok(buff, " ")) != NULL )
	{
		while( tk = strtok( NULL, " " ) )    // sequence strtok calls with NULL
		{									 // NULL as first arg
			printf("%s ", tk);
			if( strncmp("the", tk, 3) == 0 )
				r++;
		}
	}
#else
	while( tk = strtok_r(p, " ", &sp) )      // reentrant strtok_r
	{
		printf("%s ", tk);
		if( strncmp("the", tk, 3) == 0 )
			r++;
		
		p = sp;
		//p = NULL;                          // sequence call NULL and sp both ok
	}
#endif
	printf("\n%d\n", r);
	
	return 0;
}
