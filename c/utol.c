#include <stdio.h>

int main()
{
	unsigned int umask = 0x01;
	//int mask = 0x01;
	int mask = 0x70000000;
	unsigned long long i = 0xffffffffffffffff;

	printf("i=0x%llx\n", i);
	printf("mask=0x%x\n", mask);
	printf("~mask=0x%x\n", ~mask);
	printf("ull~mask=0x%llx\n", (unsigned long long)~mask);
	i &= ~mask;
	printf("i=0x%llx\n", i);

	printf("\n================\n");

	mask = 0x80000000;
	i = 0xffffffffffffffff;
	printf("i=0x%llx\n", i);
	printf("mask=0x%x\n", mask);
	printf("~mask=0x%x\n", ~mask);
	printf("ull~mask=0x%llx\n", (unsigned long long)~mask);
	i &= ~mask;
	printf("i=0x%llx\n", i);

	printf("\n================\n");

	mask = 0x1;
	i = 0xffffffffffffffff;
	printf("i=0x%llx\n", i);
	printf("mask=0x%x\n", mask);
	printf("~mask=0x%x\n", ~mask);
	printf("ull~mask=0x%llx\n", (unsigned long long)~mask);
	i &= ~mask;
	printf("i=0x%llx\n", i);

	printf("\n================\n");

	i = 0xffffffffffffffff;
	printf("i=0x%llx\n", i);
	printf("umask=0x%x\n", umask);
	printf("~umask=0x%x\n", ~umask);
	printf("ull~umask=0x%llx\n", (unsigned long long)~umask);
	i &= ~umask;
	printf("i=0x%llx\n", i);

	printf("\n================\n");
	umask = 0x01u;
	printf("(int)umask=0x%x,%u\n", (int)umask, (int)umask);
	printf("(ulong)umask=0x%lx\n", (unsigned long)umask);
	printf("(ulong long)umask=0x%llx\n", (unsigned long long)umask);
	umask = 0x80000000u;
	printf("(int)umask=0x%x,%u\n", (int)umask,(int)umask);
	printf("(ulong)umask=0x%lx\n", (unsigned long)umask);
	printf("(ulong long)umask=0x%llx\n", (unsigned long long)umask);

	printf("\n================\n");
	mask = 0x1;
	printf("(int)mask=0x%x,%d\n", (int)mask, (int)mask);
	printf("(long)mask=0x%lx\n", (long)mask);
	printf("(long long)mask=0x%llx\n", (long long)mask);
	mask = 0x80000000;
	printf("(int)mask=0x%x,%d\n", (int)mask, (int)mask);
	printf("(long)mask=0x%lx, %ld\n", (long)mask, (long)mask);
	printf("(long long)mask=0x%llx,%lld\n", (long long)mask, (long long)mask);

	return 0;
}
