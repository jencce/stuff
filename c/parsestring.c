#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NSUBMAX 100

struct sgt {
	int start;
	int end;
	int idx;
	int parent;
	int nsubs;
	int flag;
	int *subs;
	char *st;
};

void initsgts(struct sgt *sgts)
{
	int i;
	for( i = 0; i < NSUBMAX ; i++ )
	{
		sgts[i].start = -1;
		sgts[i].end = -1;
		sgts[i].idx = -1;
		sgts[i].parent = -1;
		sgts[i].nsubs = 0;
		sgts[i].flag = 0;
		sgts[i].subs = NULL;
		sgts[i].st = NULL;

	}
}
int parsesub(char *inputs, struct sgt *sgts)
{
	int left_b[NSUBMAX];
	int i, j, k, l, m;
	struct sgt *parent;

	for( i = 0; i < strlen(inputs); i++ )
	{
		left_b[i] = -1;
	}

	for( i = 0, j = 0; i < strlen(inputs); i++ )
	{
		if( inputs[i] == '(' && left_b[j] == -1 )
		{
			left_b[j] = i;
			sgts[j].idx = j;
			sgts[j].start = i;
			j++;
			continue;
		}
		
		if( inputs[i] == ')' )
		{
			if( j > 0 )
			{
				for( k = 0, l = -1, m = 0; k < strlen(inputs); k++ )
					if( left_b[k] != -1 )
					{
						l = k;
					}                         // find the last match
				if( l != -1 && left_b[l] != -1 )
				{
					sgts[l].end = i;
					left_b[l] = -1;
				}
			}
		}
	}
	
	for( i = 0; sgts[i].start != -1 && sgts[i].end != -1; i++ )
	{
		if( sgts[i].start > sgts[i].end )
			return -1;
		sgts[i].st = (char *)malloc(sgts[i].end - sgts[i].start - 1);
		for( j = sgts[i].start + 1, k = 0; j < sgts[i].end; j++ )
			sgts[i].st[k++] = inputs[j];
	}

	for( i = 0; sgts[i].start != -1 && sgts[i].end != -1; i++ )
	for( j = 0; sgts[j].start != -1 && sgts[j].end != -1; j++ )
	{
		if( sgts[i].start < sgts[j].start && sgts[i].end > sgts[j].end )
		{
			sgts[j].parent = i;
			if ( sgts[i].subs == NULL )
			{
				sgts[i].subs = (int *)malloc(NSUBMAX*sizeof(int));
				for( k = 0; k < NSUBMAX; k++ )
					sgts[i].subs[k] = -1;
				
				sgts[i].subs[sgts[i].nsubs] = j;
				sgts[i].nsubs = sgts[i].nsubs + 1;
			} 
			else
			{
				sgts[i].subs[sgts[i].nsubs] = j;
				sgts[i].nsubs = sgts[i].nsubs + 1;
			}
		}
	}
	
	for( i = 0; i < strlen(inputs); i++ )
	{
		if( sgts[i].nsubs )
		{
			for( j = 0; j < sgts[i].nsubs; j++ )
			{
				l = 0;
				for( k = 0; k < sgts[i].nsubs; k++ )
				{
					if( strstr(sgts[sgts[i].subs[k]].st, sgts[sgts[i].subs[j]].st) != NULL && j != k )
						l++;
				}
				if( l == 0 )
					sgts[sgts[i].subs[j]].flag = 1;
			}
		}
	}
	return 0;
}

int main(int argc, char **argv)
{
	char *inputs;
	char *tmp = NULL;
	int i, j, k, l;
	struct sgt * sgts;

	if( argc != 2 )
	{
		printf("need one arg\n"); 
		return 1;
	}

	if( argv[1] != NULL )
		inputs = strdup(argv[1]);

	sgts = (struct sgt *)malloc( NSUBMAX * sizeof(struct sgt) );

	initsgts(sgts);

	i = parsesub(inputs, sgts);
	if( i == -1 )
		return -1;
	
	for( i = 0; i < NSUBMAX; i++ )
	{
		if( sgts[i].idx != -1 )
		{
			printf("sgti %d: start %d, end %d, idx %d, nsubs %d, parent %d, st %s, flag %d", i,
			sgts[i].start, sgts[i].end, sgts[i].idx, sgts[i].nsubs, sgts[i].parent, sgts[i].st, sgts[i].flag);
			
			for( j = 0; j < sgts[i].nsubs; j++ )
				printf("\n\tsubs%d:%d %s", j, sgts[i].subs[j], sgts[sgts[i].subs[j]].st);
			printf("\n");
		}
	}

	return 0;
}
