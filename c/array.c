#include <stdio.h>

int main()
{
	int a[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

	printf("%d\n", 2[a]);
	/* array vaiables use bracket to access members */
	/* array[subscript] means *(array + (subscript )) */
	/* so 2[a] equals to a[2] */
	return 0;
}
