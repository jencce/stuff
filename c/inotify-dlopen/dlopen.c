#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	void (*zxd)(void);
	char *err;

	void * handle = dlopen("./libzx.so", RTLD_LAZY);
	if ( handle == NULL )
	{
		printf("dlopen failed:%s\n", dlerror());
		return -1;
	}

	dlerror();

	*(void **)&zxd = dlsym(handle, "zxdate");

	if ( (err = dlerror()) != NULL )
	{
		printf("dlsym failed\n");
		perror(err);
		return -1;
	}

	zxd();

	dlclose(handle);

	return 0;
}
