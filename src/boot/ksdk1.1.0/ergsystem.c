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
reduce_accel_array_resize_offset(int acc[], int length, int index)
{
  //int accel_fit;
  int i;

  if(length > 96)
  {

    return (int)((acc[(int)*(index/length)] * 64)/(4096 * 2));
    // float p;
    // float q;
    //
    // p = 0;
    // q = 0;
    //
    // for(i = 0; i < length; i++)
    // {
    //   if(i/length < index/96 && (i + 1)/length > index/96)
    //   {
    //     p += acc[i]*(((i + 1)/length) - index/96);
    //     q += (((i + 1)/length) - index/96)/(1/length);
    //   }
    //   else if(i/length >= index/96 && (i + 1)/length < (index + 1)/96)
    //   {
    //     p += acc[i];
    //     q++;
    //   }
    //   else if(i/length < (index + 1)/96 && (i + 1)/length > (index + 1)/96)
    //   {
    //     p += acc[i]*((index + 1)/96 - ((i)/length));
    //     q += ((index + 1)/96 - ((i)/length))/(1/length);
    //   }
    //
    //   if(i/length > (index + 1)/96)
    //   {
    //     break;
    //   }
    //
    // }



  }
  else
  {
    if(length > index)
    {
      return (int)(acc[index] * 64/(4096 * 2));
    }
    else
    {
      return 0;
    }

  }

}
