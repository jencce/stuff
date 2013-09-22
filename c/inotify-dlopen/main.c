#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <dirent.h>

#define EV_SIZE (sizeof(struct inotify_event))

void do_analysis(void);

void do_analysis()
{
	DIR *dir;
	struct dirent * dent;
	void (*zxd)(void) = NULL;

	dir = opendir( "./lib/" );

	while ( (dent = readdir(dir)) != NULL )
	{
		if ( strstr(dent->d_name, "so") != NULL )
		{
			char *err;
			char sname [256];
			char *ssname;
#if 1
			ssname = strndup( (char *)dent->d_name, 256 );
			sprintf( sname, "./lib/%s", ssname );
			printf( "%s\n", sname );
		
			void *handle = dlopen( sname, RTLD_LAZY );
			if ( handle == NULL )
			{
				printf("dlopen failed:%s\n", dlerror());
				return;
			}
		
			dlerror();
		
			*(void **)&zxd = dlsym(handle, "zxdate");
		
			if ( (err = dlerror()) != NULL )
			{
				printf("dlsym failed\n");
				perror(err);
				return;
			}
			if ( zxd != NULL )
				zxd();
		
			dlclose(handle);
#endif
		}
	}
}

int main()
{
	int fd, wd, len;
	char buf[ 1024 * EV_SIZE ];
	struct inotify_event *ie= NULL;

	fd = inotify_init();

	if ( fd != -1 )
		printf( "fd = %d\n", fd );
	else
	{
		printf( "init failed\n" );
		return -1;
	}

	wd = inotify_add_watch( fd, "./lib/", IN_CREATE | IN_DELETE );

	if ( wd != -1 )
		printf( "wd = %d\n", wd );
	else
	{
		printf( "addwatch failed\n" );
		return -1;
	}

	while ( len = read( fd, buf, BUFSIZ ) )
	{
	
		if ( len )
		{
			printf( "%d readed\n", len );
			do_analysis();
		}
		else
		{
			printf( "read failed\n" );
			return -1;
		}
	
		ie = ( struct inotify_event * ) &buf[0];
		if ( ie )
		{
			printf("wd %d, mask %d, ck %d, len %d, name %s\n",
				 ie->wd, ie->mask, ie->cookie, ie->len, ie->name);
		}
		else
			printf( "ie null\n");
	}
	inotify_rm_watch( fd, wd );
	close( fd );

	return 0;
}
