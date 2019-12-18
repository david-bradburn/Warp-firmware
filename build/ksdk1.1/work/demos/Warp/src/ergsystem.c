#include <stdint.h>


#include "ergsystem.h"

int[]
intsplittoarray(int data) //so we can actually get the data in the format the OLED is designed for.
{
  int ar[2];

  if(data > 99)
  {
    data = 99;
  }

  ar[0] = data % 10;
  data -= (data % 10)*10;
  ar[1] = data % 1;

  return ar;
}
