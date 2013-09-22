#include <stdio.h>
#include <errno.h>

static int z=0;
static int x=0;
static int m;
static int n;
int rq()
{
	printf("dd  ");
	if(m == n)
		printf("do  ~~~z=%d, x=%d, m=%d, n=%d\n", z, x, m, n);
	else
	{
	if(z < (m-1))
	{
		if(x < n)
		{
			printf("do  ~~~z=%d, x=%d, m=%d, n=%d\n", z, x, m, n);
			z++;
			x++;
		} else {
			printf("z=%d, x=%d, m=%d, n=%d\n", z, x, m, n);
			z++;
		}
		
	}
	else {
		printf("z=%d, x=%d, m=%d, n=%d\n", z, x, m, n);
		x = 0;
		z = 0;
	}
	}
	return 0;
}

int main(int argv, char** args)
{
	int i = 0;
	char s[] = "asdfgh";
	
	m = atoi(args[1]);
	n = atoi(args[2]);
	printf("m%d n%d\n", m, n);
/*	for(i = 0; i < 32; i++)
	{
		sleep(1);
		rq();	
	}
*/	perror("errorrrrrr\n");
	return 0;
}
