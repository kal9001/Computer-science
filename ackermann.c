/*\
 *
 *  Ack.c - Ackermann function, an example of extreme recursion
 *          http://en.wikipedia.org/wiki/Ackermann_function
 *
 *  The program must be run from the command line with the two terms M and N entered afterwards with a single
 *  space between e.g. :~$./ackermann 0 1. Doing anything else will likely result in some random error as there
 *  is absolutely no input conditioning in this version.
 *  If you have read the wiki you will know the Ackermann function is an example of extreme recursion with no
 *  real functional use other than to chew through as many stack frames as possible.
 *
\*/
 
//includes
#include <stdio.h>
#include <stdlib.h>

//prototypes
unsigned int ack(unsigned int m, unsigned int n);

int main(int argc, char *argv[])
{
  unsigned int m, n, r;//r needs to be a larger type ideally but we will never realistically fill it.

  m = (*argv[1] - '0'); //Minus the charictar 0 to get the integer value
  n = (*argv[2] - '0'); //entered and not the charictar value.

  printf("Calculating Ackermann(%d, %d)\n", m, n);
  printf("This may take a very long time!\n");
  printf("[ctrl] & [c] will terminate the program.");

  r = ack(m, n);//Takes an extremely long time for most values over four.

  printf("Ackermann(%d, %d) = %d\n\n", m, n, r);

  return 0;
}//end main

unsigned int ack(unsigned int m, unsigned int n)
{
  if(m == 0)
  {
    return(n + 1);
  }
  else if (n == 0)
  {
    ack((m - 1), 1);
  }
  else
  {
    return ack((m - 1), ack(m, (n - 1)));
  }
}//end ack
