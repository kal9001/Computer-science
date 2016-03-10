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
;
;  
;

bits 16
[org 0x7c00]

;set up the temp stack to use in 16bit mode just below the upper memory area
xor ax, ax        	;set ax to zero
mov es, ax		;set buffer segment to zero for later
mov ds, ax      	;set data segment to start of ram
add ax, 0x7000    	;set ax to stack segment
mov ss, ax		;set stack segment to lowest address
mov sp, 0xfffe  	;create stack just below the 
mov bp, sp	        ;set frame pointer to match

mov [0x7dfe], dx	;preserve the boot drive number over the MBR ID incase we need it later

;This pulls a further 15 sectors from the disk.
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
   
;;;;To be continued.....

newLine		db `\n\r`, 0
diskFail 	db 'Disk read failure :-/ please check and restart computer', 0

times 510-($-$$) db 0	;null padding macro
dw 0xaa55		;MBR identifier
