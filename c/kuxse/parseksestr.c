#include <stdio.h>

int main()
{
	char *s = "c101,c22,c3,c4";
	char *p = s;
	char *tp = s;
	char temp[5];
	int ucnt = 0;
	int l = strlen(s);
	int i, c = 0;
#if 0
	
	for (i = 0; i < l; i++) {
		if (s[i] != ',') {
			if (i == l - 1) {
				printf("ted %s\n", p);
			}
			continue;
		}
		tp = &s[i-1];
		ucnt = tp - p + 1;
		printf("ucnt %d\n", ucnt);
		memset(temp, 0, 5);
		strncpy(temp, p, ucnt);
		printf("t %s\n", temp);
		p = &s[i+1];
	}
#endif
	for (i = 0; i < l; i++) {
		if (s[i] == ',') {
			tp = &s[i-1];
			ucnt = tp - p + 1;
			printf("ucnt %d\n", ucnt);
			memset(temp, 0, 5);
			strncpy(temp, p, ucnt);
			printf("t %s\n", temp);
			sscanf(temp, "c%d", &c);
			printf("c %d\n", c);
			p = &s[i+1];
			continue;
		}
		if (i == l - 1 && s[i] != ',') {
			tp = &s[i];
			ucnt = tp - p + 1;
			printf("ucnt %d\n", ucnt);
			memset(temp, 0, 5);
			strncpy(temp, p, ucnt);
			printf("t %s\n", temp);
			sscanf(temp, "c%d", &c);
			printf("c %d\n", c);
		}
	}
}
