#include <stdio.h>

int array[3][10] = { { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 },
					 { 10,11,12,13,14,15,16,17,18,19 },
					 { 20,21,22,23,24,25,26,27,28,29 } };

int sa[10] = { 1, 2, 3};
int a = 100;

int main()
{
	int *sp = sa;

	printf("a = %d\n", a);
	printf("sa-p %p\n", sa); 
	printf("sa-lu %lu\n", sa); 
	printf("&sa-p %p\n", &sa); 
	printf("&sa-lu %lu\n", &sa); 
	
	printf("sp-p %p\n", sp); 
	printf("sp-lu %lu\n", sp); 
	printf("&sp-p %p\n", &sp); 
	printf("&sp-lu %lu\n", &sp); 


	printf("%p\n", array); //pointer to a 1-d array
	printf("%p\n", *array);// 1-d array
	printf("%d\n", **array);

	int *ap = array;
	printf("%d\n", *ap);
	printf("%d\n", *(++ap));
	printf("%d\n", *(++ap)); // 0 1 2  printed

	int *p = array[0];
	printf("%d\n", *p);
	printf("%d\n", *(++p));
	printf("%d\n", *(++p)); // 0 1 2  printed
	printf("%d\n", *(p + 10)); // 12  printed

	int (*pp)[10] = array;
	printf("%d\n", **pp);
	printf("%d\n", **(++pp));
	printf("%d\n", **(++pp)); // 0 10 20  printed

	return 0;
}
