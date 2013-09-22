/* cachekiller.c
 * change the XRES/YRES from 4096 to e.g. 4095 or other numbers and
 * look for a performance change.
 */

#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BUF;

#define XRES 4096
#define YRES 4096
#define BUFLEN ((long)XRES*(long)YRES*(long)sizeof(BUF))

static BUF *fillpic()
{
    BUF *buf;
    register BUF *bp;
    register int x,y;

    bp = buf = (BUF *)malloc( BUFLEN );
    if (buf == (BUF *)0) {
      printf("cannot allocate\n");
      exit(0);
    }

    for( y=0; y < YRES; y++ ) 
	for( x=0; x < XRES; x++ ) 
	    *bp++ = (BUF)(x & 0xff);

    return(buf);
} /*fillpic*/


static void filterF(in1,out1)
  BUF *in1,*out1;
{
    int y;
    register int i0,i1,i2;
    register int x;
    register BUF *in,*out;

    in = in1;
    out = out1;

    for( y=0; y < YRES; y++ ) {
	i0 = (int)in[0];
        i1 = (int)in[1];

        /* ignore boundary pixels, over/underflow for this benchmark */
	for( x=1; x < XRES-1; x++ ) {
	    i2 = (int)in[x+1];
	    out[x] = (BUF)( (i0 + (2*i1) + i2) / 4 );
	    i0 = i1; i1 = i2;
	}
        in += XRES;
        out += XRES;
    }

} /*filterF*/


main(ac,av)
  int ac;
  char *av[];
{
    int i;
    BUF *a,*b;

    a = fillpic();
    b = fillpic();

    for( i=0; i < 3; i++ )
      filterF(a,b);

    exit(0);
} /*bench2d*/
