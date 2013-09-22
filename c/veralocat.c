#include <stdio.h>

#define a 102

int b;
int e = 1;
static int c;
const int ci;
const int ci1 = 0;
const int ci2 = 1;
static int d = 0;
int bss[10] = {0};
int data[10] = {1};

char *s = "hello";
char s1[] = "hello";

int main()
{
	static int f;
	static int g = 0;
	
	//s[0] = 'a';
	s1[0] = 'a';
}
