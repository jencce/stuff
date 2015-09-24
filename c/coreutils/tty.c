#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>

int main(int argc, char **argv)
{
	char *ts = NULL;
	int c, quiet = 0;
	struct option long_options[5] = {
		{"help", no_argument, 0, 0},
		{"version", no_argument, 0, 1},
		{"silent", no_argument, 0, 's'},
		{"quiet", no_argument, 0, 's'},
		{0, 0, 0, 0}
	};

	while ((c = getopt_long(argc, argv, "s", long_options, NULL)) != -1) {
		switch (c) {
		case 0:
			printf("usage: tty [options]\n");
			return 0;
		case 1:
			printf("tty v0.1\n");
			return 0;
		case 's':
			quiet = 1;
			break;
		case '?':
			printf("tty --help for more help\n");
			return 2;
		}
	}

	if (optind < argc)
		printf("extra argument: %s\n", argv[optind]);

	ts = ttyname(STDIN_FILENO);
	if (ts == NULL) {
		perror("ttyname:");
		if (errno == ENOTTY)
			return 1;
		return 3;
	}

	if (quiet == 0)
		printf("%s\n", ts);

	return 0;
}
