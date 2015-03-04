/*\
 *
 *  4917 Microprocessor simulator - v1.0
 *  Web: http://home.kal9001.co.uk  Email: kal@kal9001.co.uk
 *
 *  Copyright (C) 2015 Kal9001
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  called GPLv3.txt in the root directory where this source file is located.
 *  If not, see <http://www.gnu.org/licenses/>.
 *
\*/

/*\
 *
 *  Simulates the 4917 4bit microprocessor.
 *  The architecture is taken from a UNSW computing department lecture intended
 *  as a tool to teach higher computing students basic machine code.
 *
 *  To program the CPU you must alter the program array. The values can be entered ether
 *  as any numeric type you like the compiler doesnt care.
 *  see the attached "instruction-set.txt" for list of instructions, registers and a brief
 *  explination of the program flow.
 *
 *  known bugs:
 *
 *  If the program reaches end of memory without a halt the IP will continue to be
 *  incremented beyond 0xf. there is a conditional which catches this to prevent out of bounds
 *  data being loaded into IS but an artifact of this is the IP will display 0x10 or 0x11 on
 *  the two steps before the halt happens.
 *
 *  It is possible to put a jump at address 0xf which will then over flow one byte. this may
 *  or may not depending what is actually stored in memory there cause the IP to point upto
 *  256 bytes out of range. This is only a visual artifact on the IP display and should be
 *  caught by the IS loader and halt the CPU before an instruction is loaded.
 *
 *  it is possible to create a program that loops and creates values upto 256, this is a
 *  side effect of not being able to use less than 8 bit integers. Additional logic could be
 *  programmed in to cause wrap arounds.
 *  It is also possible to exploit this with a jump to cause the IP to point upto 256 bytes
 *  out of range, this should be caught by the IS loader and cause a CPU halt but not before the
 *  IP will display the out of range value.
 *
\*/

//-----------------
// system includes
//-----------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


//--------------------------------
// type and variable declarations
//--------------------------------

typedef struct {
  uint8_t register_0;//general purpose register
  uint8_t register_1;//General purpose register
  uint8_t register_IS;//Instruction Store register
  uint8_t register_IP;//Instruction Pointer register
  uint8_t memory[17];//program and data memory
} CPU_4917;


//---------------------
// function prototypes
//---------------------

void init_memory(CPU_4917 *memoryMap);
//initialise memory and registers to zero

void printMemory(CPU_4917 *memoryMap);
//prints out all registers and memory to the screen.

uint8_t executeInstruction(CPU_4917 *memoryMap);
//executes instruction loaded into IS. returns 0 on sucess or 1 on error.

void loadInstruction(CPU_4917 *memoryMap);
//loads value from IP address into IS

void loadProgram(CPU_4917 *memoryMap, uint8_t program[16]);
//loads a program array into memory


//---------------
// Main function
//---------------
int main(void)
{
  uint8_t startCPU = 1;//sets the CPU loop to run
  uint8_t loop;//counts the loops the program makes.
  CPU_4917 memoryMap;//initialise the dataMap structure

  uint8_t program[16] =
  { 0x09, 0x01, 0x0a, 0x01, 0x01, 0x0b ,0x0f ,0x08,
    0x0f, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07 };

  while(startCPU)//startup/reset loop
  {
    init_memory(&memoryMap);

    system("clear");
    printf("initalise memory");
    printMemory(&memoryMap);
    getchar();//used to block the loop.

    loadProgram(&memoryMap, program);

    system("clear");
    printf("load program");
    printMemory(&memoryMap);
    getchar();//used to block the loop.

    loadInstruction(&memoryMap);

    system("clear");
    printf("Load first instruction");
    printMemory(&memoryMap);
    getchar();//used to block the loop.

    loop = 0;

    while(memoryMap.register_IS != 0)//main loop
    {
      system("clear");
      printf("Loop:%d execute instruction", loop);

      if(executeInstruction(&memoryMap))
      {
        memoryMap.register_IS = 0;//Halt the CPU (will restart if startCPU is set to zero)
        continue;//skip the rest of the program loop due to an error.
      }

      printMemory(&memoryMap);
      getchar();//used to block the loop.

      loadInstruction(&memoryMap);

      system("clear");
      printf("Loop:%d Load next instruction", loop);
      printMemory(&memoryMap);
      getchar();//used to block the loop.

    }
    startCPU = 0;//comment this out and the CPU will reset after a halt.
  }
  return 0;
}

void loadProgram(CPU_4917 *memoryMap, uint8_t program[16])
{
  uint8_t counter_a;

  for(counter_a = 0; counter_a < 16; counter_a++)
  {
    memoryMap->memory[counter_a] = program[counter_a];
  }
  return;
}

void loadInstruction(CPU_4917 *memoryMap)
{
  if(memoryMap->register_IP <= 0xf)
  { memoryMap->register_IS = memoryMap->memory[memoryMap->register_IP]; }
  else
  { memoryMap->register_IS = 0x0; }//the CPU has reached the end of memory: halt.
  return;
}

uint8_t executeInstruction(CPU_4917 *memoryMap)
{
  uint8_t data = memoryMap->register_IP + 1;
  uint8_t address = memoryMap->memory[data];

  switch(memoryMap->register_IS)
  {
    case 0x0://0000 = Halt   (Stop CPU)
      //Halt, Loop should detect this and halt.
    break;

    case 0x1://0001 = Add    (R0 = R0 + R1)
      memoryMap->register_0 += memoryMap->register_1;
      memoryMap->register_IP += 1;
    break;

    case 0x2://0010 = Sub    (R0 = R0 – R1)
      memoryMap->register_0 -= memoryMap->register_1;
      memoryMap->register_IP += 1;
    break;

    case 0x3://0011 = Inc R0 (R0 = R0 +  1)
      memoryMap->register_0 += 1;
      memoryMap->register_IP += 1;
    break;

    case 0x4://0100 = Inc R1 (R1 = R1 +  1)
      memoryMap->register_1 += 1;
      memoryMap->register_IP += 1;
    break;

    case 0x5://0101 = Dec R0 (R0 = R0 –  1)
      memoryMap->register_0 -= 1;
      memoryMap->register_IP += 1;
    break;

    case 0x6://0110 = Dec R1 (R1 = R1 –  1)
      memoryMap->register_1 -= 1;
      memoryMap->register_IP += 1;
    break;

    case 0x7://0111 = Ring Bell
      printf("\nDING!!\n");
      memoryMap->register_IP += 1;
    break;

    case 0x8://1000 = Print value of <address>
      printf("\n%d\n", memoryMap->memory[address]);
      memoryMap->register_IP += 2;
    break;

    case 0x9://1001 = Ld value of <address> into R0
      memoryMap->register_0 = memoryMap->memory[address];
      memoryMap->register_IP += 2;
    break;

    case 0xa://1010 = Ld value of <address> into R1
      memoryMap->register_1 = memoryMap->memory[address];
      memoryMap->register_IP += 2;
    break;

    case 0xb://1011 = St R0 to <address>
      memoryMap->memory[address] = memoryMap->register_0;
      memoryMap->register_IP += 2;
    break;

    case 0xc://1100 = St R1 to <address>
      memoryMap->memory[address] = memoryMap->register_1;
      memoryMap->register_IP += 2;
    break;

    case 0xd://1101 = Jmp IP to <address>
      memoryMap->register_IP = memoryMap->memory[address];
    break;

    case 0xe://1110 = Jmp IP to <address> if R0 == 0
      if(memoryMap->register_0 == 0)
      { memoryMap->register_IP = memoryMap->memory[address]; }
      else
      { memoryMap->register_IP += 2; }
    break;

    case 0xf://1110 = Jmp IP to <address> if R0 != 0
      if(memoryMap->register_0 != 0)
      { memoryMap->register_IP = memoryMap->memory[address]; }
      else
      { memoryMap->register_IP += 2; }
    break;

    default:
      printf("##CLUNK! [insert BSOD here]\n");
      printf("Instruction %x is not recognised and the CPU will now halt.", memoryMap->register_IS);
    return 1;
  }
  return 0;
}

void init_memory(CPU_4917 *memoryMap)
{
  uint8_t counter_a;

  memoryMap->register_0 = 0x0;
  memoryMap->register_1 = 0x0;
  memoryMap->register_IS = 0x0;
  memoryMap->register_IP = 0x0;
  for(counter_a = 0; counter_a < 16; counter_a++)
  {
    memoryMap->memory[counter_a] = 0x0;
  }
  memoryMap->memory[16] = 0x00;
  return;
}

void printMemory(CPU_4917 *memoryMap)
{
  uint8_t counter_a;

  printf("\n-----------------------------\nRegisters:\n");
  printf("R0: %x\n", memoryMap->register_0);
  printf("R1: %x\n", memoryMap->register_1);
  printf("IS: %x\n", memoryMap->register_IS);
  printf("IP: %x\n", memoryMap->register_IP);

  printf("Memory:\nloc-");
  for(counter_a = 0; counter_a < 16; counter_a++)
  {
    printf(" %x", counter_a);
  }
  printf("\ndat-");
  for(counter_a = 0; counter_a < 16; counter_a++)
  {
    printf(" %x", memoryMap->memory[counter_a]);
  }
  printf("\n");
  return;
}
