int main()
{
	int i = 0;
	struct netent *net;
#ifdef ZX
	printf("defined\n");
#endif

	net = getnetbyaddr( i, AF_INET );
	if( net != 0 )
		printf("ok\n");
	else
		printf("not ok\n");

	return 0;

}
