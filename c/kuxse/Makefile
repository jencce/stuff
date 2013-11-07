bins = kuxgetfileconsyscall kuxgetfilexattr kuxgettaskxattr kuxsetfileconsyscall kuxsetfilexattr kuxsettaskxattr kuxtsleep kuxtsleeplow

all: $(bins)

$(bins) : % : %.c
	cc -o $@ $<

install: $(bins)
	install $(bins) /usr/local/sbin/$(bins)

uninstall: 
	rm -f /usr/local/sbin/$(bins)

test:
	bash test.sh 2>&1 > result

clean:
	rm -f $(bins)
