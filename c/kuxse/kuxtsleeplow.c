#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/syscall.h>

#define MAC_CAT_MAX 256
#define __NR_mac_task_ctl 349

/* Mandatory access controll struct  */
struct mac_level {
	int level_type;    /* type */
	int level_value;
	int level_flag;
	int level_catsum;
	int level_category[MAC_CAT_MAX];
};

/* Intigrety level struct */
struct intigrety_level {
	int level_value;
};

struct task_security_struct {
	struct mac_level mlevel;
	struct intigrety_level ilevel;
};

void init_tss(struct task_security_struct *tss)
{
	int i;
	tss->mlevel.level_type = 0;
	tss->mlevel.level_value = 0;
	tss->mlevel.level_catsum = 0;
	tss->mlevel.level_flag = 0;
	for (i = 0; i < MAC_CAT_MAX; i++)
		tss->mlevel.level_category[i] = 0;

	tss->ilevel.level_value = 0;
}

struct task_security_struct tss1;
struct task_security_struct tss2;

void display_tss(struct task_security_struct *tss)
{
	int i;
	
	printf("multi level security type: %d\n", tss->mlevel.level_type);
	printf("                    value: %d\n", tss->mlevel.level_value);
	
	printf("                 category: ");
	for (i = 0; i < 255; i++)
		if (tss->mlevel.level_category[i] == 1)
			printf("c%d ", i);
	printf("\n");
	printf("multi level integrity value: %d\n", tss->ilevel.level_value);
}

void gettaskcon()
{
	int i = 0;

	init_tss(&tss2);

	i = syscall(__NR_mac_task_ctl, 0, 0, &tss2);
	if (i < 0) {
		perror("gettaskcon error");
		exit(-1);
	} else 
		display_tss(&tss2);
}

int settaskcon() 
{
	int i;

	init_tss(&tss1);
	tss1.mlevel.level_type = 1;
	tss1.mlevel.level_value = 0;
	tss1.mlevel.level_catsum = 1;
	tss1.mlevel.level_flag = 0;
	for (i = 0; i < MAC_CAT_MAX; i++)
		tss1.mlevel.level_category[i] = 0;

	tss1.mlevel.level_category[4] = 1;
	tss1.ilevel.level_value = 8;

	i = syscall(__NR_mac_task_ctl, 1, 0, &tss1);
	if (i < 0) {
		perror("settaskcon error");
		return -1;
	}

	return 0;
}

int main()
{
	printf("pid: %d\n", getpid());
	printf("before change:\n");
	gettaskcon();
	if (settaskcon() != 0)
		return -1;
	printf("\n\nafter change:\n");
	gettaskcon();
	sleep(20);
	return 0;
}
