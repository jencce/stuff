#include <sys/utsname.h>
#include <getopt.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

enum {
	PRINT_ALL,
	PRINT_KERNEL_NAME,
	PRINT_NODENAME,
	PRINT_KERNEL_RELEASE,
	PRINT_KERNEL_VERSION,
	PRINT_MACHINE,
	PRINT_PROCESSOR,
	PRINT_HARDWARE_PLATFORM,
	PRINT_OPERATING_SYSTEM,
	PRINT_VERSION,
	PRINT_HELP
};

int print_flag = PRINT_KERNEL_NAME;

int parse_params(int argc, char **argv, int *flag)
{
	int c;

	struct option longopts[] = {
		{ "all",		no_argument,	NULL, 'a'	},
		{ "kernel-name",	no_argument,	NULL, 's'	},
		{ "nodename",		no_argument,	NULL, 'n'	},
		{ "kernel-release",	no_argument,	NULL, 'r'	},
		{ "kernel-version",	no_argument,	NULL, 'v'	},
		{ "machine",		no_argument,	NULL, 'm'	},
		{ "processor",		no_argument,	NULL, 'p'	},
		{ "hardware-platform",	no_argument,	NULL, 'i'	},
		{ "operating-system",	no_argument,	NULL, 'o'	},
		{ "version",		no_argument,	NULL, 0	},
		{ "help",		no_argument,	NULL, 1	},
		{ 0,	0,		NULL, 0	}
	};

	char optstring[] = "aimnoprv";
	int opt_index = 0;

	while (1) {
		c = getopt_long (argc, argv, optstring, longopts, &opt_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
			*flag = PRINT_VERSION;
			printf("version: 0.1\n");
			break;
		case 1:
			*flag = PRINT_HELP;
			printf("option: %s ", longopts[opt_index].name);
			if (optarg)
				printf("with arg %s", optarg);
			printf("\n");
			break;
		case 'a':
			*flag = PRINT_ALL;
			break;
		case 'i':
			*flag = PRINT_HARDWARE_PLATFORM;
			break;
		case 'm':
			*flag = PRINT_MACHINE;
			break;
		case 'n':
			*flag = PRINT_NODENAME;
			break;
		case 'o':
			*flag = PRINT_OPERATING_SYSTEM;
			break;
		case 'p':
			*flag = PRINT_PROCESSOR;
			break;
		case 'r':
			*flag = PRINT_KERNEL_RELEASE;
			break;
		case 'v':
			*flag = PRINT_KERNEL_VERSION;
			break;
		default:
			printf("bad parameters\n");
			exit(-1);
		}
	}
	return 0;
}
int main(int argc, char **argv)
{
	struct utsname utsn;
	int ret;

	parse_params(argc, argv, &print_flag);

	ret = uname(&utsn);
	if (ret != 0) {
		printf("uname error: %s\n", strerror(errno));
		return -1;
	}

	switch (print_flag) {
	case PRINT_ALL:
		printf("%s %s %s %s %s %s %s GNU/Linux\n", utsn.sysname,
			utsn.nodename, utsn.release, utsn.version,
				utsn.machine, utsn.machine, utsn.machine);
		break;
	default:
		break;
	}

	return 0;
}
