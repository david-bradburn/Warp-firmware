/*
	Authored 2016-2018. Phillip Stanley-Marbell, Youchao Wang.

	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	*	Redistributions of source code must retain the above
		copyright notice, this list of conditions and the following
		disclaimer.

	*	Redistributions in binary form must reproduce the above
		copyright notice, this list of conditions and the following
		disclaimer in the documentation and/or other materials
		provided with the distribution.

	*	Neither the name of the author nor the names of its
		contributors may be used to endorse or promote products
		derived from this software without specific prior written
		permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
	FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
	COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.
*/
#include <stdlib.h>

#include "fsl_misc_utilities.h"
#include "fsl_device_registers.h"
#include "fsl_i2c_master_driver.h"
#include "fsl_spi_master_driver.h"
#include "fsl_rtc_driver.h"
#include "fsl_clock_manager.h"
#include "fsl_power_manager.h"
#include "fsl_mcglite_hal.h"
#include "fsl_port_hal.h"

#include "gpio_pins.h"
#include "SEGGER_RTT.h"
#include "warp.h"



extern volatile WarpI2CDeviceState	deviceAMG8834State;
extern volatile uint32_t		gWarpI2cBaudRateKbps;
extern volatile uint32_t		gWarpI2cTimeoutMilliseconds;
extern volatile uint32_t		gWarpSupplySettlingDelayMilliseconds;


/*
 *	AMG8834.
 */
void
initAMG8834(const uint8_t i2cAddress, WarpI2CDeviceState volatile *  deviceStatePointer)
{
	deviceStatePointer->i2cAddress	= i2cAddress;
	deviceStatePointer->signalType	= (
						kWarpTypeMaskTemperature
					);
	return;
}

WarpStatus
writeSensorRegisterAMG8834(uint8_t deviceRegister, uint8_t payload, uint16_t menuI2cPullupValue)
{
	uint8_t		payloadByte[1], commandByte[1];
	i2c_status_t	returnValue;

	switch (deviceRegister)
	{
		case 0x00: case 0x01: case 0x02: case 0x03:
		case 0x05: case 0x07: case 0x08: case 0x09:
		case 0x0A: case 0x0B: case 0x0C: case 0x0D:
		{
			/* OK */
			break;
		}
		
		default:
		{
			return kWarpStatusBadDeviceCommand;
		}
	}

	i2c_device_t slave =
	{
		.address = deviceAMG8834State.i2cAddress,
		.baudRate_kbps = gWarpI2cBaudRateKbps
	};

	enableI2Cpins(menuI2cPullupValue);

	/*
	 *	Wait for supply and pull-ups to settle.
	 */
	OSA_TimeDelay(gWarpSupplySettlingDelayMilliseconds);

	commandByte[0] = deviceRegister;
	payloadByte[0] = payload;
	returnValue = I2C_DRV_MasterSendDataBlocking(
							0 /* I2C instance */,
							&slave,
							commandByte,
							1,
							payloadByte,
							1,
							gWarpI2cTimeoutMilliseconds);
	if (returnValue != kStatus_I2C_Success)
	{
		return kWarpStatusDeviceCommunicationFailed;
	}

	return kWarpStatusOK;
}

WarpStatus
configureSensorAMG8834(uint8_t payloadConfigReg, uint8_t payloadFrameRateReg, uint8_t menuI2cPullupValue)
{
	WarpStatus	i2cWriteStatus1, i2cWriteStatus2;
	i2cWriteStatus1 = writeSensorRegisterAMG8834(kWarpSensorAMG8834Configuration /* register address configuration register */,
							payloadConfigReg /* payload: 3F initial reset */,
							menuI2cPullupValue);

	i2cWriteStatus2 = writeSensorRegisterAMG8834(kWarpSensorAMG8834FrameRate /* register address frame rate register */,
							payloadFrameRateReg /* payload: 1 FPS */,
							menuI2cPullupValue);

	return (i2cWriteStatus1 | i2cWriteStatus2);
}

WarpStatus
readSensorRegisterAMG8834(uint8_t deviceRegister)
{
	uint8_t 	cmdBuf[1]	= {0xFF};
	i2c_status_t	status;


	if (deviceRegister > 0xFF)
	{
		return kWarpStatusBadDeviceCommand;
	}

	i2c_device_t slave =
	{
		.address = deviceAMG8834State.i2cAddress,
		.baudRate_kbps = gWarpI2cBaudRateKbps
	};

	cmdBuf[0] = deviceRegister;

	status = I2C_DRV_MasterReceiveDataBlocking(
							0 /* I2C peripheral instance */,
							&slave,
							cmdBuf,
							1,
							(uint8_t *)deviceAMG8834State.i2cBuffer,
							1,
							gWarpI2cTimeoutMilliseconds);

	if (status != kStatus_I2C_Success)
	{
		return kWarpStatusDeviceCommunicationFailed;
	}

	return kWarpStatusOK;
}

void
printSensorDataAMG8834(bool hexModeFlag)
{
	uint8_t		readSensorRegisterValueLSB;
	uint8_t		readSensorRegisterValueMSB;
	uint16_t	readSensorRegisterValueCombined;
	WarpStatus	i2cReadStatusLow, i2cReadStatusHigh;

	for (uint16_t bufAddress = kWarpSensorAMG8834T01L; bufAddress <= kWarpSensorAMG8834T64H; bufAddress = bufAddress + 2)
	{
		i2cReadStatusLow		= readSensorRegisterAMG8834(bufAddress);
		readSensorRegisterValueLSB	= deviceAMG8834State.i2cBuffer[0];
		i2cReadStatusHigh		= readSensorRegisterAMG8834(bufAddress + 1);
		readSensorRegisterValueMSB	= deviceAMG8834State.i2cBuffer[0];
		readSensorRegisterValueCombined	= ((readSensorRegisterValueMSB & 0xFF)<<8) + (readSensorRegisterValueLSB & 0xFF);

		if ((i2cReadStatusLow != kWarpStatusOK) || (i2cReadStatusHigh != kWarpStatusOK))
		{
			SEGGER_RTT_WriteString(0, " ----,");
		}
		else
		{
			if (hexModeFlag)
			{
				SEGGER_RTT_printf(0, " 0x%02x 0x%02x,", readSensorRegisterValueMSB, readSensorRegisterValueLSB);
			}
			else
			{
				SEGGER_RTT_printf(0, " %d,", readSensorRegisterValueCombined);
			}
		}
	}

	i2cReadStatusLow		= readSensorRegisterAMG8834(kWarpSensorAMG8834TTHL);
	readSensorRegisterValueLSB	= deviceAMG8834State.i2cBuffer[0];
	i2cReadStatusHigh		= readSensorRegisterAMG8834(kWarpSensorAMG8834TTHH);
	readSensorRegisterValueMSB	= deviceAMG8834State.i2cBuffer[0];
	readSensorRegisterValueCombined	= ((readSensorRegisterValueMSB & 0xFF)<<8) + (readSensorRegisterValueLSB & 0xFF);

	if ((i2cReadStatusLow != kWarpStatusOK) || (i2cReadStatusHigh != kWarpStatusOK))
	{
		SEGGER_RTT_WriteString(0, " ----,");
	}
	else
	{
		if (hexModeFlag)
		{
			SEGGER_RTT_printf(0, " 0x%02x 0x%02x,", readSensorRegisterValueMSB, readSensorRegisterValueLSB);
		}
		else
		{
			SEGGER_RTT_printf(0, " %d,", readSensorRegisterValueCombined);
		}
	}
}