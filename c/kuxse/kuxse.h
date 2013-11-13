#include <stdio.h>
#include <unistd.h>
#include <errno.h>
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


void display_tss(struct task_security_struct *tss)
{
	int i;
	
	printf("multi level security type: %d,value: %d,category:",
		tss->mlevel.level_type, tss->mlevel.level_value);
	for (i = 0; i < 255; i++)
		if (tss->mlevel.level_category[i] == 1)
			printf("c%d ", i);
	printf("\n");

	printf("multi level integrity value: %d\n", tss->ilevel.level_value);
}

void get_task_xattr(pid_t pid)
{
	int i = 0;
	struct task_security_struct tss2;

	if (pid < 0) {
		perror("get_task_xattr pid not valid");
		_exit(-1);
	}

	init_tss(&tss2);
	i = syscall(__NR_mac_task_ctl, 0, pid, &tss2);
	if (i < 0) {
		printf("get pid %d 's xattr error(0 means current): %s\n",
			pid, strerror(errno));
		_exit(-1);
	} else {
		if (pid == 0)
			printf("xattr of current:\n", pid);
		else
			printf("xattr of pid %d:\n", pid);
		display_tss(&tss2);
	}
}

int set_task_xattr(struct task_security_struct *tss)
{
	int i = 0;

	i = syscall(__NR_mac_task_ctl, 1, 0, tss);
	if (i < 0) {
		perror("set current's xattr error");
		return -1;
	} 
}
