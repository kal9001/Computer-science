[BITS 16]
org 0x7c00

;set up the temp stack to use in 16bit mode just below the upper memory area
xor ax, ax		;set ax to zero
mov es, ax		;set buffer segment to zero for later
mov ds, ax		;set data segment to start of ram
add ax, 0x7000		;set ax to stack segment
mov ss, ax		;set stack segment to lowest address
mov sp, 0xfffe		;create stack just below the 
mov bp, sp		;set frame pointer to match

mov [0x7dfe], dx	;preserve the boot drive number


mov ah, 0x02 		;BIOS disk read sectors
;dl already set		;from which drive
mov ch, 0x00 		;(C) track/cylinder   
;dh already set		;(H) R/W head
mov cl, 0x02 		;(S) start sector
mov al, 0x0f 		;number of sectors to read
			
;es already set		;segment for buffer		
mov bx, 0x7e00		;set offset for buffer
int 0x13		;execute disk read

jnc 0x7e00		;jump if succesful

mov si, diskFail	;disk failure message
call print_string	;print message
jmp $			;endless loop

print_string:;prints a string from memory
   mov ah, 0x0e		;BIOS TTY function
   .repeat:
      lodsb		;AL = SI++
      cmp al, 0		;compare al to 0
      je .done		;Jump if equal
      int 0x10		;call BIOS to print char
      jmp .repeat	;loop to next char
   .done:
   ret			;return from function

new_line:;Prints line line feed and carriage return
   push si		;preserve si
   mov si, newLine	;load newline string
   call print_string	;call to print the newline chars
   pop si		;restore si
   ret			;return from function

A20_check:;checks if the A20 line is enabled or not
   ;fs is set by A20_enable to 0xffff
   ;ds is already set for the bootloadr at 0x0000.

   mov di, 0x0500	;for low memory offset
   mov si, 0x0510	;for high memory offset
 
   mov al, byte [fs:si]	;preserve the high memory byte 
   push ax		;on the stack
   mov al, byte [ds:si]	;preserve the low memory byte
   push ax		;on the stack
 
   mov byte [fs:si], 0x00	;set high memory to zeros
   mov byte [ds:di], 0xFF	;set low memory to ones
   cmp byte [fs:si], 0xFF	;high memory should still be ones
 
   pop ax		;from the stack
   mov byte [ds:di], al	;restore lower the memory byte
   pop ax		;from the stack
   mov byte [fs:si], al	;restore the higher memory byte
 
   je .disable	;jump over the success part if the values were the same
 
   mov ax, 0	;ax=0 means the memory values were different
   ret   	;as they should have been so no action is needed
.disable:
   mov ax, 1	;ax=1 means the read from high memory looped around to
   ret		;low memory so the A20 line needs enabling

A20_enable:;attempt to activate the A20 line
   mov ax, 0xffff	
   mov fs, ax		;set fs to highest segment
	
   call A20_check	;check...
   cmp ax, 0		;does it work
   je .done1		;yes...great, jump over the rest
   mov ax, 0x2401	;try enabling by BIOS
   int 0x15		;idgaf about the retrn value...
   call A20_check	;check again
   cmp ax, 0		;does it work
   je .done2		;yes...great, jump over the rest
   mov si, gateFail	;no!
   call print_string	;give up for now
   jmp $		;lock up..cant be assed
   ;keyboard and fast methods should be added here later on...

   
.done1:
   mov si, gateWoop
   call print_string
   ret
.done2:
   mov si, gateBios
   call print_string
   ret

newLine		db `\n\r`, 0
diskFail 	db 'Disk read failure :-/ please check and restart computer', 0
gateFail	db 'Could not open A20...', 0
gateWoop	db 'A20 was already open!', 0
gateBios	db 'A20 was opened by BIOS', 0
welcome		db 'Kal_OS', 0
times 510-($-$$) db 0	;null padding macro
dw 0xaa55		;MBR identifier

call A20_enable		;enable A20, subroutine will lockup if it fails

call new_line
mov si, welcome		;print welcome message
call print_string

jmp $			;nothing else to do at the moment...lockup

;just to move this stuff away so the hex is easier to read.
times 128 db 0

gdt:;temporary global descriptor table for 32-bit mode
   .null:
      db 0x00
      db 0x00

   .code:
      dw 0xffff		;Limit (bits  0-15)
      dw 0x0000		;Base  (bits  0-15)
      db 0x00		;Base  (bits 16-23)
      db 0b10011010	;Flags1 / Type flags
      db 0b11001111	;Flags2 / Limit 16-19
      db 0x00		;Base 24-31 
   
   .data:
      dw 0xffff		;Limit (bits  0-15)
      dw 0x0000		;Base  (bits  0-15)
      db 0x00		;Base  (bits 16-23)
      db 0b10010010	;Flags1 / Type flags
      db 0b11001111	;Flags2 / Limit 16-19
      db 0x00		;Base 24-31 

   .end:

   .descriptor:
      dw .end - .null - 1
      dd .null

CODE_SEGMENT equ .code - .null
DATA_SEGMENT equ .data - .null

switch_to_PM:
   cli			;disable interrupts
   lgdt [gdt.descriptor]
   
   mov eax, cr0		;load control register 0
   or  eax, 0x1		;mask enable bit 1
   mov cr0, eax		;save modified control register 0

   jmp CODE_SEGMENT:init_PM	;long jump to fully enable

[bits 32]

init_PM:
   mov ax, DATA_SEGMENT	;flat memory model
   mov ds, ax
   mov ss, ax
   mov es, ax
   mov fs, ax
   mov gs, ax

   mov esp, 0x7fffe	;place stack ontop of previous stack
   mov ebp, esp		;set base to start of stack

   jmp $		;this jump should be to more code...

times 8192-($-$$) db 0	;null padding macro upto 8k
