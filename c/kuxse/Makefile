bins = kuxgetfileconsyscall kuxgetfilexattr kuxsetfileconsyscall kuxsetfilexattr kuxsettaskxattr kuxtsleep kuxtsleeplow kuxmkfifo 
headbins = kuxgettaskxattr kuxtestfifo

all: $(bins) $(headbins)

$(bins) : % : %.c 
	cc -o $@ $<

$(headbins) : % : %.c kuxse.h
	cc -o $@ $<

install: $(bins)
	install $(bins) /usr/local/sbin/$(bins)

uninstall: 
	rm -f /usr/local/sbin/$(bins)

test:
	bash test.sh 2>&1 > result

clean:
	rm -f $(bins)
