Jump instructions<br>
* null
* JGT | jump if  > 0        
* JEQ | jump if  = 0
* JGE | jump if >= 0
* JLT | jump if  < 0
* JNE | jump if != 0
* JLE | jump if <= 0
* JMP | jump always

destinations<br>
null, M, D, MD, A, AM, AD, AMD <br>

operations<br>
0, 1, -1, D, A, !D, !A, -D, -A, D+1, A+1, D-1, A-1, D+A, D-A, A-D, D&A, D|A <br>
 ,  ,   , M,  , !M,   , -M,   , M+1,    , M-1,    , D+M, D-M, M-D, D&M, D|M <br>

Memory map <br>
* 0x0000  0   R0/SP
* 0x0001  1   R1/LCL
* 0x0002  2   R2/ARG
* 0x0003  3   R3/THIS
* 0x0004  4   R4/THAT
* 0x0005  5   R5
* 0x0006  6   R6
* 0x0007  7   R7
* 0x0008  8   R8
* 0x0009  9   R9
* 0x000a  10  R10
* 0x000b  11  R11
* 0x000c  12  R12
* 0x000d  13  R13
* 0x000e  14  R14
* 0x000f  15  R15
* 0x4000  16384  SCREEN ~8k 512x256
* 0x6000  24576  KBD  16 bits, Keyboard ASCII code
