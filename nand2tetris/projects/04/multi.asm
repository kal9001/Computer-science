// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

    @2      //Select R2
    M=0     //Set to zero
(LOOP)
    @0      //Select R0
    D=M     //Set D to R0
    @END    //Select END
    D;JEQ   //If D == 0, jump to END

    @0      //Select R0
    M=M-1   //Decrement R0

    @1      //Select R1
    D=M     //Set D to R1
    @2      //Select R2
    M=D+M   //Set R2 to D+R2
    @LOOP   //Select LOOP
    0;JMP   //Jump to LOOP
(END)
    @END    //Select END
    0;JMP   //Jump to END
