#include <stdio.h>

int main()
{
	int i = 0;
	for (; i < 10; i++)
	{
		switch (i) {
		case 1:
			printf("1\n");
		case 2:
			printf("2\n");
			break;
		}
		printf("for\n");
	}
	return 0;
}
