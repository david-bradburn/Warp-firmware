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



uint8_t
reduce_accel_array_resize_offset(int16_t acc[], uint16_t length, uint16_t index)
{
  //int i;

  if(length > 96)
  {

    return (uint8_t)((acc[(uint16_t)(index*length/96)] * 32)/(4096 * 2) + 32);


  }
  else
  {
    if(length > index)
    {
      return (uint8_t)(acc[index] * 32/(4096 * 2) + 32);
    }
    else
    {
      return 0;
    }

  }

}
