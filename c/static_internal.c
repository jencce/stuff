#include <stdio.h>
static int a = 1;
static void f()
{
	int a = 2; //different a form main
}
static void g()
{
	printf("in g a=%d &a=%p\n", a, &a);
}
void main()
{
	//static int a = 3;  //not make a linked internal 
			// only can be used in main
			// overwrite global a
	a = 6;
	f();
	g();
	printf("a=%d &a=%p\n", a, &a);
}
