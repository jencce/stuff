#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main()
{
	struct winsize ws;
	int ret;
	ret = ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
        if (ret != 0) {
                perror("ioctl error");
                return;
        }

	printf("%d, %d\n", ws.ws_row, ws.ws_col);
	printf("%d\n", ws.ws_col / ws.ws_row);
}
