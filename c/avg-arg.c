#include <stdio.h>
#include <stdarg.h>

double avg( int n_values, ... )
{
	va_list vl;
	int cnt;
	double sum = 0;

	va_start( vl, n_values);

	for( cnt = 0; cnt < n_values; cnt++ )
	{
		printf("cnt %d ", cnt);
		vprintf("%d\n", vl);
		sum += va_arg( vl, int );
	}
	
	va_end(vl);

	return sum/n_values;
}

int main()
{
	double d = avg( 3, 89, 191, 120 );
	printf("%lf\n", d);
	return 0;
}
