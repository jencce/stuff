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

	struct root_s {
		int fake;
		int fake2;
		struct node *nodes;  /* pointer */
	};

	struct root *hroot;
	struct root_s *sroot;

	printf("sizeof root: %d\n", sizeof(struct root));
#if 1
	/* zero-length array tests*/
	printf("\nsizeof hroot: %d\n", sizeof(hroot));
	hroot = (struct root *)malloc(sizeof(struct root)
					+ 20*sizeof(struct node));

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
	printf("hroot->nodes[0].hash: %d\n", hroot->nodes[0].hash);
	printf("hroot->nodes[19].hash: %d\n", hroot->nodes[19].hash);
	printf("hroot->nodes[21].hash: %d\n", hroot->nodes[21].hash);
	printf("hroot->nodes[100].hash: %d\n", hroot->nodes[100].hash);

	free(hroot);
#endif
	/* pointer tests*/
	printf("\nsizeof sroot: %d\n", sizeof(sroot));
	sroot = (struct root_s *)malloc(sizeof(struct root_s));
	sroot->nodes = (struct node *)malloc(20*sizeof(struct node));  /* complicate allocs */

	printf("sizeof sroot: %d\n", sizeof(sroot));
	printf("sizeof *sroot: %d\n", sizeof(*sroot));
	printf("sizeof sroot->nodes: %d\n", sizeof(sroot->nodes));
	printf("sizeof sroot->nodes[0]: %d\n", sizeof(sroot->nodes[0]));
	printf("sizeof sroot->nodes[19]: %d\n", sizeof(sroot->nodes[19]));

	printf("sizeof sroot->nodes[21]: %d\n", sizeof(sroot->nodes[21]));
	printf("sizeof sroot->nodes[100]: %d\n", sizeof(sroot->nodes[100]));
	sroot->nodes[0].hash = 0;
	sroot->nodes[19].hash = 1;
	/* write across the border will crash the upcoming frees */
	//sroot->nodes[21].hash = 2;
	//sroot->nodes[100].hash = 3;
	printf("sizeof sroot->nodes[0]: %d\n", sizeof(sroot->nodes[0]));
	printf("sizeof sroot->nodes[19]: %d\n", sizeof(sroot->nodes[19]));
	printf("sizeof sroot->nodes[21]: %d\n", sizeof(sroot->nodes[21]));
	printf("sizeof sroot->nodes[100]: %d\n", sizeof(sroot->nodes[100]));
	printf("sroot->nodes[0].hash: %d\n", sroot->nodes[0].hash);
	printf("sroot->nodes[19].hash: %d\n", sroot->nodes[19].hash);
	printf("sroot->nodes[21].hash: %d\n", sroot->nodes[21].hash);
	printf("sroot->nodes[100].hash: %d\n", sroot->nodes[100].hash);

	free(sroot->nodes);
	free(sroot);

	/* init zero-length array ?  NO, this is NOT zla */
	struct root_v {
		int fake;
		int fake2;
		struct node nodes[5];
	};

	struct root_v vroot = {
		.fake = 1,
		.fake2 = 2,
		.nodes[0] = {
			.hash = 3,
			.value = 4,
		},
	};

	printf("\nsizeof rootv: %d\n", sizeof(struct root_v));
	printf("sizeof vroot: %d\n", sizeof(vroot));
	printf("vroot->nodes[0].hash: %d\n", vroot.nodes[0].hash);
	printf("vroot->nodes[1].hash: %d\n", vroot.nodes[1].hash);

	return 0;
}
