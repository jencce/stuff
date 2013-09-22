#include <stdio.h>

void bin_to_ascii( unsigned int value )
{
	int qt;
	qt = value / 10;

	if ( qt != 0 )
		bin_to_ascii( qt );

	printf("%c", '0' + (value%10) );

}

int main()
{
	bin_to_ascii(121234567);
	printf("\n");
	return 0;
}
