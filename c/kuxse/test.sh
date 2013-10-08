#! /bin/bash

cd /home/zx/git/stuff/c/kuxse
make

# test get task con
./kuxgettaskconsyscall

# test get old labeled file xattr
./kuxgetfilexattr /home/zx/ffcmd
# test get old labeled file con
./kuxgetfileconsyscall /home/zx/ffcmd

# test set old labeled file xattr
./kuxgetfilexattr /home/zx/ffcmd
./kuxsetfilexattr /home/zx/ffcmd
./kuxgetfilexattr /home/zx/ffcmd
# test set old labeled file con
./kuxgetfileconsyscall /home/zx/ffcmd
./kuxsetfileconsyscall /home/zx/ffcmd
./kuxgetfileconsyscall /home/zx/ffcmd

# last four case for directory
# test get old labeled dir xattr
#./kuxgetfilexattr /home/zx/
# test get old labeled dir con
#./kuxgetfileconsyscall /home/zx/

# test set old labeled dir xattr
#./kuxgetfilexattr /home/zx/
#./kuxsetfilexattr /home/zx/
#./kuxgetfilexattr /home/zx/
# test set old labeled dir con
#./kuxgetfileconsyscall /home/zx/
#./kuxsetfileconsyscall /home/zx/
#./kuxgetfileconsyscall /home/zx/

# test set task con
./kuxgettaskconsyscall
./kuxsettaskconsyscall
./kuxgettaskconsyscall

touch 1
# test get new file xattr
./kuxgetfilexattr `pwd`/1
# test get new file con
./kuxgetfileconsyscall `pwd`/1

# test set new file xattr
./kuxgetfilexattr `pwd`/1
./kuxsetfilexattr `pwd`/1
./kuxgetfilexattr `pwd`/1
# test set new file con
./kuxgetfileconsyscall `pwd`/1
./kuxsetfileconsyscall `pwd`/1
./kuxgetfileconsyscall `pwd`/1
rm -f 1

#mkdir ttc
# test get new dir xattr
#./kuxgetfilexattr `pwd`/ttc
# test get new dir con
#./kuxgetfileconsyscall `pwd`/ttc

# test set new dir xattr
#./kuxgetfilexattr `pwd`/ttc
#./kuxsetfilexattr `pwd`/ttc
#./kuxgetfilexattr `pwd`/ttc
# test set new dir con
#./kuxgetfileconsyscall `pwd`/ttc
#./kuxsetfileconsyscall `pwd`/ttc
#./kuxgetfileconsyscall `pwd`/ttc

