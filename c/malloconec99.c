#include <stdio.h>
#include <stdlib.h>

void dd(int n)
{
	int ay[n];
	ay[0] = 10;
}

int main()
{
	dd(10);

	double *s = (double *)malloc(1*sizeof(double));
	if (s) {
		printf("freed\n");
		free(s);
	}

	return 0;
}
