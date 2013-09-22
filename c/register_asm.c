int main()
{
	register unsigned long lesp asm("esp");
	unsigned long llesp asm("esp");
	register unsigned long lsp asm("sp");
	//register unsigned long leip asm("ip");
	printf("%lx\n", lesp);
	printf("%lx\n", llesp);
	printf("%lx\n", lsp);
	//printf("%lx\n", leip);

	return 0;
}
