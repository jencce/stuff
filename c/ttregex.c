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
        ret = regcomp(&preg, "\\(ab\\(xyz\\)*\\)\\(c\\(d\\)*\\)\\{0,3\\}", 0);
	printf("ret=%d\n", ret);
        ret = regexec(&preg, "abcdcdc", 5, pmatch2, 0);
	printf("ret=%d\n", ret);
	for(i = 0;i < 5;i++)
	{
		printf("%d pmatch.rm_so = %d\n", i, pmatch2[i].rm_so);
        	printf("%d pmatch.rm_eo = %ld\n", i, pmatch2[i].rm_eo);
	}
	
        return 0;
}
