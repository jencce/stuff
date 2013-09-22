#include <stdio.h>

#define typecheck(type,x) \
({      type __dummy; \
        typeof(x) __dummy2; \
        (void)(&__dummy == &__dummy2); \
        1; \
})

int main(int argv, char** args)
{
	int i = 0;
	typecheck(char, i);
	return 0;
}
