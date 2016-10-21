struct node {
	int len;
	char buf[10];
};

int main()
{
	struct node node1 = { 2, "ab" };
	struct node node2 = { 2, "cd" };
	struct node nodes[2];
	struct node **ss;
	struct node *s;

	nodes[0] = node1;
	nodes[1] = node2;

	ss = &nodes;

	s = *ss;
	printf("len %d, s %s\n", s[0].len, s[0].buf);
	//ss++;
	//s = *ss;
	//printf("len %d, s %s\n", s->len, s->buf);

	return 0;
}
