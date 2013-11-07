#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argc, char **argv)
{
	char *cate = NULL;
	char *p = NULL;
	char *cp = NULL;
	char *tp = NULL;
	unsigned long scontext_len = strtoul(argv[2], NULL, 0);
	char *scontext = argv[1];
	char temp[5];
	int level_category[255];
	int i, l, ucnt, ctemp;

	if ( scontext_len < 5 || scontext == NULL )
		return -EINVAL;

	p = scontext;
	ucnt = 0;
	for (i = 0; p[i] && i < scontext_len; i++) {
		if (p[i] == ':')
			ucnt++;
	}
	/* number of ':' == 3 */
	if (ucnt != 3)
		return -EINVAL;

	/* first letter, mlevel->level_type */
	ucnt = 1; //letters' counter
	cp = scontext;
	if ( cp[0] >= '0' && cp[0] <= '9' )
		printf("first %d\n", cp[0] - '0');
	else
		printf("first %d\n", 0);

	while (*cp != ':') {
		cp++;
		ucnt++;
	}
	cp++;
	ucnt++;

	/* second letter, mlevel->level_value */
	if ( *cp >= '0' && *cp <= '9' )
		printf("secd %d\n", cp[0] - '0');
	else
		printf("secd %d\n", 0);

	while (*cp != ':') {
		cp++;
		ucnt++;
	}
	cp++;
	ucnt++;

	/* third field, mlevel categories, like c0,c1 */
	cate = malloc(scontext_len);
	if (cate == NULL)
		return -ENOMEM;
	memset(cate, 0, (int)scontext_len);
	memset(temp, 0, 5);

	p = cate;
	while (*cp != ':') {
		*p++ = *cp++;
		ucnt++;
	}
	cp++;
	ucnt++;

	if (ucnt > scontext_len)
		return -EINVAL;
	
	printf("ucnt %d, cate %s\n", ucnt, cate);
	/* fourth field, ilevel value */
	ctemp = 0;
	if (ucnt == scontext_len) {
		if (isdigit(cp[0]))
			ctemp = cp[0] - '0';
		else
			return -EINVAL;
	} else if ((ucnt + 1) == scontext_len) {
		if (isdigit(cp[0]) && isdigit(cp[1]))
			ctemp = (cp[0] - '0')*10 + cp[1] - '0';
		else
			return -EINVAL;
	} else
		return -EINVAL;

	if (ctemp >=0 && ctemp <=15)
		printf("forth %d\n", ctemp);
	else
		printf("ctemp %d\n", ctemp);

	printf("cti scon:%s, %s\n", scontext, cate);

	for (i = 0; i < 255; i++)
		level_category[i] = 0;

	p = cate;
	tp = cate;
	l = strlen(cate);
	ctemp = 0;
	ucnt = 0;
	
	for (i = 0; i < l; i++) {
		if (cate[i] == ',') {
			tp = &cate[i-1];
			ucnt = tp - p + 1;
			memset(temp, 0, 5);
			strncpy(temp, p, ucnt);
			sscanf(temp, "c%d", &ctemp);
			printf("ctemp in cate %d\n", ctemp);
			level_category[ctemp] = 1;
			p = &cate[i+1];
			continue;
		}
		if (i == l - 1 && cate[i] != ',') {
			tp = &cate[i];
			ucnt = tp - p + 1;
			memset(temp, 0, 5);
			strncpy(temp, p, ucnt);
			sscanf(temp, "c%d", &ctemp);
			printf("ctemp in cate %d\n", ctemp);
			level_category[ctemp] = 1;
		}
	}

	l = 0;
	for (i = 0; i < 255; i++)
		if (level_category[i] == 1)
			printf("cate[%d] = 1\n", i);

	
	if (cate && strlen(cate))
		free(cate);

	return 0;
}
