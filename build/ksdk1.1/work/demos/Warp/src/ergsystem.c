#include <stdint.h>
//#include <stdlib.h>

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
reduce_accel_array_resize_offset(int acc[], int offset, int length)
{
  static int accel_fit[96];
  int i;
  int n;
  //int n_1 = 1;

  //size_t acc_len = (sizeof(acc)/sizeof(acc[0]));
  int ar_len = (int)(sizeof(acc)/sizeof(acc[0]));

  for(i = 0; i < ar_len; i++)
  {
    acc[i] -= offset;
  }

  if(ar_len > 96)
  {
    float p;
    float q;

    for(n = 0; n < 96; n++)
    {
      p = 0;
      q = 0;

      for(i = 0; i < ar_len; i++)
      {
        if(i/ar_len < n/96 && (i + 1)/ar_len > n/96)
        {
          p += acc[i]*(((i + 1)/ar_len) - n/96);
          q += (((i + 1)/ar_len) - n/96)/(1/ar_len);
        }
        else if(i/ar_len >= n/96 && i/ar_len < (n + 1)/96 && (i + 1)/ar_len < (n + 1)/96)
        {
          p += acc[i];
          q++;
        }
        else if(i/ar_len < (n + 1)/96 && (i + 1)/ar_len > (n + 1)/96)
        {
          p += acc[i]*((n + 1)/96 - ((i)/ar_len));
          q += ((n + 1)/96 - ((i)/ar_len))/(1/ar_len);
        }

        if(i/ar_len > (n + 1)/96)
        {
            break;
        }

      }

      accel_fit[n] = ((p/q) * 64)/(4096 * 2);
    }

  }
  else
  {
    for(i = 0; i < ar_len; i++)
    {
      accel_fit[i] = acc[i];
    }
    for(i = ar_len; i < 96; i++)
    {
      accel_fit[i] = 0;
    }
  }

  return accel_fit;
}
