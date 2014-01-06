#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <stdlib.h>
#include <libgen.h>
#include <locale.h>
#include <termios.h>

#ifdef SELINUX
#include <selinux/selinux.h>
#endif

#define _GNU_SOURCE

int max_filesize = 0;
int max_links = 0;
int longest_usr = 0;
int longest_grp = 0;
int have_secon = 0;
int block_size_int = 0;
char block_size_unit[3];
int list_size = 0;

struct ls_param {
	int all;
	int almost;
	int long_list;
	int recursive;
	int dot;
	int blk_size;
};

void init_param(struct ls_param *params)
{
	params->all = 0;
	params->almost = 0;
	params->long_list = 0;
	params->recursive = 0;
	params->dot = 0;
	params->blk_size = 0;
}

int parse_params(int argc, char **argv, struct ls_param *params)
{
	int c;

	struct option longopts[] = {
		{ "all",		no_argument,	NULL, 'a'	},
		{ "almost-all",		no_argument,	NULL, 'A'	},
		{ "author",		no_argument,	NULL, 0	},
		{ "escape",		no_argument,	NULL, 'b'	},
		{ "block-size",		required_argument,NULL, 1	},
		{ "ignore-backups",	no_argument,	NULL, 'B'	},
		{ "color",		optional_argument,NULL, 0	},
		{ "directory",		no_argument,	NULL, 'd'	},
		{ "dired",		no_argument,	NULL, 'D'	},
		{ "classify",		no_argument,	NULL, 'F'	},
		{ "file-type",		no_argument,	NULL, 0	},
		{ "format",		required_argument,NULL, 0	},
		{ "full-time",		no_argument,	NULL, 0	},
		{ "group-dir-first",	no_argument,	NULL, 0	},
		{ "no-group",		no_argument,	NULL, 'G'	},
		{ "human-readable",	no_argument,	NULL, 'h'	},
		{ "si",			no_argument,	NULL, 0	},
		{ "derefrence-cmdline",	no_argument,	NULL, 'H'	},
		{ "hide",		required_argument,NULL, 0	},
		{ "indicator-style",	required_argument,NULL, 0	},
		{ "inode",		no_argument,	NULL, 'i'	},
		{ "ignore",		required_argument,NULL, 'I'	},
		{ "derefrence",		no_argument,	NULL, 'L'	},
		{ "numeric-ids",	no_argument,	NULL, 'n'	},
		{ "literal",		no_argument,	NULL, 'N'	},
		{ "hide-ctl-ch",	no_argument,	NULL, 'q'	},
		{ "show-ctl-ch",	no_argument,	NULL, 0	},
		{ "quote-name",		no_argument,	NULL, 'Q'	},
		{ "quoting-style",	required_argument,NULL, 0	},
		{ "reverse",		no_argument,	NULL, 'r'	},
		{ "recursive",		no_argument,	NULL, 'R'	},
		{ "size",		no_argument,	NULL, 's'	},
		{ "sort",		required_argument,NULL, 0	},
		{ "time",		required_argument,NULL, 0	},
		{ "time-style",		required_argument,NULL, 0	},
		{ "tabsize",		required_argument,NULL, 'T'	},
		{ "width",		required_argument,NULL, 'w'	},
		{ "content",		no_argument,	NULL, 'Z'	},
		{ "version",		no_argument,	NULL, 0	},
		{ "help",		no_argument,	NULL, 0	},
		{ 0,	0,		NULL, 0	}
	};

	char optstring[] = "aAbcCdDfFgGhHi:I:klmopqQSrRstT:uUvw:xXZ1";
	int opt_index = 0;

	while (1) {
		c = getopt_long (argc, argv, optstring, longopts, &opt_index);
		if (c == -1)
			break;

		switch (c) {
		case 0:
			printf("option: %s ", longopts[opt_index].name);
			if (optarg)
				printf("with arg %s", optarg);
			printf("\n");
			break;
		case 1: /* --block-size */
			#ifdef LS_DEBUG
			printf("option: %s ", longopts[opt_index].name);
			if (optarg)
				printf("with arg %s", optarg);
			printf("\n");
			#endif
			if (optarg == NULL) {
				printf("ls: invalid --block-size"
					"argument '%s'\n", optarg);
				exit(-1);
			}
			params->blk_size = 1;
			memset(block_size_unit, 0, sizeof(block_size_unit));
			if (isdigit(optarg[0])) {
				int ret = sscanf(optarg, "%d%[mMkKgGtTPEZYB]",
					&block_size_int, block_size_unit);
				if (ret < 2 && block_size_int == 0) {
					printf("ls: invalid --block-size"
						"argument '%s'\n", optarg);
					exit(-1);
				}
			}
			else {
				int ret = sscanf(optarg, "%[mMkKgGtTPEZYB]",
					block_size_unit);
				if (ret < 1) {
					printf("ls: invalid --block-size"
						"argument '%s'\n", optarg);
					exit(-1);
				}
			}
			list_size = get_list_size(params);
			//printf("%d %s\n", block_size_int, block_size_unit);
			break;
		case 'a':
			params->all = 1;
			break;
		case 'A':
			params->almost = 1;
			break;
		case 'l':
			params->long_list = 1;
			break;
		case 'R':
			params->recursive = 1;
			break;
		default:
			printf("bad parameters\n");
			#ifdef LS_DEBUG
			printf("getopt returned %c\n", c);
			#endif
			exit(-1);
		}
	}

	return 0;
}

int get_list_size(struct ls_param *params)
{
	int cnt = 0;

	if (params->blk_size != 1)
		return -1;

	if (block_size_int == 0)
		cnt = 1;
	else
		cnt = block_size_int;

	switch (block_size_unit[0]) {
		case 0	:
			if (cnt > 1)
				return cnt;
			else {
				printf("ls: invalid --block-size argument"
					"'%d%s'\n", block_size_int,
					block_size_unit);
				exit(-1);
			}
		case 'k':
		case 'K':
			if (block_size_unit[1] == 'B')
				return cnt*1000;
			else
				return cnt*1024;
		case 'm':
		case 'M':
			if (block_size_unit[1] == 'B')
				return cnt*1000*1000;
			else
				return cnt*1024*1024;
		case 'g':
		case 'G':
			if (block_size_unit[1] == 'B')
				return cnt*1000*1000*1000;
			else
				return cnt*1024*1024*1024;
		case 't':
		case 'T':
		case 'P':
		case 'E':
		case 'Z':
		case 'Y':
			printf("ls: --block-size argument '%d%s' too large\n",
				block_size_int, block_size_unit);
			exit(-1);
		default :
			printf("ls: invalid --block-size argument '%d%s'\n",
				block_size_int, block_size_unit);
			exit(-1);
	}
}

void file_llist(char *dir, struct stat *stat, int secon)
{
	char s[12];
	char times[20];
	struct passwd *pwd;
	struct group *grp;
	char *tmp = NULL;

	#ifdef LS_DEBUG
	printf("file_llist %s\n", dir);
	#endif

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
	if (stat->st_mode & S_IXUSR) {
		if (stat->st_mode & S_ISUID) 
			s[3] = 's';
		//else if (stat->st_mode & S_ISVTX) 
		//	s[3] = 't';
		else
			s[3] = 'x';
	}
	else {
		if (stat->st_mode & S_ISUID) 
			s[3] = 'S';
		//else if (stat->st_mode & S_ISVTX) 
		//	s[3] = 'T';
		else
			s[3] = '-';
	}
	
	if (stat->st_mode & S_IRGRP)
		s[4] = 'r';
	else
		s[4] = '-';
	if (stat->st_mode & S_IWGRP)
		s[5] = 'w';
	else
		s[5] = '-';
	if (stat->st_mode & S_IXGRP) {
		if (stat->st_mode & S_ISGID) 
			s[6] = 's';
		//else if (stat->st_mode & S_ISVTX) 
		//	s[6] = 't';
		else
			s[6] = 'x';
	}
	else {
		if (stat->st_mode & S_ISGID) 
			s[6] = 'S';
		//else if (stat->st_mode & S_ISVTX) 
		//	s[6] = 'T';
		else
			s[6] = '-';
	}

	if (stat->st_mode & S_IROTH)
		s[7] = 'r';
	else
		s[7] = '-';
	if (stat->st_mode & S_IWOTH)
		s[8] = 'w';
	else
		s[8] = '-';
	if (stat->st_mode & S_IXOTH) {
		if (stat->st_mode & S_ISVTX) 
			s[9] = 't';
		else
			s[9] = 'x';
	}
	else {
		if (stat->st_mode & S_ISVTX) 
			s[9] = 'T';
		else
			s[9] = '-';
	}

	if (secon) {
		s[10] = '.';
		s[11] = '\0';
	}
	else {
		if (have_secon) {
			s[10] = ' ';
			s[11] = '\0';
		} else
			s[10] = '\0';
	}

	pwd = getpwuid(stat->st_uid);
	grp = getgrgid(stat->st_gid);

	printf("%s ", s);

	memset(times, 0, 20);
	snprintf(times, 20, "%d", max_links);
	#ifdef LS_DEBUG
	printf("maxlinks %d, %s, %dem", max_links, times, strlen(times));
	#endif
	printf("%1$*2$d ", stat->st_nlink, strlen(times));

	printf("%1$-*2$s ", pwd->pw_name, longest_usr);
	printf("%1$-*2$s ", grp->gr_name, longest_grp);

	#ifdef LS_DEBUG
	//printf("mf%d, %s, %dfm", max_filesize, times, strlen(times));
	printf("list_size %d\n", list_size);
	#endif
	if (list_size == 0) {
		memset(times, 0, 20);
		snprintf(times, 20, "%d", max_filesize);
		printf("%1$*2$ld ", stat->st_size, strlen(times));
	}
	else {
		memset(times, 0, 20);
		snprintf(times, 20, "%d", max_filesize/list_size + 1);
		printf("%1$*2$ld ", stat->st_size/list_size + 1, strlen(times));
	}

	memset(times, 0, 20);
	strftime(times, 20, "%b %e", localtime(&stat->st_mtime));
	printf("%s ", times);

	memset(times, 0, 20);
	time_t now;
	now = time(NULL);
	#if 0
	struct tm tmnow;
	struct tm tmmtime;

	localtime_r(&now, &tmnow);
	localtime_r(&stat->st_mtime, &tmmtime);

	if ((tmnow.tm_year == tmmtime.tm_year &&
	     tmnow.tm_mon - tmmtime.tm_mon < 6) ||
		(tmnow.tm_year == tmmtime.tm_year &&
		tmnow.tm_mon - tmmtime.tm_mon == 6 &&
		tmnow.tm_mday - tmmtime.tm_mday < 0) ||
		(tmnow.tm_year == tmmtime.tm_year &&
		tmnow.tm_mon - tmmtime.tm_mon == 6 &&
		tmnow.tm_mday - tmmtime.tm_mday == 0 &&
		tmnow.tm_hour - tmmtime.tm_hour > 0))
	#endif
	/* A Gregorian year has 365.2425 * 24 * 60 * 60 ==
	         31556952 seconds on the average. */
	if (now - stat->st_mtime < 31556952 / 2)
		strftime(times, 20, "%R", localtime(&stat->st_mtime));
	else
		strftime(times, 20, "%Y", localtime(&stat->st_mtime));
	#ifdef LS_DEBUG
	printf("time%ld, now %ldit", stat->st_mtime, now);
	#endif
	printf("%1$*2$s ", times, 5);

	tmp = strdup(dir);
	if (tmp) {
		if (s[0] != 'l')
			printf("%s\n", basename(tmp));
		else {
			char lbuf[BUFSIZ];
			memset(lbuf, 0, BUFSIZ);

			ssize_t st = readlink(dir, lbuf, BUFSIZ);
			if (st != -1)
				printf("%s -> %s\n", basename(tmp), lbuf);
			#ifdef LS_DEBUG
			else
				printf("readlink error\n");
			#endif
		}
		free(tmp);
	}
}

char out_buff[BUFSIZ][BUFSIZ];
static int obindex;

void init_ob()
{
	int i = 0;
	obindex = 0;
	while (i < BUFSIZ) 
		memset(out_buff[i++], '\0', BUFSIZ);
}

void file_slist(char *bname)
{
	#if 0
	if (isatty(STDOUT_FILENO)) {
		sprintf(out_buff[obindex++], "%s", bname);
	} else {
		printf("%s\n", bname);
	}
	#else
	sprintf(out_buff[obindex++], "%s", bname);
	#endif
}

static int cmpstr(const void *s1, const void *s2)
{
	char *p1 = (char *)s1;
	char *p2 = (char *)s2;
	while (! isalpha(*p1))
		p1++;
	while (! isalpha(*p2))
		p2++;

	//return strcmp(*(char * const *)s1, *(char * const *)s2);
	return strcasecmp(p1, p2);
}

void sort_all(char **buff, int max)
{
	//qsort(out_buff, obindex, sizeof(out_buff[0]), cmpstr);
	qsort(buff, max, BUFSIZ, cmpstr);
}

void print_all()
{
	struct winsize ws;
	int ret = 0, z;
	int longest_item = 0;
	int tt_length = 0;
	int min_rows, max_rows, max_cols;

	if (obindex == 0)
		return;
	if (! isatty(STDOUT_FILENO)) {
		for (z = 0; z < obindex; z++)
			printf("%s\n", out_buff[z]);
		return;
	}

	ret = ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	if (ret != 0) {
		//fprintf(stderr, "ioctl:%s\n", strerror(errno));
		return;
	}
	#ifdef LS_DEBUG
	printf("row %d, col %d, oni %d\n", ws.ws_row, ws.ws_col, obindex);
	#endif

	for(ret = 0; ret < obindex; ret++) {
		int item_leng = strlen(out_buff[ret]);
		#ifdef LS_DEBUG
		printf("out_buff[%d]:%s,", ret, out_buff[ret]);
		printf("len %d\n", strlen(out_buff[ret]));
		#endif

		tt_length += item_leng;
		tt_length += 2;
		if (item_leng > longest_item)
			longest_item = item_leng;
		#ifdef LS_DEBUG
		printf("tt %d, li %d\n", tt_length, longest_item);
		#endif
	}

	min_rows = tt_length / ws.ws_col;
	if (min_rows == 0)
		min_rows = 1;
	max_rows = (longest_item + 2) * obindex / ws.ws_col + 1;
	max_cols = obindex / min_rows + 1;

	#ifdef LS_DEBUG
	printf("min/max rows:%d %d\n", min_rows, max_rows);
	#endif
	
	/* figure out the best rows number*/
	int best_row = 0;
	int col_len[max_cols];
	for (ret = min_rows; ret <= max_rows; ret++) {
		int smax_col_len = 0;
		int i, j;

		for (i = 0; i < max_cols; i++)
			col_len[i] = 0;
		int k = 0;
		for (i = 0 + ret; ; i = i + ret) {
			int max_col_len = 0;
			for (j = i - ret; j < i && j < obindex; j++) {
				int item_len = strlen(out_buff[j]);
				#ifdef LS_DEBUG
				//printf("item %d len %d\n", j, item_len);
				#endif
				if (max_col_len < item_len)
					max_col_len = item_len;
			}
			if (max_col_len == 0) {
				smax_col_len -= 2;
				break;
			}

			smax_col_len += max_col_len;
			smax_col_len += 2;
			#ifdef LS_DEBUG
			printf("col %d,len %d,", k, max_col_len);
			printf("smax_col_len %d\n", smax_col_len);
			#endif
			col_len[k++] = max_col_len;
		}
		if (smax_col_len < ws.ws_col) {
			#ifdef LS_DEBUG
			printf("best rows: %d, smcl %d\n", ret, smax_col_len);
			#endif
			best_row = ret;
			break;
		}
		/* min/max cols cal may not be right*/
		if (ret == max_rows && best_row == 0)
			max_rows++;
	}

	/* print according to best_row*/
	int i, j, k = 0;
	for (i = 0; i < best_row; i++) {
		for (j = i, k = 0; j < obindex; j = j + best_row) {
			//printf("%1$*2$s  ", out_buff[j], col_len[i]);
			printf("%-*s", col_len[k++], out_buff[j]);
			if (j + best_row < obindex)
				printf("  ");
		}
		printf("\n");
	}
}

int file_secon(char *item)
{
	#ifdef SELINUX
	security_context_t con;
	if (lgetfilecon(item, &con) > 0) {
		freecon(con);
		return 1;
	}
	#endif
	return 0;
}

int list_dir(char *dir, struct ls_param *params)
{
	struct stat buf;
	int ret;
	struct dirent **namelist;
	int n = 0, i = 0, j = 0, dirno = 0;
	char *dirs[BUFSIZ];
	int tt_blks = 0;
	int blk_size = 0;

	#ifdef LS_DEBUG
	printf("dir %s\n", dir);
	#endif
	max_filesize = 0;
	max_links = 0;
	have_secon = 0;

	if (params->recursive) {
		printf("%s:\n", dir);
	}

	ret = lstat(dir, &buf);
	if (ret != 0) {
		#ifdef LS_DEBUG
		printf("stat error:%s %s\n", dir, strerror(errno));
		#endif
	}

	if (! S_ISDIR(buf.st_mode)) {
		char *dir_copy = strdup(dir);
		char *bname = basename(dir_copy);
		if (params->long_list) {
			int secon = file_secon(dir);
			file_llist(dir, &buf, secon);
		} else
			file_slist(bname);
		return;
	}


	n = scandir(dir, &namelist, 0, alphasort);
	if (n < 0) {
		#ifdef LS_DEBUG
		fprintf(stderr, "scandr:%s\n", strerror(errno));
		#endif
		//free(namelist);
	} else {
		while (i < n) {
			char *item = (char *)malloc(BUFSIZ);
			char *dir_copy;
			char *bname;
			int ignore = 0;
			struct passwd *pwd;
			struct group *grp;

			sprintf(item, "%s/%s", dir,namelist[i]->d_name);
			dir_copy = strdup(item);
			bname = basename(dir_copy);

			ret = lstat(item, &buf);
			if (ret != 0) {
				#ifdef LS_DEBUG
				printf("stat error:%s %s\n", item, 
						strerror(errno));
				#endif
				i++;
				continue;
			}

			if (bname[0] == '.' && params->all == 0)
				ignore = 1;

			if (params->almost == 1) {
				if (strcmp(bname, ".") == 0 ||
					strcmp(bname, "..") == 0)
					ignore = 1;
				else
					ignore = 0;
			}

			if (ignore == 0) {
				pwd = getpwuid(buf.st_uid);
				grp = getgrgid(buf.st_gid);
				if (longest_usr < strlen(pwd->pw_name))
					longest_usr = strlen(pwd->pw_name);
				if (longest_grp < strlen(grp->gr_name))
					longest_grp = strlen(grp->gr_name);
			}

			#ifdef SELINUX
			security_context_t con;
			if (have_secon == 0 && ignore == 0
				&& getfilecon(item, &con) > 0) {
				freecon(con);
				have_secon = 1;
			}
			#endif

			if (S_ISDIR(buf.st_mode)) { 
				if (strcmp(bname, ".") != 0 &&
				    strcmp(bname, "..") != 0)
					dirno++;
			}

			if (buf.st_size > max_filesize && ignore == 0)
				max_filesize = buf.st_size;
		
			if (buf.st_nlink > max_links && ignore == 0)
				max_links = buf.st_nlink;

			if (ignore == 0) {
				#ifdef LS_DEBUG
				printf("%s xblks:%ld ttblks:%d\n",
					item, buf.st_blocks, tt_blks);
				#endif
				tt_blks += buf.st_blocks;
			}
			blk_size = buf.st_blksize;
			#ifdef LS_DEBUG
			printf("blksize:%d\n", blk_size);
			#endif
		
			free(namelist[i]);
			free(item);
			i++;
		}
		free(namelist);
	}

	if (params->long_list) {
		if (list_size == 0)
			printf("total %d\n", tt_blks/2);
		else
			printf("total %d\n", tt_blks*512/list_size);
	}

	for (i = 0; i < dirno; i++)
		while (dirs[i] == NULL)
			dirs[i] = (char *)malloc(BUFSIZ);

	i = 0;
	n = 0;
	n = scandir(dir, &namelist, 0, alphasort);
	if (n < 0) {
		#ifdef LS_DEBUG
		fprintf(stderr, "scandir:%s\n", strerror(errno));
		#endif
		//free(namelist);
		return -1;
	} 
	while (i < n) {
		char *item = (char *)malloc(BUFSIZ);
		sprintf(item, "%s/%s", dir,namelist[i]->d_name);
		#ifdef LS_DEBUG
		printf("item %s\n", item);
		#endif

		ret = lstat(item, &buf);
		if (ret != 0) {
			#ifdef LS_DEBUG
			printf("stat error:%s %s\n", item, strerror(errno));
			#endif
			i++;
			continue;
		}
	
		if (S_ISDIR(buf.st_mode)) {
			char *dir_copy = strdup(item);
			char *bname = basename(dir_copy);
			char *dir_copy1 = strdup(item);
			char *dname = dirname(dir_copy1);
	
			if (strcmp(bname, ".") == 0) {
				if (params->all) {
					if (params->long_list) {
						int secon = file_secon(item);
						file_llist(item, &buf, secon);
					} else
						file_slist(bname);
						//printf("%s  ", bname);
				}
				free(namelist[i]);
				free(item);
				i++;
				continue;
			}
	
			if (strcmp(bname, "..") == 0) {
				if (params->all) {
					if (params->long_list) {
						int secon = file_secon(item);
						file_llist(item, &buf, secon);
					} else
						file_slist(bname);
						//printf("%s  ", bname);
				}
				free(namelist[i]);
				free(item);
				i++;
				continue;
			}
	
			if (bname[0] == '.' && strlen(bname) > 1) {
				if (params->all || params->almost) {
					/*
			 		* show before recurse for directory
			 		*/
					if (params->long_list) {
						int secon = file_secon(item);
						file_llist(item, &buf, secon);
					} else {
						file_slist(bname);
						//printf("%s  ", bname);
					}
					if (params->recursive)
						strcpy(dirs[j++], item);
						//list_dir(item, params);
				}
				free(namelist[i]);
				free(item);
				i++;
				continue;
			}
	
			/*
			 * show before recurse for directory
			 */
			if (params->long_list) {
				int secon = file_secon(item);
				file_llist(item, &buf, secon);
			} else {
				file_slist(bname);
				//printf("%s  ", bname);
			}
			if (params->recursive)
				strcpy(dirs[j++], item);
				//list_dir(item, params);
	
			free(dir_copy);
			free(dir_copy1);
		}
		else {
			char *dir_copy = strdup(item);
			char *bname = basename(dir_copy);
			if (bname[0] == '.' && strlen(bname) > 1) {
				if (params->all || params->almost) {
					/*
			 		* show . start files
			 		*/
					if (params->long_list) {
						int secon = file_secon(item);
						file_llist(item, &buf, secon);
					} else
						file_slist(bname);
				}
				free(namelist[i]);
				free(item);
				i++;
				continue;
			}
			if (params->long_list) {
				int secon = file_secon(item);
				file_llist(item, &buf, secon);
			} else {
				char *s = strdup(item);
				file_slist(basename(s));
				//printf("%s  ", basename(s));
				free(s);
			}
		}
	
		free(namelist[i]);
		free(item);
		i++;
	}
	free(namelist);

	//if (params->long_list == 0)
	//printf("\n");

	for (i = 0; i < j; i++) {
		#ifdef LS_DEBUG
		printf("dirs[%d]: %s\n", i, dirs[i]);
		#endif
		print_all();
		printf("\n");
		init_ob();
		list_dir(dirs[i], params);
	}

	//for (i = 0; i < j; i++)
	//	free(dirs[i]);

	return 0;
}

int main(int argc, char **argv)
{
	int list;
	int ret = 0, i, mdirs = 0, nfiles = 0;
	struct ls_param params;
	char *target;
	struct stat buf;

	setlocale(LC_ALL, "");

	init_param(&params);
	parse_params(argc, argv, &params);

	if (argv[optind] == NULL) {
		have_secon = 0;
		ret = list_dir(".", &params);
		print_all();
	}

	/* take a glance at all argvs, tell files out from dirs.
	 * display the files as a group first, alphabetically default.
	 * then the dirs one by one is handled, both dir name and dir
	 * content are displayed aplhabetically default.
	 */

	init_ob();
	for (i = optind; i < argc; i++) {
		//printf("argv %d: %s\n", i, argv[i]);
		memset(&buf, 0, sizeof(struct stat));
		ret = lstat(argv[i], &buf);
		if (ret != 0) {
			#ifdef LS_DEBUG
			printf("stat error:%s %s\n", argv[i], strerror(errno));
			#endif
			continue;
		}

		if (! S_ISDIR(buf.st_mode)) {
			//printf("not dir %s\n", argv[i]);
			nfiles++;
			if (params.long_list) {
				int secon = file_secon(argv[i]);
				file_llist(argv[i], &buf, secon);
			} else
				file_slist(argv[i]);
		} else
			mdirs++;
	}

	sort_all((char **)out_buff, obindex);
	print_all();

	if (nfiles  > 0 && mdirs > 0)
		printf("\n");

	/* now the dirs */
	char tmpdirs[mdirs][BUFSIZ];
	int k = 0;

	for (i = 0; i < mdirs; i++)
		memset(tmpdirs[i], 0, BUFSIZ);

	for (i = optind; i < argc; i++) {
		//printf("argv %d: %s\n", i, argv[i]);
		memset(&buf, 0, sizeof(struct stat));
		ret = lstat(argv[i], &buf);
		if (ret != 0) {
			#ifdef LS_DEBUG
			printf("stat error:%s %s\n", argv[i], strerror(errno));
			#endif
			continue;
		}

		if (S_ISDIR(buf.st_mode)) {
			#ifdef LS_DEBUG
			printf("dir %d: %s\n", k, argv[i]);
			#endif
			sprintf(tmpdirs[k++], "%s", argv[i]);
		}
	}

	sort_all((char **)tmpdirs, k);
	for (i = 0; i < k; i++) {
		//printf("dir %d: %s\n", i, tmpdirs[i]);
		memset(&buf, 0, sizeof(struct stat));
		ret = lstat(tmpdirs[i], &buf);
		if (ret != 0) {
			#ifdef LS_DEBUG
			printf("stat error:%s %s\n", argv[i], strerror(errno));
			#endif
			continue;
		}

		init_ob();
		have_secon = 0;

		if (mdirs > 1)
			printf("%s:\n", tmpdirs[i]);

		ret = list_dir(tmpdirs[i], &params);

		print_all();

		if (i + 1 < k)
			printf("\n");
	}

	#if 0
	while (argv[optind]) {
		#ifdef LS_DEBUG
		printf("argc %d, argv[%d]: %s\n", argc, optind, argv[optind]);
		#endif
		init_ob();
		have_secon = 0;

		ret = list_dir(argv[optind], &params);

		if (isatty(STDOUT_FILENO))
			print_all();
		if (argv[optind + 1] != NULL)
			printf("\n");
		optind++;
	}
	#endif

	return ret;
}
