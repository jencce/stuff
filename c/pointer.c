#include <stdio.h>
#include <string.h>

void szof( char array[] )
{
	printf("szof array %d\n", sizeof(array) );
	/* 4, just a pointer, not array */
}
void zstrcpy( char *d, const char *s )
{
	while( (*d++ = *s++) != '\0' )
		;
}

int main()
{
	int b = 10;
	int *d = &b;
	char *s = strdup("abcdefghijklmn");
	char *ch = s;

#if 0		
	printf("bp %p, b %d\n", b, b);

	*d = 10 - *d;
	printf("dp %p\n", d);
	printf("*dp %p\n", *d);
	printf("*d %d\n", *d);

	d = 10 - *d;

	printf("%p\n", d);
	printf("%d\n", *d);
	*(int *)100 = 10;
#endif

	while( *ch++ )
		printf("%c\n", *ch);
	printf("-------\n");

	ch = s;
	printf("%p\n", ch);
	printf("%c\n", *ch);
	printf("%s\n", s);

	//printf("%p\n", *++ch);
	printf("%p\n", *++ch);
	printf("%s\n", s);
	//printf("d%d\n", *(int *)0xb769a000);

	char *cp = s + 15;
	if ( ch < cp )
		printf("ok1\n");
	char *cr = s - 3;
	if ( cr < ch )
		printf("ok2\n");
#if 0

	while( ++*ch )
		printf("%c\n", *ch);
#endif
	char c = 'a';
	char *cpp = &c;
	cpp++;
	printf("cpp %d\n", *cpp);
	// out of bounds are allowed but not safe

	char array[10] = "abcdefghi";
	printf("array %s\n", array);
	//array++; //lvalue cannot inc
	char *ap = &array[3];
	ap++;
	printf("ap %c\n", *ap);
	printf("ap[2] %c\n", ap[2]);
	printf("ap[-2] %c\n", ap[-2]);
	printf("2[ap] %c\n", 2[ap]);
	printf("ap[12] %c\n", ap[12]);
	// [ ] and * are equal

	char a2[10];
	char *sa = array;
	printf("sa %c\n", *sa);
	printf("a2 %s\n", a2);
	zstrcpy( a2, array );
	// args are delivered by value, actual arguments do not change
	printf("sa %c\n", *sa);
	printf("a2 %s\n", a2);

	printf("sizeof array %d\n", sizeof(array) );
	/* 10, array's length, not a pointer length */
	szof( array );

	char ay[] = "iiiihellowwww";
	char *say = "hellowwww";
	say++;
	//ay++;
	printf("say %s\n", say);
	printf("ay %s\n", ay);
	printf("sayc %c\n", *(++say));       //say is not const
	//printf("sayc %c\n", ++(*(say+3)));  *say is const
	printf("ayc %c\n", ++(*(ay+5)));
	//printf("ayc %c\n", ++(*(++ay)));   //ay is const, ay[i] not const

	const char *ccs = malloc(10);
	strncpy(ccs, "qwe123", 6);
	char *cs = ccs;
	//ccs[1] = '1'; //error assignment of read-only location
	printf("ccs %s\n", ccs);
	cs[1] = '1';
	printf("cs %s\n", cs);

	return 0;
}
