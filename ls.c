#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <stdlib.h>

#define _GNU_SOURCE

struct ls_param {
	int all;
	int long_list;
};

int init_param(struct ls_param *params)
{
	params->all = 0;
	params->long_list = 0;
}

int parse_params(int argc, char **argv, struct ls_param *params)
{
	int c;

	struct option longopts[] = {
		{ "all",		no_argument,		0, 'a'	},
		{ "almost-all",		no_argument,		0, 'A'	},
		{ "author",		no_argument,		0, 0	},
		{ "escape",		no_argument,		0, 'b'	},
		{ "block-size",		required_argument,	0, 0	},
		{ "ignore-backups",	no_argument,		0, 'B'	},
		{ "color",		optional_argument,	0, 0	},
		{ "directory",		no_argument,		0, 'd'	},
		{ "dired",		no_argument,		0, 'D'	},
		{ "classify",		no_argument,		0, 'F'	},
		{ "file-type",		no_argument,		0, 0	},
		{ "format",		required_argument,	0, 0	},
		{ "full-time",		no_argument,		0, 0	},
		{ "group-dir-first",	no_argument,		0, 0	},
		{ "no-group",		no_argument,		0, 'G'	},
		{ "human-readable",	no_argument,		0, 'h'	},
		{ "si",			no_argument,		0, 0	},
		{ "derefrence-cmdline",	no_argument,		0, 'H'	},
		{ "hide",		required_argument,	0, 0	},
		{ "indicator-style",	required_argument,	0, 0	},
		{ "inode",		no_argument,		0, 'i'	},
		{ "ignore",		required_argument,	0, 'I'	},
		{ "derefrence",		no_argument,		0, 'L'	},
		{ "numeric-ids",	no_argument,		0, 'n'	},
		{ "literal",		no_argument,		0, 'N'	},
		{ "hide-ctl-ch",	no_argument,		0, 'q'	},
		{ "show-ctl-ch",	no_argument,		0, 0	},
		{ "quote-name",		no_argument,		0, 'Q'	},
		{ "quoting-style",	required_argument,	0, 0	},
		{ "reverse",		no_argument,		0, 'r'	},
		{ "recursive",		no_argument,		0, 'R'	},
		{ "size",		no_argument,		0, 's'	},
		{ "sort",		required_argument,	0, 0	},
		{ "time",		required_argument,	0, 0	},
		{ "time-style",		required_argument,	0, 0	},
		{ "tabsize",		required_argument,	0, 'T'	},
		{ "width",		required_argument,	0, 'w'	},
		{ "content",		no_argument,		0, 'Z'	},
		{ "version",		no_argument,		0, 0	},
		{ "help",		no_argument,		0, 0	},
		{ 0,		0,			0, 0	}
	};

	char optstring[] = "aAbcCdDfFgGhHi:I:klmopqQSrRstT:uUvw:xXZ1";

	while (1) {
		c = getopt_long (argc, argv, optstring, longopts, NULL);
		if (c == -1)
			break;

		switch (c) {
		case '0':
			printf("option: %s\n", longopts[0].name);
			break;
		case 'a':
			params->all = 1;
			break;
		case 'l':
			params->long_list = 1;
			break;
		default:
			printf("bad parameters\n");
			#ifdef LS_DEBUG
			printf("getopt returned %c\n", c);
			#endif
			return -1;
		}
	}

	return 0;
}

void file_long_list(char *dir, struct stat *stat)
{
	char s[11];
	char times[20];
	struct passwd *pwd;
	struct group *grp;

	memset(s, 0, 10);

	if (S_ISREG(stat->st_mode))
		s[0] = '-';
	if (S_ISBLK(stat->st_mode))
		s[0] = 'b';
	if (S_ISDIR(stat->st_mode))
		s[0] = 'd';
	if (S_ISCHR(stat->st_mode))
		s[0] = 'c';
	if (S_ISLNK(stat->st_mode))
		s[0] = 'l';
	if (S_ISSOCK(stat->st_mode))
		s[0] = 's';
	if (S_ISFIFO(stat->st_mode))
		s[0] = 'p';
	if (s[0] == 0)
		s[0] = '?';
	
	if (stat->st_mode & S_IRUSR)
		s[1] = 'r';
	else
		s[1] = '-';
	if (stat->st_mode & S_IWUSR)
		s[2] = 'w';
	else
		s[2] = '-';
	if (stat->st_mode & S_IXUSR)
		s[3] = 'x';
	else
		s[3] = '-';
	
	if (stat->st_mode & S_IRGRP)
		s[4] = 'r';
	else
		s[4] = '-';
	if (stat->st_mode & S_IWGRP)
		s[5] = 'w';
	else
		s[5] = '-';
	if (stat->st_mode & S_IXGRP)
		s[6] = 'x';
	else
		s[6] = '-';

	if (stat->st_mode & S_IROTH)
		s[7] = 'r';
	else
		s[7] = '-';
	if (stat->st_mode & S_IWOTH)
		s[8] = 'w';
	else
		s[8] = '-';
	if (stat->st_mode & S_IXOTH)
		s[9] = 'x';
	else
		s[9] = '-';

	s[10] = '\0';

	pwd = getpwuid(stat->st_uid);
	grp = getgrgid(stat->st_gid);

	strftime(times, 20, "%b %d %Y", localtime(&stat->st_mtime));

	printf("%s %d %s %s %ld %s %s\n", s, stat->st_nlink, pwd->pw_name,
		grp->gr_name, stat->st_size, times, dir);
}

int list_dir(char *dir, struct ls_param *params)
{
	struct stat buf;
	int ret;

	#ifdef LS_DEBUG
	printf("dir %s\n", dir);
	#endif

	ret = stat(dir, &buf);
	if (ret != 0) {
		printf("stat error: %s\n", strerror(errno));
		return -1;
	}

	if (S_ISREG(buf.st_mode)) {
		if (params->long_list)
			file_long_list(dir, &buf);
		else
			printf("%s ", dir);

		return 0;
	}

	if (S_ISDIR(buf.st_mode)) {
		int n = 0, i = 0;
		static int rflag = 0;
		struct dirent **namelist;

		if (strncmp(dir, ".", strlen(dir) > 1 ? strlen(dir) : 1) == 0
			&& rflag) {

			if (params->long_list)
				file_long_list(dir, &buf);
			else if (params->all)
				printf("%s ", dir);

			return 0;
		}

		if (strncmp(dir, "..", strlen(dir) > 2 ? strlen(dir) : 2) == 0
			&& rflag) {

			if (params->long_list)
				file_long_list(dir, &buf);
			else if (params->all)
				printf("%s ", dir);

			return 0;
		}

		if (dir[0] == '.' && rflag && params->all == 0)
			return;

		n = scandir(dir, &namelist, 0, alphasort);
		if (n < 0) {
			perror("scandir");
			free(namelist);
			rflag = 1;
			return -1;
		} else {
			rflag = 1;
			while (i < n) {
				list_dir(namelist[i]->d_name, params);
				free(namelist[i]);
				i++;
			}
			free(namelist);
		}
	}

	if (params->long_list == 0)
		printf("\n");

	return 0;
}

int main(int argc, char **argv)
{
	int list;
	int ret = 0;
	struct ls_param params;
	char *target;

	init_param(&params);
	parse_params(argc, argv, &params);

	while (argv[optind]) {
		#ifdef LS_DEBUG
		printf("argv[%d]: %s\n", optind, argv[optind]);
		#endif
		ret = list_dir(argv[optind], &params);
		optind++;
	}

	if (argv[optind] == NULL)
		ret = list_dir(".", &params);

	return ret;
}
