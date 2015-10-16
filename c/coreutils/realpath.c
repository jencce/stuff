#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <limits.h>
#include <sys/sysinfo.h>

int main(int argc, char **argv)
{
	int c;
	char * path = NULL;

	struct option opts[13] = {
		{"version", 0, 0, 0},
		{"help", 0, 0, 1},
		{"canonicalize-existing", 0, 0, 'e'},
		{"canonicalize-missing", 0, 0, 'm'},
		{"logical", 0, 0, 'L'},
		{"physical", 0, 0, 'P'},
		{"quiet", 0, 0, 'q'},
		{"no-symlinks", 0, 0, 's'},
		{"strip", 0, 0, 's'},
		{"zero", 0, 0, 'z'},
		{"relative-to", 1, 0, 2},
		{"relative-base", 1, 0, 3},
		{0, 0, 0, 0}
	};

	while ((c = getopt_long(argc, argv, "emLPqsz", opts, NULL)) != -1) {
		switch (c) {
		case 0:
			printf("%s v0.1\n", argv[0]);
			return 0;
		case 1:
			printf("%s --version|--help|-m|-e|... <file>\n", argv[0]);
			return 0;
		case 2:
			break;
		case 3:
			break;
		case '?':
			printf("wrong option, %s --help\n", argv[0]);
			return 1;
		}
	}

	if (optind == argc) {
		printf("need a file, %s --help\n", argv[0]);
		return 1;
	} else {
		while (optind < argc) {
			path = realpath(argv[optind], NULL);
			if (path == NULL) {
				printf("%s: %s\n", argv[optind], strerror(errno));
			} else
				printf("%s\n", path);
			optind++;
		}
	}

	// canonicalize_file_name
	return 0;
}
