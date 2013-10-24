#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
 * strsub(char *str, char *old, char *new)
 * replace string old in str with new
 * */
char *strsub(char *str, char *old, char *new)
{
	int tl, ol, nl, sl, i = 0, j;
	char *gs[255] = {NULL, };
	char *sp = str;
	char *tn;
	char *tnp;

	if (str == NULL || old == NULL || new == NULL)
		return NULL;

	sl = strlen(str);
	ol = strlen(old);
	nl = strlen(new);

	if (sl < ol)
		return NULL;

	while (gs[i] = strstr(sp, old)) 
		sp = gs[i++] + strlen(old);

	if (i == 0)
		return NULL;

	tl = sl + i * (nl - ol);
	tn = (char *)malloc(tl);
	if (tn == NULL)
		return NULL;

	sp = str;
	tnp = tn;
	j = 0;
	while (j < i) {
		int tmp = nl;
		char *np = new;

		while (sp < gs[j])
			*tnp++ = *sp++;
		while(tmp--)
			*tnp++ = *np++;

		sp = sp + ol;
		j++;
	}
	while (*sp != '\0')
		*tnp++ = *sp++;
	*tnp = '\0';	
	return tn;
}

char *strsub1(char *str, char *old, char *new)
{
	int tl, ol, nl, sl, i = 0, j;
	char *gs[255] = {NULL, };
	char *sp = str;
	char *tn;
	char *tnp;

	if (str == NULL || old == NULL || new == NULL)
		return NULL;

	sl = strlen(str);
	ol = strlen(old);
	nl = strlen(new);

	if (sl < ol)
		return NULL;

	while (gs[i] = strstr(sp, old)) 
		sp = gs[i++] + strlen(old);

	if (i == 0)
		return NULL;

	tl = sl + i * (nl - ol);
	tn = (char *)malloc(tl);
	if (tn == NULL)
		return NULL;

	sp = str;
	tnp = tn;
	j = 0;
	while (j < i) {
		strncat(tnp, sp, gs[j] - sp);
		strcat(tnp, new);
		sp = gs[j] + ol;
		j++;
	}
	if (*sp != '\0')
		strcat(tnp, sp);
	
	return tn;
}

int main(int argc, char **argv)
{
	printf("1 %s\n", strsub(argv[1], argv[2], argv[3]));
	printf("2 %s\n", strsub1(argv[1], argv[2], argv[3]));
	return 0;
}
