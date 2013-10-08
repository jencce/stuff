#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <glob.h>
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

	ret = stat(name, &stat_buf);
	if (ret < 0)
		return;

	size = 256;
	buf = malloc(size);
	if (buf == NULL) {
		perror("malloc ");
		exit(-1);
	}
	memset(buf, 0, size);
	sprintf(buf, "%s", "1:1:c0:1");

	ret = setxattr(name, "security.kse", buf, size - 1);
	if (ret <= 0) {
		//perror("getxattr ");
		//exit(-1);
	}
	else
		printf("xattr1 %s\n", buf);
}

void process_one(char * const *name)
{
	FTS *handle = NULL;
	FTSENT *fsent = NULL;

	handle = fts_open(name, 0, NULL);
	if (handle == NULL) {
		perror("fts_open: ");
		exit(-1);
	}

	printf("dirp: %s\n", *name);
	kse_setcon(*name);
	
	fsent = fts_read(handle);
	if (fsent == NULL) {
		perror("fts_read: ");
		exit(-1);
	} else 
		printf("rootp: %s\n", fsent->fts_path);

	do {
		printf("rootp: %s\n", fsent->fts_path);
		if (fsent->fts_statp)
			kse_setcon(fsent->fts_path);
	} while ((fsent = fts_read(handle)) != NULL);

	fts_close(handle);
}

void process_thread(int *cnt)
{
	int i, j = *cnt;

	printf("j=%d\n", j);
	for (i = j; i < glob_buff.gl_pathc; i += 4) {
		//printf("%s\n", glob_buff.gl_pathv[i]);
		if (strstr(glob_buff.gl_pathv[i], "/proc") != NULL)
			continue;
		process_one(&glob_buff.gl_pathv[i]);
	}

}

int main()
{
	int ret, err, i;
	pthread_t *pt = NULL;
	pthread_attr_t pattr;
	void *res = NULL;
	
	printf("Setting KUXSE file labels\n");

	pt = (pthread_t *)malloc(4 * sizeof(pthread_t));
	if (pt == NULL) {
		perror("malloc ");
		exit(-1);
	}
	pthread_attr_init(&pattr);

	ret = glob("/*", 0, NULL, &glob_buff);
	if (ret != 0) {
		perror("glob :");
		exit(-1);
	}

	for (i = 0; i < 4; i++) {
		cpu_set_t cpuset;
		CPU_ZERO(&cpuset);
		CPU_SET(i, &cpuset);
	
		ret = pthread_attr_setaffinity_np(&pattr, sizeof(cpu_set_t),
				&cpuset);
		if (ret != 0) {
			perror("pthread_attr_set ");
			exit(-1);
		}

		ret = pthread_create(&pt[i], &pattr, &process_thread,
				&i);
		if (ret != 0) {
			perror("pthread_create ");
			exit(-1);
		}
	}

	for (i = 0; i < 4; i++) {
		ret = pthread_join(pt[i], &res);
		if (ret != 0) {
			perror("pthread_join ");
			exit(-1);
		}
		//if (res)
		//	free(res);
	}

#if 0
	for (i = 0; i < glob_buff.gl_pathc; i++) {
		//printf("%s\n", glob_buff.gl_pathv[i]);
		ret = pthread_create(&pt[i], &pattr, &process_one,
				&glob_buff.gl_pathv[i]);
		if (ret != 0) {
			perror("pthread_create ");
			exit(-1);
		}
			
		//process_one(&glob_buff.gl_pathv[i]);
	}

	for (i = 0; i < glob_buff.gl_pathc; i++) {
		ret = pthread_join(pt[i], &res);
		if (ret != 0) {
			perror("pthread_join ");
			exit(-1);
		}
		free(res);
	}
#endif

	return 0;
}
