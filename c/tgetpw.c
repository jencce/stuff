#include <sys/types.h>
#include <pwd.h>

int main()
{
	printf("%s\n", getpwnam("zx")->pw_passwd);
	return 0;
}
