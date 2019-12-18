#include <stdint.h>

#include "fsl_spi_master_driver.h"
#include "fsl_port_hal.h"

#include "SEGGER_RTT.h"
#include "gpio_pins.h"
#include "warp.h"
#include "devSSD1331.h"

volatile uint8_t	inBuffer[1];
volatile uint8_t	payloadBytes[1];
volatile int acc_prev[2] = {9, 9};


/*
 *	Override Warp firmware's use of these pins and define new aliases.
 */
enum
{
	kSSD1331PinMOSI		= GPIO_MAKE_PIN(HW_GPIOA, 8),
	kSSD1331PinSCK		= GPIO_MAKE_PIN(HW_GPIOA, 9),
	kSSD1331PinCSn		= GPIO_MAKE_PIN(HW_GPIOB, 13),
	kSSD1331PinDC		= GPIO_MAKE_PIN(HW_GPIOA, 12),
	kSSD1331PinRST		= GPIO_MAKE_PIN(HW_GPIOB, 0),
};

static int
writeCommand(uint8_t commandByte)
{
	spi_status_t status;

	/*
	 *	Drive /CS low.
	 *
	 *	Make sure there is a high-to-low transition by first driving high, delay, then drive low.
	 */
	GPIO_DRV_SetPinOutput(kSSD1331PinCSn);
	OSA_TimeDelay(10);
	GPIO_DRV_ClearPinOutput(kSSD1331PinCSn);

	/*
	 *	Drive DC low (command).
	 */
	GPIO_DRV_ClearPinOutput(kSSD1331PinDC);

	payloadBytes[0] = commandByte;
	status = SPI_DRV_MasterTransferBlocking(0	/* master instance */,
					NULL		/* spi_master_user_config_t */,
					(const uint8_t * restrict)&payloadBytes[0],
					(uint8_t * restrict)&inBuffer[0],
					1		/* transfer size */,
					1000		/* timeout in microseconds (unlike I2C which is ms) */);

	/*
	 *	Drive /CS high
	 */
	GPIO_DRV_SetPinOutput(kSSD1331PinCSn);

	return status;
}


//////////////////////////////////////////////////////////////////////////////


int
default_colour(void)
{
	writeCommand(0x00);
	writeCommand(0x3F);
	writeCommand(0x00);
	return 0;
}

int
recttest(void)
{
	writeCommand(kSSD1331CommandDRAWRECT);
	writeCommand(0x00);
	writeCommand(0x00);
	writeCommand(0x5F);
	writeCommand(0x3F);

	default_colour();
	default_colour();

	return 0;
}

int
drawone(uint8_t loc_x, uint8_t loc_y)
{
	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 21);
	writeCommand(loc_y);
	writeCommand(loc_x + 21);
	writeCommand(loc_y + 56);

	default_colour();

	return 0;
}

int
drawtwo(uint8_t loc_x, uint8_t loc_y)
{
	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y);
	writeCommand(loc_x + 42);
	writeCommand(loc_y);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 28);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 28);
	writeCommand(loc_x);
	writeCommand(loc_y + 28);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y + 28);
	writeCommand(loc_x);
	writeCommand(loc_y + 56);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y + 56);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 56);
	default_colour();

	return 0;
}


int
drawthree(uint8_t loc_x, uint8_t loc_y)
{
	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y);
	writeCommand(loc_x + 42);
	writeCommand(loc_y);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 56);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 56);
	writeCommand(loc_x);
	writeCommand(loc_y + 56);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y + 28);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 28);
	default_colour();

	return 0;

}


int
drawfour(uint8_t loc_x, uint8_t loc_y)
{
	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y);
	writeCommand(loc_x);
	writeCommand(loc_y + 42);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y + 42);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 42);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 21);
	writeCommand(loc_y + 28);
	writeCommand(loc_x + 21);
	writeCommand(loc_y + 56);
	default_colour();

	return 0;

}


int
drawfive(uint8_t loc_x, uint8_t loc_y)
{
	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y);
	writeCommand(loc_x);
	writeCommand(loc_y);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y);
	writeCommand(loc_x);
	writeCommand(loc_y + 28);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y + 28);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 28);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 28);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 56);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 56);
	writeCommand(loc_x);
	writeCommand(loc_y + 56);
	default_colour();

	return 0;
}


int
drawsix(uint8_t loc_x, uint8_t loc_y)
{
	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y);
	writeCommand(loc_x);
	writeCommand(loc_y);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y);
	writeCommand(loc_x);
	writeCommand(loc_y + 56);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y + 56);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 56);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 56);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 28);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 28);
	writeCommand(loc_x);
	writeCommand(loc_y + 28);
	default_colour();

	return 0;
}


int
drawseven(uint8_t loc_x, uint8_t loc_y)
{
		writeCommand(kSSD1331CommandDRAWLINE);
		writeCommand(loc_x);
		writeCommand(loc_y);
		writeCommand(loc_x + 42);
		writeCommand(loc_y);
		default_colour();

		writeCommand(kSSD1331CommandDRAWLINE);
		writeCommand(loc_x + 42);
		writeCommand(loc_y);
		writeCommand(loc_x + 42);
		writeCommand(loc_y + 56);
		default_colour();

		return 0;
}


int
draweight(uint8_t loc_x, uint8_t loc_y)
{
		writeCommand(kSSD1331CommandDRAWLINE);
		writeCommand(loc_x);
		writeCommand(loc_y);
		writeCommand(loc_x + 42);
		writeCommand(loc_y);
		default_colour();

		writeCommand(kSSD1331CommandDRAWLINE);
		writeCommand(loc_x + 42);
		writeCommand(loc_y);
		writeCommand(loc_x + 42);
		writeCommand(loc_y + 56);
		default_colour();

		writeCommand(kSSD1331CommandDRAWLINE);
		writeCommand(loc_x + 42);
		writeCommand(loc_y + 56);
		writeCommand(loc_x);
		writeCommand(loc_y + 56);
		default_colour();

		writeCommand(kSSD1331CommandDRAWLINE);
		writeCommand(loc_x);
		writeCommand(loc_y + 56);
		writeCommand(loc_x);
		writeCommand(loc_y);
		default_colour();

		writeCommand(kSSD1331CommandDRAWLINE);
		writeCommand(loc_x);
		writeCommand(loc_y + 28);
		writeCommand(loc_x + 42);
		writeCommand(loc_y + 28);
		default_colour();

		return 0;
}


int
drawnine(uint8_t loc_x, uint8_t loc_y)
{
	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y);
	writeCommand(loc_x);
	writeCommand(loc_y);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y);
	writeCommand(loc_x);
	writeCommand(loc_y + 28);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y + 28);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 28);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 56);
	default_colour();

	return 0;
}


int
drawzero(uint8_t loc_x, uint8_t loc_y)
{
	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y);
	writeCommand(loc_x);
	writeCommand(loc_y);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y);
	writeCommand(loc_x);
	writeCommand(loc_y + 56);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x);
	writeCommand(loc_y + 56);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 56);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y + 56);
	writeCommand(loc_x + 42);
	writeCommand(loc_y);
	default_colour();

	writeCommand(kSSD1331CommandDRAWLINE);
	writeCommand(loc_x + 42);
	writeCommand(loc_y);
	writeCommand(loc_x);
	writeCommand(loc_y + 56);
	default_colour();
	return 0;
}

//need to split Writing

int
writetoscreenupper(int upper)
{
	clearscreenupper();

	int x = 3;
	int y = 3;

	if(upper == 0)
	{
		drawzero(x, y);
	}
	else if(upper == 1)
	{
		drawone(x, y);
	}
	else if(upper == 2)
	{
		drawtwo(x, y);
	}
	else if(upper == 3)
	{
		drawthree(x, y);
	}
	else if(upper == 4)
	{
		drawfour(x, y);
	}
	else if(upper == 5)
	{
		drawfive(x, y);
	}
	else if(upper == 6)
	{
		drawsix(x, y);
	}
	else if(upper == 7)
	{
		drawseven(x, y);
	}
	else if(upper == 8)
	{
		draweight(x, y);
	}
	else if(upper == 9)
	{
		drawnine(x, y);
	}
	return 0;
}

int
writetoscreenlower(int lower)
{
	clearscreenlower();

	int x = 50;
	int y = 3;

	if(lower == 0)
	{
		drawzero(x, y);
	}
	else if(lower == 1)
	{
		drawone(x, y);
	}
	else if(lower == 2)
	{
		drawtwo(x, y);
	}
	else if(lower == 3)
	{
		drawthree(x, y);
	}
	else if(lower == 4)
	{
		drawfour(x, y);
	}
	else if(lower == 5)
	{
		drawfive(x, y);
	}
	else if(lower == 6)
	{
		drawsix(x, y);
	}
	else if(lower == 7)
	{
		drawseven(x, y);
	}
	else if(lower == 8)
	{
		draweight(x, y);
	}
	else if(lower == 9)
	{
		drawnine(x, y);
	}
	return 0;  // This can be compressed into 1 function but i'm to bored to write it.
}

int
writetoscreenboth(int ar[2])
{
	clearscreen();

	int i;

	int x[2] = {3, 50};
	int y[2] = {3, 3};

	for(i = 0; i < 2; i++)
	{
		if(ar[i] == 0)
		{
			drawzero(x[i], y[i]);
		}
		else if(ar[i] == 1)
		{
			drawone(x[i], y[i]);
		}
		else if(ar[i] == 2)
		{
			drawtwo(x[i], y[i]);
		}
		else if(ar[i] == 3)
		{
			drawthree(x[i], y[i]);
		}
		else if(ar[i] == 4)
		{
			drawfour(x[i], y[i]);
		}
		else if(ar[i] == 5)
		{
			drawfive(x[i], y[i]);
		}
		else if(ar[i] == 6)
		{
			drawsix(x[i], y[i]);
		}
		else if(ar[i] == 7)
		{
			drawseven(x[i], y[i]);
		}
		else if(ar[i] == 8)
		{
			draweight(x[i], y[i]);
		}
		else if(ar[i] == 9)
		{
			drawnine(x[i], y[i]);
		}
	}
	return 0;
}

int
writetoscreen(int ar[2]) //this bit is just to save processing as the the writing command is rather slow, so by reducing the amount need to be rewritten we can save time.
{
	if(ar[0] != acc_prev[0] && ar[1] != acc_prev[1])
	{
		writetoscreenboth(ar);
	}
	else if(ar[0] == acc_prev[0] &&  ar[1] != acc_prev[1])
	{
		writetoscreenlower(ar[1]);
	}
	else if (ar[0] != acc_prev[0] && ar[1] == acc_prev[1])
	{
		writetoscreenupper(ar[0]);
	}

	acc_prev[0] = ar[0]; // i fucjen hopes this works
	acc_prev[1] = ar[1];

	return 0;
}


int
clearscreen(void)
{
	/*
	 *	Clear Screen
	 */
	writeCommand(kSSD1331CommandCLEAR);
	writeCommand(0x00);
	writeCommand(0x00);
	writeCommand(0x5F);
	writeCommand(0x3F);

	return 0;
}


int
clearscreenupper(void)
{
	writeCommand(kSSD1331CommandCLEAR);
	writeCommand(0x00);
	writeCommand(0x00);
	writeCommand(0x2F);
	writeCommand(0x3F);

	return 0;
}

int
clearscreenlower(void)
{
	writeCommand(kSSD1331CommandCLEAR);
	writeCommand(0x30);
	writeCommand(0x00);
	writeCommand(0x5F);
	writeCommand(0x3F);

	return 0;
}




////////////////////////////////////////////////////////////////////////////////

int
devSSD1331init(void)
{
	/*
	 *	Override Warp firmware's use of these pins.
	 *
	 *	Re-configure SPI to be on PTA8 and PTA9 for MOSI and SCK respectively.
	 */
	PORT_HAL_SetMuxMode(PORTA_BASE, 8u, kPortMuxAlt3);
	PORT_HAL_SetMuxMode(PORTA_BASE, 9u, kPortMuxAlt3);

	enableSPIpins();

	/*
	 *	Override Warp firmware's use of these pins.
	 *
	 *	Reconfigure to use as GPIO.
	 */
	PORT_HAL_SetMuxMode(PORTB_BASE, 13u, kPortMuxAsGpio);
	PORT_HAL_SetMuxMode(PORTA_BASE, 12u, kPortMuxAsGpio);
	PORT_HAL_SetMuxMode(PORTB_BASE, 0u, kPortMuxAsGpio);


	/*
	 *	RST high->low->high.
	 */
	GPIO_DRV_SetPinOutput(kSSD1331PinRST);
	OSA_TimeDelay(100);
	GPIO_DRV_ClearPinOutput(kSSD1331PinRST);
	OSA_TimeDelay(100);
	GPIO_DRV_SetPinOutput(kSSD1331PinRST);
	OSA_TimeDelay(100);

	/*
	 *	Initialization sequence, borrowed from https://github.com/adafruit/Adafruit-SSD1331-OLED-Driver-Library-for-Arduino
	 */
	writeCommand(kSSD1331CommandDISPLAYOFF);	// 0xAE
	writeCommand(kSSD1331CommandSETREMAP);		// 0xA0
	writeCommand(0x72);				// RGB Color
	writeCommand(kSSD1331CommandSTARTLINE);		// 0xA1
	writeCommand(0x0);
	writeCommand(kSSD1331CommandDISPLAYOFFSET);	// 0xA2
	writeCommand(0x0);
	writeCommand(kSSD1331CommandNORMALDISPLAY);	// 0xA4
	writeCommand(kSSD1331CommandSETMULTIPLEX);	// 0xA8
	writeCommand(0x3F);				// 0x3F 1/64 duty
	writeCommand(kSSD1331CommandSETMASTER);		// 0xAD
	writeCommand(0x8E);
	writeCommand(kSSD1331CommandPOWERMODE);		// 0xB0
	writeCommand(0x0B);
	writeCommand(kSSD1331CommandPRECHARGE);		// 0xB1
	writeCommand(0x31);
	writeCommand(kSSD1331CommandCLOCKDIV);		// 0xB3
	writeCommand(0xF0);				// 7:4 = Oscillator Frequency, 3:0 = CLK Div Ratio (A[3:0]+1 = 1..16)
	writeCommand(kSSD1331CommandPRECHARGEA);	// 0x8A
	writeCommand(0x64);
	writeCommand(kSSD1331CommandPRECHARGEB);	// 0x8B
	writeCommand(0x78);
	writeCommand(kSSD1331CommandPRECHARGEA);	// 0x8C
	writeCommand(0x64);
	writeCommand(kSSD1331CommandPRECHARGELEVEL);	// 0xBB
	writeCommand(0x3A);
	writeCommand(kSSD1331CommandVCOMH);		// 0xBE
	writeCommand(0x3E);
	writeCommand(kSSD1331CommandMASTERCURRENT);	// 0x87
	writeCommand(0x0E);
	writeCommand(kSSD1331CommandCONTRASTA);		// 0x81
	writeCommand(0x91);
	writeCommand(kSSD1331CommandCONTRASTB);		// 0x82
	writeCommand(0xFF);
	writeCommand(kSSD1331CommandCONTRASTC);		// 0x83
	writeCommand(0x7D);
	writeCommand(kSSD1331CommandDISPLAYON);		// Turn on oled panel

	/*
	 *	To use fill commands, you will have to issue a command to the display to enable them. See the manual.
	 */
	writeCommand(kSSD1331CommandFILL);
	writeCommand(0x01);


	clearscreen();


	/*
	 *	Any post-initialization drawing commands go here.
	 */
	//...
	//3,3
	//50, 3
	int bv[2] = {0, 0};
	writetoscreen(bv);

	return 0;
}
