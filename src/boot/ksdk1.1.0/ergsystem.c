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


int *
reduce_accel_array_resize_offset(int acc[], int offset)
{
  static int accel_fit[96];
  int i;
  int n;
  //int n_1 = 1;

  int acc_len = (sizeof(acc)/sizeof(acc[0]));

  for(i = 0; i < acc_len; i++)
  {
    acc[i] -= offset;
  }

  if(acc_len > 96)
  {
    float p;
    float q;

    for(n = 0; n < 96; n++)
    {
      p = 0;
      q = 0;

      for(i = 0; i < acc_len; i++)
      {
        if(i/acc_len < n/96 && (i + 1)/acc_len > n/96)
        {
          p += acc[i]*(((i + 1)/acc_len) - n/96);
          q += (((i + 1)/acc_len) - n/96)/(1/acc_len);
        }
        else if(i/acc_len >= n/96 && i/acc_len < (n + 1)/96 && (i + 1)/acc_len < (n + 1)/96)
        {
          p += acc[i];
          q++;

        }
        else if(i/acc_len < (n + 1)/96 && (i + 1)/acc_len > (n + 1)/96)
        {
          p += acc[i]*((n + 1)/96 - ((i)/acc_len));
          q += ((n + 1)/96 - ((i)/acc_len))/(1/acc_len);
        }

        if(i/acc_len > (n + 1)/96)
        {
            break;
        }

      }

      accel_fit[n] = ((p/q) * 64)/(4096 * 2);
    }
  }

  return acc_fit;
}
