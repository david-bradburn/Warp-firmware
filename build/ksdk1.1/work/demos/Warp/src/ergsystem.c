#include <stdint.h>


#include "ergsystem.h"

int
intsplittoarrayupper(int data) //so we can actually get the data in the format the OLED is designed for.
{
  int ar[2];

  if(data > 99)
  {
    data = 99;
  }

  ar[0] = data % 10;


  return ar[0];
}

int
intsplittoarraylower(int data) //so we can actually get the data in the format the OLED is designed for.
{
  int ar;

  if(data > 99)
  {
    data = 99;
  }

  data -= (data % 10)*10;
  ar = data % 1;

  return ar;
}
