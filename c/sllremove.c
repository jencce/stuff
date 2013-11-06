#include <stdio.h>
#include <stdlib.h>

// sll, single link list

typedef struct node {
	struct node *next;
	int v; } node_t; 
void create_sll(node_t **head)
{
	int i;
	node_t *p = *head;
	for( i = 0; i < 5; i++ ) {
		node_t *p1 = (node_t *)malloc( sizeof(node_t) );
		p1->v = i;
		p->next = p1;
		p = p1;
	}
	p->next = NULL;
}

void print_sll(node_t **head)
{
	node_t *p = *head;
	while( p ) {
		printf( "%d ", p->v );
		p = p->next;
	}
	printf("\n");
}

void del_sll(node_t **head, int nd)
{
	node_t *pp = *head;
	node_t *p = pp->next;

	while( p ) {
		if( p->v == nd ) {
			pp->next = p->next;
			p->next = NULL;
			free(p);
		}
		pp = p;
		p = p->next;
	}
}

void reverse_sll(node_t **head)
{
	node_t *p;
	node_t *n;
	node_t *temp;

	if (head == NULL || *head == NULL)
		return;

	p = *head;
	n = p->next;

	while(p) {
		if(n->next != NULL) {
			temp = n->next;
			n->next = p;
			p = n;
			n = temp;
		} else {
			temp = n->next;
			n->next = p;
			p = temp;
		}
	}
	(*head)->next = NULL;
}

int main()
{
	node_t head;
	node_t *ph = &head;
	int nd = 3;

	head.v = 5;

	create_sll( &ph );
	print_sll( &ph );

	del_sll( &ph, nd);
	print_sll( &ph );

	reverse_sll(&ph);
	print_sll( &ph );

	return 0;
}
