#include <stdio.h>
#include <stdint.h>

int main()
{
	size_t st = 12001;
	uintmax_t it = 121212121212;

	printf("st %zd, it %ju\n", st, it);

	return 0;
}
