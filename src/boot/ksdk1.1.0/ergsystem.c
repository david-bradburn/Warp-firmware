#include <stdint.h>
#include <stdlib.h>

#include "ergsystem.h"
//#include "devMMA8451Q.h"

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

  if(data > 99)
  {
    data = 99;
  }

  if(data < 10)
  {
    return 0;
  }
  else
  {
    data -= (data % 10);
    return data/10;
  }
}



int
reduce_accel_array_resize_offset(int16_t acc[], uint16_t length, uint16_t index)
{
  //int i;

  if(length > 96)
  {

    return (int16_t)((acc[(int16_t)(index*length/96)] * 64)/(4096 * 2));


  }
  else
  {
    if(length > index)
    {
      return (int16_t)(acc[index] * 64/(4096 * 2));
    }
    else
    {
      return 0;
    }

  }

}
