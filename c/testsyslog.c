#include <stdio.h>
#include <syslog.h>

int main()
{
	openlog("ZXTEST", LOG_NDELAY, LOG_USER);
	syslog(LOG_INFO, "%s", "zx test");
	closelog();
	return 0;
}
