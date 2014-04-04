#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
	uid_t uid = -1;
	struct passwd *pw = NULL;
	
	uid = geteuid();
	if (uid != -1) {
		pw = getpwuid(uid);
		if (pw == NULL) {
			perror("");
			return -1;
		}
		printf("%s\n", pw->pw_name);
		return 0;
	}
	perror("");
	return -1;
}
