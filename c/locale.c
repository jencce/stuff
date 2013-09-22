#include <stdio.h>
#include <locale.h>
#include <time.h>

int main()
{
	time_t tt;
	char s[20];
	//setlocale(LC_TIME, "zh_CN.UTF-8");
	//setlocale(LC_NUMERIC, "zh_CN.UTF-8");
	setlocale(LC_ALL, "");
	tt = time(NULL);
	strftime(s, 20, "%B %d", localtime(&tt));
	printf("%s\n", s);
	return 0;
}
