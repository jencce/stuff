int x[2] = {1, 2};
int y[2] = {4, 5};
int z[2];

int main()
{
	addvec(x, y, z, 2);
	printf("z = [%d %d]\n", z[0], z[1]);
	return 0;
}
