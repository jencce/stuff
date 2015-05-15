#include <stdio.h>
#include <stdlib.h>

/* why zero-length array over pointer:
 * http://www.quora.com/What-is-the-advantage-of-using-zero-length-arrays-in-C
 * 1, pointer need anoter allocation mem to it
 * 2, the separate allocated mem may not be continuous with the structure
 * 3, sometimes you can have pointers in structure
 */

int main()
{
	struct node {
		int hash;
		int value;
	};

	struct root {
		int fake;
		int fake2;
		struct node nodes[0];
	};

	struct root *hroot;

	printf("sizeof root: %d\n", sizeof(struct root));
	printf("sizeof hroot: %d\n", sizeof(hroot));

	hroot = (struct root *)malloc(sizeof(struct root) +
			20*sizeof(struct node));
	printf("sizeof hroot: %d\n", sizeof(hroot));
	printf("sizeof *hroot: %d\n", sizeof(*hroot));
	printf("sizeof hroot->nodes: %d\n", sizeof(hroot->nodes));
	printf("sizeof hroot->nodes[0]: %d\n", sizeof(hroot->nodes[0]));
	printf("sizeof hroot->nodes[19]: %d\n", sizeof(hroot->nodes[19]));
	printf("sizeof hroot->nodes[21]: %d\n", sizeof(hroot->nodes[21]));
	printf("sizeof hroot->nodes[100]: %d\n", sizeof(hroot->nodes[100]));
	hroot->nodes[0].hash = 0;
	hroot->nodes[19].hash = 1;
	hroot->nodes[21].hash = 2;
	hroot->nodes[100].hash = 3;
	printf("sizeof hroot->nodes[0]: %d\n", sizeof(hroot->nodes[0]));
	printf("sizeof hroot->nodes[19]: %d\n", sizeof(hroot->nodes[19]));
	printf("sizeof hroot->nodes[21]: %d\n", sizeof(hroot->nodes[21]));
	printf("sizeof hroot->nodes[100]: %d\n", sizeof(hroot->nodes[100]));
	printf("sizeof hroot->nodes[0].hash: %d\n", hroot->nodes[0].hash);
	printf("sizeof hroot->nodes[19].hash: %d\n", hroot->nodes[19].hash);
	printf("sizeof hroot->nodes[21].hash: %d\n", hroot->nodes[21].hash);
	printf("sizeof hroot->nodes[100].hash: %d\n", hroot->nodes[100].hash);

	free(hroot);
	return 0;
}
