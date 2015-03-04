##########################################
One byte Instructions:-

0: 0000 = Halt   (Stop CPU)
1: 0001 = Add    (R0 = R0 + R1)
2: 0010 = Sub    (R0 = R0 – R1)
3: 0011 = Inc R0 (R0 = R0 +  1)
4: 0100 = Inc R1 (R1 = R1 +  1)
5: 0101 = Dec R0 (R0 = R0 –  1)
6: 0110 = Dec R1 (R1 = R1 –  1)
7: 0111 = Ring Bell

Two byte Instructions:-
value of the second byte is called <address>

8: 1000 = Print value of <address>
9: 1001 = Ld value from <address> into R0
a: 1010 = Ld value from <address> into R1
b: 1011 = St R0 to <address>
c: 1100 = St R1 to <address>
d: 1101 = Jmp IP to <address>
e: 1110 = Jmp IP to <address> if R0 == 0
f: 1111 = Jmp IP to <address> if R0 != 0

##########################################
Registers:-

IS - Instruction Store:
  Holds the currently executing instruction.
IP - Instruction Pointer:
  Points to the next instruction to be executed.
R0 - 4bit general register:
R0 - 4bit general register:

##########################################
Program Flow:-

Start up:
1, Set all registers to 0
2, Load program into memory
3, Load instruction into IS

Running:
1, Instruction in IS is executed.
2, IP is ether:
  Incremented 1 - if a one byte instruction.
  Incremented 2 - if a two byte instruction.
  set to <address> if current instruction is a jmp (d, e, f).
3, Instruction pointed to by IP is loaded into IS:
  If IP is within range.
  Else the CPU is halted.
