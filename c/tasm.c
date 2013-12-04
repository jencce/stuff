int main()
{
	char *s = "asd";
	__asm__ __volatile__ (
	" mov %0, %%ax\n"
	" mov %%ax, %%bp\n"
	" mov $16, %%cx\n"
	" mov $0x1301, %%ax\n"
	" mov $0x00c, %%bx\n"
	" mov $0, %%dl\n"
	" int $0x10\n"
	" ret\n"
	::""(s)
	);
	return 0;
}
