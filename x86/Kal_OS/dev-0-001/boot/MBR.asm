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
;  TBA....
;

bits 16
[org 0x7c00]

;set up the temp stack to use in 16bit mode just below the upper memory area
xor ax, ax        	;set ax to zero
mov ds, ax      	;set data segment to start of ram
add ax, 0x7000    	;set ax to stack segment
mov ss, ax		;set stack segment to lowest address
mov sp, 0xfffe  	;create stack just below the 
mov bp, s	        ;set frame pointer to match

mov [0x7dfe], dx	;preserve the boot drive number


mov ah, 0x02 		;BIOS disk read sectors
;dl already set		;from which drive
mov ch, 0x00 		;(C) track/cylinder   
;dh already set		;(H) R/W head
mov cl, 0x02 		;(S) start sector
mov al, 0x0f 		;number of sectors to read
			

mov bx, 0x0000		;load buffer segment
mov es, bx		;set buffer segment
mov bx, 0x7e00		;set offset for buffer

int 0x13		;execute disk read

;;;;To be continued.....
