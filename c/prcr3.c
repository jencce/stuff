#include <stdio.h>

int main()
{
	unsigned long a;
	//register unsigned long lcr3 asm("cr3");
	//printf("cr3 %ld\n", lcr3);
	asm volatile("mov %%cr3, %%eax\n"
		     "mov %%eax, %0\n"    : "=rm"(a));
	printf("%lu\n", a);
	return 0;
}
