#include	<regex.h>
#include	<stdio.h>
#include        <unistd.h>
#include        <sys/types.h>

int main()
{
	int ret;
        regmatch_t pmatch1[1];
	regmatch_t pmatch2[10];
        regex_t preg;
	int i;
        ret = regcomp(&preg, "\\(ab*\\)\\{1,3\\}\\1", 0);
        //ret = regcomp(&preg, "\\(ab*\\)*\\1", 0);
	printf("%s ret=%d\n","\\(ab*\\)*\\1", ret);
        ret = regexec(&preg, "abbbabbabbbbb", 1, pmatch2, 0);
	printf("%s ret=%d\n","abbbabbabbbbb", ret);
	for(i = 0;i < 1;i++)
	{
		printf("%d pmatch.rm_so = %d\n", i, (long)pmatch2[i].rm_so);
        	printf("%d pmatch.rm_eo = %d\n", i, (long)pmatch2[i].rm_eo);
	}
        return 0;
}
