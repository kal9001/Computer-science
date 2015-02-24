/*\
 * number words - v1.0
 * Web: http://home.kal9001.co.uk  Email: kal@kal9001.co.uk
 *
 * Copyright (C) 2015 Kal9001
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * called GPLv3.txt in the root directory where this source file is located.
 * If not, see <http://www.gnu.org/licenses/>.
\*/

/*\
 *
 *  Takes an input upto 999,999,999 and converts it into a spoken number.
 *  e.g. 123 would become "One hundred and twenty three".
 *  It has four languages to select from, English is #1 the others are currently
 *  blank but any words for a base ten system can be used.
 *
 *  bugs: None that i have found in this version yet.
 *
\*/

//global includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//local includes
//none

//function prototypes
uint8_t zero(uint32_t* input, uint8_t* language);
  //checks for a zero

void group(uint32_t* input, uint32_t* magnitude, uint8_t group, uint8_t* carry, uint8_t* language);
  //process each 'group' of xxx's 
  void ones(uint8_t input, uint8_t* language);
    //does the ones.
  void tens(uint8_t input, uint8_t* language);
    //does the tens
  void teen(uint8_t input, uint8_t* language);
    //does the teens
  void bign(uint8_t input, uint8_t* language);
    //does the bigger numbers like 'million', 'thousand', 'hundered', 'and' and blank.


int main()
{
  uint32_t input, magnitude;
  uint8_t counter_g, carry, language;

  printf("Please select a language to use:\n");
  printf("1, English\n2, UNUSED\n3, UNUSED\n4, UNUSED\n");
  scanf("%c", &language);
  language -= 0x30;

  while(1)
  {
    //reset to starting values.
    input = 0; carry = 0;
    magnitude = 100000000;
    
    system("clear");//clear the console.
    printf("NumberWords v1.0. Copyright (C) 2015  Kal9001\n");
    printf("Welcome to kal's numbers to words program\n");
    printf("Please enter a number to convert into text: ");

    scanf("%u", &input);//read upto a 32bit integer from stdin
    getchar();//eats the return key

    printf("You have entered: %u.\n\n", input);

    if(zero(&input, &language) == 0)
    {
      for(counter_g = 0; counter_g <= 2; counter_g++)
      {
        group(&input, &magnitude, counter_g, &carry, &language);
      }
    }
    printf("\nPress return to enter another number\n");
    printf("or press [ctrl] + [c] to quit.\n");
    getchar();//wait for return key
  }
  return 0;
}

void group(uint32_t* input, uint32_t* magnitude, uint8_t group, uint8_t* carry, uint8_t* language)
{
  uint8_t value1, value10, value100;

  value100 = *input / *magnitude;    //hundereds
  *input -= (value100 * *magnitude);
  *magnitude /= 10;

  value10 = *input / *magnitude;    //tens
  *input -= (value10 * *magnitude);
  *magnitude /= 10;

  value1 = *input / *magnitude;    //ones
  *input -= (value1 * *magnitude);
  *magnitude /= 10;

  if(value100 > 0)
  {
    ones(value100, language);//number
    bign(3, language);//hundered
  }
  if
 (((value100 > 0) & ((value10 > 0) | (value1 > 0))) | ((group == 2) & (*carry == 1)))
  {
    bign(4, language);//and
  }

  if((value10 == 1) & (value1 != 0))
  {
    teen(value1, language);
  }
  else
  {
    tens(value10, language);
    if(value1 > 0)
    {
      ones(value1, language);
    }
  }

  if((value1 > 0) | (value10 > 0) | (value100 > 0))
  {
    bign(group, language);
    *carry = 1;
  }
  return;
}

void space(void)
{
  printf(" ");
}

uint8_t zero(uint32_t* input, uint8_t* language)
{
  if(*input == 0)
  {
    switch(*language)
    {
    case 1: printf("zero "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
    return 1;
  }
  return 0;
}

void bign(uint8_t input, uint8_t* language)
{
  switch(input)
  {
  case 4: switch(*language)
    {
    case 1: printf("and "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 3: switch(*language)
    {
    case 1: printf("hundered "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 2: switch(*language)
    {
    case 1: /*nothing*/ break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 1: switch(*language)
    {
    case 1: printf("thousand "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 0: switch(*language)
    {
    case 1: printf("million "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;
  }
return;
}

void teen(uint8_t input, uint8_t* language )
{
  switch(input)
  {
  case 1: switch(*language)
    {
    case 1: printf("eleven "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 2: switch(*language)
    {
    case 1: printf("twelve "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 3: switch(*language)
    {
    case 1: printf("thirteen "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 4: switch(*language)
    {
    case 1: printf("fourteen "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 5: switch(*language)
    {
    case 1: printf("fifteen "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 6: switch(*language)
    {
    case 1: printf("sixteen "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 7: switch(*language)
    {
    case 1: printf("seventeen "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 8: switch(*language)
    {
    case 1: printf("eighteen "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 9: switch(*language)
    {
    case 1: printf("nineteen "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;
  }
return;
}

void tens(uint8_t input, uint8_t* language)
{
  switch(input)
  {
  case 1: switch(*language)
    {
    case 1: printf("ten "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 2: switch(*language)
    {
    case 1: printf("twenty "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 3: switch(*language)
    {
    case 1: printf("thirty "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 4: switch(*language)
    {
    case 1: printf("fourty "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 5: switch(*language)
    {
    case 1: printf("fifty "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 6: switch(*language)
    {
    case 1: printf("sixty "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 7: switch(*language)
    {
    case 1: printf("seventy "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 8: switch(*language)
    {
    case 1: printf("eighty "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 9: switch(*language)
    {
    case 1: printf("ninety "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;
  }
return;
}

void ones(uint8_t input, uint8_t* language)
{
  switch(input)
  {
  case 1: switch(*language)
    {
    case 1: printf("one "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 2: switch(*language)
    {
    case 1: printf("two "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 3: switch(*language)
    {
    case 1: printf("three "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 4: switch(*language)
    {
    case 1: printf("four "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 5: switch(*language)
    {
    case 1: printf("five "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 6: switch(*language)
    {
    case 1: printf("six "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;

    }
  break;

  case 7: switch(*language)
    {
    case 1: printf("seven "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 8: switch(*language)
    {
    case 1: printf("eight "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;

  case 9: switch(*language)
    {
    case 1: printf("nine "); break;
    case 2: printf(". "); break;
    case 3: printf(". "); break;
    case 4: printf(". "); break;
    }
  break;
  }
return;
}
