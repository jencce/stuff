.code16  # this line is necessary
.text
	jmp __start
	nop
	BS_OEMName:		.ascii	"jencceos"
	BPB_BytsPerSec:		.2byte	512
	BPB_SecPerCluster:	.byte 	1
	BPB_ResvdSecCnt:	.2byte	32
	BPB_NumFATs:		.byte	2
	BPB_RootEntCnt:		.2byte	0
	BPB_TotSec16:		.2byte	0
	BPB_Media:		.byte	0xf0
	BPB_FATSz16:		.2byte	0
	BPB_SecPerTrk:		.2byte	18
	BPB_NumHeads:		.2byte	2
	BPB_HiddSec:		.4byte	0
	BPB_TotSec32:		.4byte	1
	BPB_FATSz32:		.4byte	1
	BPB_ExtFlags:		.2byte	0x80
	BPB_FSVer:		.2byte	0
	BPB_RootClus:		.4byte	2
	BPB_FSInfo:		.2byte	1
	BPB_BkBootSec:		.2byte	6
	BPB_Reserved:		.4byte	0x0
	BPB_Reserved1:		.4byte	0x0
	BPB_Reserved2:		.4byte	0x0
	BS_DrvNum:		.byte	0x80
	BS_Reserved1:		.byte	0
	BS_BootSig:		.byte	0x29
	BS_VolID:		.4byte	0
	BS_VolLab:		.ascii	"cat 0.1    "
	BS_FileSysType:		.ascii	"FAT32   "

__start:
	mov %cs, %ax
	mov %ax, %ds
	mov %ax, %es
	call disp
	jmp .
disp:
	mov $msg, %ax
	mov %ax, %bp
	mov $8, %cx
	mov $0x1301, %ax
	mov $0x00c, %bx
	mov $0, %dl
	int $0x10
	ret
msg: .ascii "Hello OS"
#.org 510
#.word 0xaa55  #these 2 lines are optional
