#include <stdio.h>
#include <unistd.h>
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

//__syscall2(int, mac_task_ctl, int, cmd, struct task_security_struct *, tss);

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

int main(int argc, char **argv)
{
	int i = 0;

	init_tss(&tss1);
	init_tss(&tss2);

	printf("pid %d\n", getpid());
	i = syscall(__NR_mac_task_ctl, 0, 0, &tss2);
	if (i < 0) {
		perror("1get error");
		return -1;
	} else 
		display_tss(&tss2);
	
	if (argc > 1 && argv[1]) {
		int pid = strtol(argv[1], NULL, 0);
		printf("to get attr of pid %d\n", pid);
		i = syscall(__NR_mac_task_ctl, 0, pid, &tss1);
		if (i < 0) {
			perror("get task attr error");
			return -1;
		} else
			display_tss(&tss1);
	}

	return 0;
}
