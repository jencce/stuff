.section .data
	msg: .ascii "hell"
.section .text
.global main
main:
	mov $4, %eax   # No. 4 call 
	mov $1, %ebx   # 1 means out
	mov $msg, %ecx # addr to ecx
	mov $4, %edx    # len to edx
	int $0x80      # call
	mov $1, %eax   # No. 1 call to exit
	int $0x80;
