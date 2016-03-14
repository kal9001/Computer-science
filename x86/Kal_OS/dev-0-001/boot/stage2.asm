;HUGE work in progress...
;stage 2 entry point
;serts the environment for C functions to take over
;and will have MANY assembly functions for calling in C 'main'

[bits 32]
[org 0x7e00]
mov ax, (gdt.data - gdt.null)	;flat memory model   
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax
mov esp, 0x7fffe	;place stack ontop of previous stack
mov ebp, esp		;set base to start of stack
;finished set up. dont bother returning as the stack just got nuked
;may as well just fall out of the bottom.

mov esi, protected
call PM_print_text
jmp $			;nothing else to do at the moment...lockup

;this function is crap and needs a better version
PM_print_text:;prints a string onto the first line of the video buffer
   mov edx, 0xb8000	;VGA memory start
   .repeat:
      lodsb		;AL = SI++
      cmp al, 0		;compare al to 0
      je .done		;Jump if equal
      mov [edx], al
      add edx, 1
      mov [edx], byte 0x0f
      add edx, 1
      jmp .repeat	;loop to next char
   .done:
   ret			;return from function


protected	db 'Welcome to protected mode! :D', 0

times 7680-($-$$) db 0	;null padding macro upto 8k
