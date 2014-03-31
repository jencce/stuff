#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	long hid = 0;

	if (argc != 1) {
		printf("No arg\n");
		return -1;
	}

	hid = gethostid();
	if (hid == -1) {
		perror("hostid:");
		return -1;
	}

	hid = hid & 0xffffffff;

	printf("%x\n", hid);

	return 0;
}
