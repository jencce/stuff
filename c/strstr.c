#include <stdio.h>

char *strstr(char *s1, char *s2)
{
	char *s2h = s2;
	char *s1s = NULL;
	int flag = 0;
	while (*s1 != '\0') {
		s2 = s2h;
		flag = 0;
		while (*s2 != '\0' && *s1 != '\0') {
			if (*s1 != *s2) {
				flag = 1;
				break;
			}
			if (s1s == NULL)
				s1s = s1;
			s1++;
			s2++;
		}
		if (flag == 0)
			return s1s;
		s1++;
	}
}

int strlen(const char *s)
{
	int len = 0;
	while (s != NULL && *s != '\0') {
		len++;
		s++;
	}

	return len;
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("2 args needed\n");
		return 1;
	}
	printf("strstr(%s,%s) : %s\n", argv[1], argv[2], 
			strstr(argv[1], argv[2]));;
	printf("strlen(%s) : %d\n", argv[1], strlen(argv[1]));
	return 0;
}
