#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Mandatory access controll struct  */
struct mac_level {
	int level_type;    /* type */
	int level_value;
	int level_flag;
	int level_catsum;
	int level_category[255];
};

/* Intigrety level struct */
struct intigrety_level {
	int level_value;
};

struct inode_security_struct {
	struct mac_level mlevel;
	struct intigrety_level ilevel;
	int task_sid;
	int initialized;
};
/* context string to inode->i_security  */
static int context_to_iss(struct inode_security_struct *iss,
				const void **scontext, int scontext_len)
{
	char *cate = NULL;
	char *p = NULL;
	char *cp = NULL;
	char *tp = NULL;
	char temp[5];
	int i, l, ucnt, ctemp;

	if ( scontext_len < 5 || *scontext == NULL )
		return 0;

	p = (char *)*scontext;
	ucnt = 0;
	for (i = 0; p[i] && i < scontext_len; i++) {
		if (p[i] == ':')
			ucnt++;
	}
	if (ucnt != 3)
		return 0;

	cp = (char *)*scontext;
	if ( cp[0] >= '0' && cp[0] <= '9' )
		iss->mlevel.level_type = cp[0] - '0';
	else
		iss->mlevel.level_type = 1;

	while (*cp != ':')
		cp++;
	cp++;

	if ( *cp >= '0' && *cp <= '9' )
		iss->mlevel.level_value = cp[0] - '0';
	else
		iss->mlevel.level_value = 1;

	while (*cp != ':')
		cp++;
	cp++;

	cate = malloc(scontext_len);
	if (cate == NULL) {
		printf("zx1\n");
		return -1;
	}
	memset(cate, 0, (int)scontext_len);
	memset(temp, 0, 5);

	p = cate;
	while (*cp != ':')
		*p++ = *cp++;
	cp++;

	if ( *cp >= '0' && *cp <= '9' )
		iss->ilevel.level_value = cp[0] - '0';
	else
		iss->ilevel.level_value = 1;

	//sscanf((char *)*scontext, "%d:%d:%s:%d", &iss->mlevel.level_type,
	//		&iss->mlevel.level_value,
	//		cate, &iss->ilevel.level_value);

	printf("zx1 scontext:%s, %d:%d:%s:%d\n", (char *)*scontext,
			iss->mlevel.level_type, iss->mlevel.level_value,
			cate, iss->ilevel.level_value);

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
			iss->mlevel.level_category[ctemp] = 1;
			p = &cate[i+1];
			continue;
		}
		if (i == l - 1 && cate[i] != ',') {
			tp = &cate[i];
			ucnt = tp - p + 1;
			memset(temp, 0, 5);
			strncpy(temp, p, ucnt);
			sscanf(temp, "c%d", &ctemp);
			iss->mlevel.level_category[ctemp] = 1;
		}
	}
	if (cate && strlen(cate) )
		free(cate);
	
	for (i = 0; i < 255; i++) {
		if (iss->mlevel.level_category[i] == 1)
			printf("category[%d] == 1\n", i);
	}

	return 0;
}

int main(int argc, char **argv)
{
	struct inode_security_struct iss;
	int i;

	iss.mlevel.level_type = 0;
	iss.mlevel.level_value = 0;
	iss.ilevel.level_value = 0;
	for (i = 0; i < 255; i++) 
		iss.mlevel.level_category[i] = 0;

	if (argc < 2 || argv[1] == NULL) {
		printf("need arg\n");
		exit(-1);
	}

	context_to_iss(&iss, (const void **)&argv[1], strlen(argv[1]));
}
