#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	pid_t pid;
	int key, ret;
	char *buf;

	key = shmget(key, 4096, IPC_CREAT|IPC_EXCL);
	if (ret == -1) {
		perror("shmget");
		return -1;
	}

	buf = shmat(key, NULL, 0);
	if (buf == (void *)-1) {
		perror("shmat parent");
		return -1;
	}

	memset(buf, 'a', 4096);

	pid = fork();
	if (pid == -1) {
		perror("fork");
		return -1;
	} else if (pid == 0) { /* child */
		buf = shmat(key, NULL, 0);
		if (buf == (void *)-1) {
			perror("shmat");
			return -1;
		}
		printf("in child %c\n", buf[10]);
		sleep(10);
		ret = shmdt(buf);
		if (ret == -1) {
			perror("shmdt");
			return -1;
		}
	}

	sleep(10);
	ret = shmdt(buf);
	if (ret == -1) {
		perror("shmdt parent");
		return -1;
	}

	ret = shmctl(key, IPC_RMID, NULL);
	if (ret == -1) {
		perror("rmid");
		return -1;
	}

	wait(pid, &ret, 0);
	return 0;
}
