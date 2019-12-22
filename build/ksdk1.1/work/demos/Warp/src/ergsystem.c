#include <stdint.h>
#include <stdlib.h>

#include "ergsystem.h"
#include "devMMA8451Q.h"

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
offset_av_calc(int length, int menuI2cPullupValue)
{
  int16_t hexoutx;
  int offset_av = 0;

  enableI2Cpins(menuI2cPullupValue);

  writeSensorRegisterMMA8451Q(0x2A, 0x01, menuI2cPullupValue);

  for(i = 0; i < off_len; i++)
  {

    readSensorRegisterMMA8451Q(0x01, 2);

    hexoutx = ((deviceMMA8451QState.i2cBuffer[0] & 0xFF) << 6) | (deviceMMA8451QState.i2cBuffer[1] >> 2);

    hexoutx = (hexoutx ^ (1 << 13)) - (1 << 13);

    offset_av += hexoutx;

  }

  offset_av /= off_len;

  return offset_av;
}


int
reduce_accel_array_resize_offset(int acc[], int length, int index)
{
  //int accel_fit;
  int i;

  if(length > 96)
  {
    float p;
    float q;

    p = 0;
    q = 0;

    for(i = 0; i < length; i++)
    {
      if(i/length < index/96 && (i + 1)/length > index/96)
      {
        p += acc[i]*(((i + 1)/length) - index/96);
        q += (((i + 1)/length) - index/96)/(1/length);
      }
      else if(i/length >= index/96 && (i + 1)/length < (index + 1)/96)
      {
        p += acc[i];
        q++;
      }
      else if(i/length < (index + 1)/96 && (i + 1)/length > (index + 1)/96)
      {
        p += acc[i]*((index + 1)/96 - ((i)/length));
        q += ((index + 1)/96 - ((i)/length))/(1/length);
      }

      if(i/length > (index + 1)/96)
      {
        break;
      }

    }


    return (int)((p/q) * 64)/(4096 * 2);


  }
  else
  {
    if(length > index)
    {
      return (int)(acc[index] *64/(4096 * 2));
    }
    else
    {
      return 0;
    }

  }

}
