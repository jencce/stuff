#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/sysinfo.h>

int main(int argc, char **argv)
{
	int nr = get_nprocs();
	int c, ign, all = 0;

	struct option opts[5] = {
		{"version", 0, 0, 0},
		{"help", 0, 0, 1},
		{"all", 0, 0, 2},
		{"ignore", 1, 0, 3},
		{0, 0, 0, 0}
	};

	while ((c = getopt_long(argc, argv, "", opts, NULL)) != -1) {
		switch (c) {
		case 0:
			printf("%s v0.1\n", argv[0]);
			return 0;
		case 1:
			printf("%s --version|--help|--all|--ignore=N\n", argv[0]);
			return 0;
		case 2:
			all = 1;
			break;
		case 3:
			ign = atoi(optarg);
			break;
		case '?':
			printf("wrong option, %s --help\n", argv[0]);
			return 1;
		}
	}

	if (all == 1)
		nr = get_nprocs_conf();
	nr = nr - ign;
	if (nr <= 0)
		nr = 1;

	printf("%d\n", nr);

	return 0;
}
