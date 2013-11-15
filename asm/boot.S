.code16
.text
	mov %cs, %ax
	mov %ax, %ds
	mov %ax, %es
	call disp
	jmp .
disp:
	mov $msg, %ax
	mov %ax, %bp
	mov $16, %cx
	mov $0x1301, %ax
	mov $0x00c, %bx
	mov $0, %dl
	int $0x10
	ret
msg: .ascii "Hello OS"
.org 510
.word 0xaa55
