int main()
{
	int a = 0;
	int b = 1;
	__asm__ __volatile__( "movl %1, %%eax;"
			      "movl %%eax, %0;"
			      :"=r"(b)
			      :"r"(a)
			      :"%eax" );
	printf("b= %d\n", b);
	return 0;
}
