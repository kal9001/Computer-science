;simple hello world. My first every assembley program!

;x86_linux

section .text
	global _start

_start:
	mov eax,4		;sys call = sys_write
	mov ebx,1		;std_out
	mov ecx,string		;address of string
	mov edx,length		;length of string
	int 0x80		;system call
	mov eax,1		;sys call = sys_exit
	mov ebx,0		;return code 0
	int 0x80		;system call

section .data
string: db "Hello world",0x0a	;string to output + line feed
length: equ 12			;string length (+1 for termination
