#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <glob.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>

glob_t glob_buff;

void kse_setcon(char *name)
{
	char *buf;
	ssize_t size;
	int ret;
	struct stat stat_buf;

	if (name == NULL) {
		perror("name ");
		exit(-1);
	}

	ret = lstat(name, &stat_buf);
	if (ret == -1) {
		printf("%s lstat error: %s\n", name, strerror(errno));
		return;
	}

	size = 255;
	buf = malloc(size);
	if (buf == NULL) {
		perror("malloc ");
		exit(-1);
	}
	memset(buf, 0, size);

	if (	strncmp(name, "/bin", 4) == 0 ||
		strncmp(name, "/sbin", 5) == 0 ||
		strncmp(name, "/usr/bin", 8) == 0 ||
		strncmp(name, "/usr/sbin", 9) == 0 ||
		strncmp(name, "/usr/local/bin", 14) == 0 ||
		strncmp(name, "/usr/local/sbin", 15)== 0)
			sprintf(buf, "%s", "4:0:c0:15\0");
	else if (strncmp(name, "/dev/null", 9) == 0 ||
		strncmp(name, "/dev/zero", 9) == 0 ||
		strncmp(name, "/dev/random", 11) == 0 ||
		strncmp(name, "/dev/fd/0", 9) == 0 ||
		strncmp(name, "/dev/fd/1", 9) == 0 ||
		strncmp(name, "/dev/fd/2", 9) == 0 ||
		strncmp(name, "/dev/fd/3", 9) == 0)
			sprintf(buf, "%s", "4:0:c0:8\0");
	else if (strncmp(name, "/root", 5) == 0)
		sprintf(buf, "%s", "1:2:c0,c1:8\0");
	else if (strncmp(name, "/home", 5) == 0) {
		if (strncmp(name, "/home/secadm", 12) == 0)
			sprintf(buf, "%s", "1:2:c0,c2:8\0");
		else if (strncmp(name, "/home/audadm", 12) == 0)
			sprintf(buf, "%s", "1:2:c0,c3:8\0");
		else {
			if (strcmp(name, "/home") == 0)
				sprintf(buf, "%s", "4:0:c0:8\0");
			else
				sprintf(buf, "%s", "1:1:c4:8\0");
		}
	} else if (strncmp(name, "/tmp", 4) == 0)
		sprintf(buf, "%s", "0:0:c0:8\0");
#if 0
	else if (strcmp(name, "/etc/passwd") == 0)
		sprintf(buf, "%s", "1:2:c0:8\0");
	else if (strcmp(name, "/etc/.pwd.lock") == 0)
		sprintf(buf, "%s", "1:2:c0,c1:8\0");
#endif
	else
		sprintf(buf, "%s", "2:1:c0:8\0");

		
	ret = setxattr(name, "security.kse", buf, strlen(buf) + 1, 0);
	if (ret < 0) {
		//printf("%s setxattr error: %s ", name, strerror(errno));
		//exit(-1);
	}

	free(buf);
}

void process_one(char * name)
{
	FTS *handle = NULL;
	FTSENT *fsent = NULL;
	char * const namelist[2] = {name, NULL};
	

	handle = fts_open((char * const *)namelist, FTS_PHYSICAL, NULL);
	if (handle == NULL) {
		perror("fts_open: ");
		exit(-1);
	}

	while ((fsent = fts_read(handle)) != NULL) {
		if (fsent->fts_info == FTS_DP)
			continue;
		//printf("rootp: %s\n", fsent->fts_path);
		if (fsent->fts_statp)
			kse_setcon(fsent->fts_path);
	}

	fts_close(handle);
}

void *process_thread(int *cnt)
{
	int k, j = *cnt;

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	
	for (k = j; k < glob_buff.gl_pathc; k++) {
		//printf("%s %d \n", glob_buff.gl_pathv[k], k);
		if (strstr(glob_buff.gl_pathv[k], "proc") != NULL)
			continue;
		process_one(glob_buff.gl_pathv[k]);
	}
	fflush(stdout);
	return;

}

int main()
{
	int ret, err, i;

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	
	printf("Setting KUXSE file labels\n");


	memset(&glob_buff, 0 , sizeof(glob_t));
	ret = glob("/*", 0, NULL, &glob_buff);
	if (ret != 0) {
		perror("glob :");
		exit(-1);
	}

	i = 0;
	process_thread(&i);

	globfree(&glob_buff);
	return 0;
}
