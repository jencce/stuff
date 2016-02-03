#include <stdio.h>

int main()
{
	printf("sizeof (void ) %d\n", sizeof (void ));  //1
	printf("sizeof (void *) %d\n", sizeof (void *)); //4
	printf("sizeof (char) %d\n", sizeof (char)); //1
	printf("sizeof (short) %d\n", sizeof (short)); //2
	printf("sizeof (unsigned short) %d\n", sizeof (unsigned short)); //2
	printf("sizeof (int) %d\n", sizeof (int)); //4
	printf("sizeof (unsigned int) %d\n", sizeof (unsigned int)); //4
	printf("sizeof (long) %d\n", sizeof (long)); //4
	printf("sizeof (unsigned long) %d\n", sizeof (unsigned long)); //4
	printf("sizeof (double) %d\n", sizeof (double)); //8
	printf("sizeof (long long) %d\n", sizeof (long long)); //8
	printf("sizeof (unsigned long long) %d\n", sizeof (unsigned long long)); //8
	return 0;
}
