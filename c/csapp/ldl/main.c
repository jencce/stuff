#include <stdio.h>
#include <dlfcn.h>

int x[2] = {1, 2};
int y[2] = {4, 5};
int z[2];

int main()
{
	void *handle;
	void (*addvec)(int *, int *, int *, int );

	handle = dlopen("./libvec.so", RTLD_LAZY);
	if(!handle)
		printf("dlopen err");
	
	addvec = dlsym(handle, "addvec");
	if(dlerror() != NULL)
		printf("dlsym err");

	addvec(x, y, z, 2);
	printf("z = [%d %d]\n", z[0], z[1]);
	
	if(dlclose(handle) < 0)
		printf("dlclose err");
	
	return 0;
}
