module := pam_capset_auth.so

all : $(module)

$(module) : %.so :%.c
	gcc -g -fPIC -c $^
	gcc -g -shared -o $@ $(patsubst %.so, %.o, $(module)) -lpam


install: $(module)
	install $(module) /lib64/security/$(module)

uninstall: 
	rm -f /lib64/security/$(module)

clean:
	rm -f $(module) $(patsubst %.so, %.o, $(module))

