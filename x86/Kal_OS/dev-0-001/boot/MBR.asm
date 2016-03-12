;  Kal_OS master boot record. Stage 1 boot loader.
;  Web: http://home.kal9001.co.uk  Email: kal@kal9001.co.uk
;
;  Copyright (C) 2016 Kal9001
;
;  This program is free software: you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation, either version 3 of the License, or
;  (at your option) any later version.
;
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License
;  called GPLv3.txt in the root directory where this source file is located.
;  If not, see <http://www.gnu.org/licenses/>.
;
;  Features:
;  512B 1st stage boot loader, Pulls a further 15 sectors from the disk and writes them directly after the first.
;  These sectors form the stage 2 boot loader which is in a seperate file.
;  *note right now we dont care about a partition table...
;  **note this binary must start on the first sector of the disk.
;  

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

jnc continue		;if disk loading didnt fail jump to continue

mov si, diskFail	;disk failure message
call print_string	;print message
jmp $			;lockup if disk fail

continue:
call A20_enable		;enable A20, subroutine will lockup if it fails
call new_line
mov si, welcome		;print welcome message
call print_string
call new_line
call switch_to_PM	;will jump to 32bit code
jmp $			;just incase some berk accidently ret's from above...

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
      ret	;low memory so the A20 line needs enabling

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
   .done1:;reports that the gate was already open!
      mov si, gateWoop
      call print_string
      ret
   .done2:;reports that the gate needed to be opened by BIOS
      mov si, gateBios
      call print_string
      ret

switch_to_PM:
   cli			;disable interrupts
   lgdt [gdt.descriptor]
   mov eax, cr0		;load control register 0
   or  eax, 1		;mask enable bit 1
   mov cr0, eax		;save modified control register 0
   jmp (gdt.code - gdt.null):init_PM	;long jump to fully enable

gdt:;temporary global descriptor table for 32-bit mode
   .null:
      dd 0x00
      dd 0x00

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

   .descriptor:
      dw .descriptor - .null - 1
      dd .null

newLine		db `\n\r`, 0
diskFail 	db 'Disk read failure :-/ please check and restart computer', 0
gateFail	db 'Could not open A20...', 0
gateWoop	db 'A20 was already open!', 0
gateBios	db 'A20 was opened by BIOS', 0
welcome		db 'Kal_OS', 0
protected	db 'Welcome to protected mode! :D', 0

times 510-($-$$) db 0	;null padding macro
dw 0xaa55		;MBR identifier

[bits 32]
init_PM:;everything should be 32bit from here on! :D
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

times 8192-($-$$) db 0	;null padding macro upto 8k
