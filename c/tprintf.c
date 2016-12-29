#include <stdio.h>

int main()
{
	printf("%d\n", 1, 2);  //1
	printf("%*d\n", 1, 2); //2  // 1 is field width
	printf("%**d\n", 1, 2, 3); //%1*d
	printf("%*%*d\n", 1, 2, 3); //%*d
	printf("%*%*%d\n", 1, 2, 3); //%*2
	printf("**%d\n", 1, 2, 3); //**1
	printf("%**%d\n", 1, 2, 3); //%1*2
	printf("%* *%d\n", 1, 2, 3); //%1 *2
	printf("%* %*%d\n", 1, 2, 3); //%1 %d

	printf("%4d\n%4d\n", 12345, 678);
	printf("%6d\n%6d\n", 12345, 678);
	printf("%*2$d\n", 1, 2);  //" 1"

	printf("%.3d\n", 1, 2);  //001
	printf("%.*2$d\n", 1, 2);  //01
	printf("%.3f\n", 1.12345, 2);  //1.123
	printf("%.*2$f\n", 1.12345, 2);  //1.12

	printf("%m\n");  //Success
	int i;
	printf("asdf%n\n", &i);  //asdf
	printf("%d\n", i);  //4

	return 0;
}
