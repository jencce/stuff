bins = kuxgetfileconsyscall kuxgetfilexattr kuxgettaskconsyscall kuxsetfileconsyscall kuxsetfilexattr kuxsettaskconsyscall

all: $(bins)

$(bins) : % : %.c
	cc -o $@ $<

test:
	bash test.sh 2>&1 > result

clean:
	rm -f $(bins)
