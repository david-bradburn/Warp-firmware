#include <stdint.h>


#include "ergsystem.h"

int
intsplittoarraylower(int data) //so we can actually get the data in the format the OLED is designed for.
{
  int ar1;

  if(data > 99)
  {
    data = 99;
  }

  ar1 = data % 10;


  return ar1;
}

int
intsplittoarrayupper(int data) //so we can actually get the data in the format the OLED is designed for.
{
  int ar2;

  if(data > 99)
  {
    data = 99;
  }

  data -= (data % 10)/10;

  return data;
}
