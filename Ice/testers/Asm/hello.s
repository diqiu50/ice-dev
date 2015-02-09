#hello.s
.data
	msg:.string "hello, world!\n"
	len = . - msg
.text
.global _start
_start:
	movl $len, %edx
	movl $msg, %ecx
	movl $1, %ebp
	movl $4, %eax
	int  $0x80

	movl $0, %ebp
	movl $1, %eax
	int  $0x80

