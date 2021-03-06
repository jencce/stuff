#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define MAC_CAT_MAX 256
#define __NR_mac_file_ctl 350

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

struct inode_security_struct {
	struct mac_level mlevel;
	struct intigrety_level ilevel;
	int task_sid;
	int initialized;
};

//__syscall2(int, mac_task_ctl, int, cmd, struct inode_security_struct *, tss);

void init_tss(struct inode_security_struct *tss)
{
	int i;
	tss->mlevel.level_type = 0;
	tss->mlevel.level_value = 0;
	tss->mlevel.level_catsum = 0;
	tss->mlevel.level_flag = 1;
	for (i = 0; i < MAC_CAT_MAX; i++)
		tss->mlevel.level_category[i] = 0;

	tss->ilevel.level_value = 0;
	tss->task_sid = 0;
	tss->initialized = 0;
}

struct inode_security_struct tss1;
struct inode_security_struct tss2;

int main(int argc, char **argv)
{
	int i = 0;

	init_tss(&tss1);
	init_tss(&tss2);

	if (argc < 2 || argv[1] == NULL) {
		printf("filename needed\n");
		return -1;
	}

	i = syscall(__NR_mac_file_ctl, 0, argv[1], &tss2);
	if (i < 0) {
		perror("1get error");
		return -1;
	} else {
		printf("mv %d, iv %d\n", tss2.mlevel.level_value,
			tss2.ilevel.level_value);
	}

	tss1.mlevel.level_type = 0;
	tss1.mlevel.level_value = 5;
	tss1.mlevel.level_catsum = 0;
	tss1.mlevel.level_flag = 1;
	for (i = 0; i < MAC_CAT_MAX; i++)
		tss1.mlevel.level_category[i] = 0;

	tss1.ilevel.level_value = 5;
	
	syscall(__NR_mac_file_ctl, 1, argv[1], &tss1);

	init_tss(&tss2);
	i = syscall(__NR_mac_file_ctl, 0, argv[1], &tss2);
	if (i < 0) {
		perror("2get error");
		return -1;
	} else {
		printf("mv %d, iv %d\n", tss2.mlevel.level_value,
			tss2.ilevel.level_value);
	}

}
