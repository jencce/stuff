#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <sys/types.h>

int print_group_list(uid_t uid) {
	int ret, cnt, i;
	gid_t *group_list = NULL;
	struct passwd *pwd = NULL;

	pwd = getpwuid(uid);
	if (pwd == NULL) {
		perror("getpwuid:");
		return -1;
	}
	//printf("%s", pwd->pw_name);

	cnt = getgroups(0, group_list);
	if (cnt > 0) {
		group_list = (gid_t *)malloc(sizeof(gid_t) * cnt);
		if (group_list == NULL) {
			perror("malloc:");
			return -1;
		}
	}

	ret = getgroups(cnt, group_list);
	if (ret == -1) {
		perror("getgroups:");
		return -1;
	}

	for (i = 0; i < cnt; i++) {

		struct group *grp = NULL;

		grp = getgrgid(group_list[i]);
		if (grp != NULL) {
			printf(" %s", grp->gr_name);
		} else {
			perror("getgrgid:");
			return -1;
		}
	}
	printf("\n");
	return 0;
}

int main(int argc, char ** argv)
{
	int ret, c;
	struct option long_options[3] = {
		{"help", no_argument, 0, 0},
		{"version", no_argument, 0, 1},
		{0, 0, 0, 0}
	};

	while ((c = getopt_long_only(argc, argv, "", long_options, NULL)) != -1) {
		switch (c) {
		case 0:
			printf("usage: groups [options] [usernames]\n");
			return 0;
			break;
		case 1:
			printf("groups v0.1\n");
			return 0;
			break;
		case '?':
			printf("groups --help for more help\n");
			return 0;
			break;
		}
	}
	
	if (optind == argc) {

		/* no more arguments, print groups of calling process*/
		print_group_list(getuid());

	} else {
		while (optind < argc) {

			struct passwd *pwd = NULL;
			errno = 0;
			pwd = getpwnam(argv[optind]);
			if (pwd == NULL && errno == 0) {
				printf("groups: no such user: %s\n", argv[optind++]);
				return -1;
			}
			optind++;

			print_group_list(pwd->pw_uid);
		}
	}

	return 0;
}
