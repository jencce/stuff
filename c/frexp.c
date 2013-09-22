#include <stdio.h>
#include <math.h>

int main()
{
	int exp;
	double ld;
	double ip;
	ld = frexp(123, &exp);
	printf("frac %lf\n", frexp(123, &exp));
	printf("exp %d\n", exp);
	printf("value %lf\n", ldexp(ld, exp));
	ld += 10.0;
	printf("ld %lf\n", ld);
	printf("lddp %lf\n", modf(ld, &ip));
	printf("ip %lf\n", ip);
	return 0;
}
