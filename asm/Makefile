CC=gcc
LD=ld
LDFILE=x86.ld
OBJCOPY=objcopy

all: boot.img

# step 1: gcc invoke as compile .S to .o
boot.o : boot.S
	$(CC) -c boot.S

# step 2: ld link .o to .elf with ld scripts
boot.elf : boot.o
	$(LD) boot.o -o boot.elf -e c -T$(LDFILE)

# step 3: objcopy remove useless sections in boot.elf (like .pdr,.comment)
#         strip all symbol info, generate boot.bin
boot.bin : boot.elf
	@$(OBJCOPY) -R .prt -R .comment -R .note -S -O binary boot.elf boot.bin

# step 4: generate boot image
boot.img : boot.bin
	@dd if=boot.bin of=boot.img bs=512 count=1
	@dd if=/dev/zero of=boot.img skip=1 seek=1 bs=512 count=2879

clean:
	@rm -rf boot.{o,elf,bin,img}
