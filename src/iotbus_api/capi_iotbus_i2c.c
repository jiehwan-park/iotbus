//******************************************************************
//
// Copyright 2014 Samsung Electronics All Rights Reserved.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
/// @file iotbus_gpio.c
/// @brief Iotbus APIs for Gpio

#include "capi_iotbus_i2c.h"
#include "capi_iotbus_error.h"
#include "capi_iotbus.h"
#include "capi_iotbus_dbus.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_NAME	"i2c"
int I2C_Addr = 0;
/************************************************************************************
 * Name: iotbus_i2c_init
 *
 * Description:
 *   Initialize one I2C bus
 *
 ************************************************************************************/
API iotbus_i2c_context_h capi_iotbus_i2c_init(int bus)
{
	iotbus_i2c_context_h dev;

	int ret = IOTBUS_ERROR_NONE;

	/* Initialize iotbus_i2c_context_h */
	dev = (iotbus_i2c_context_h)malloc(sizeof(struct _iotbus_i2c_s));

	if(dev == NULL)
		return NULL;

	ret = capi_iotbus_dbus_i2c(dev, I2C_NAME, "INIT", bus, 0, I2C_Addr );

	if(ret != IOTBUS_ERROR_NONE)
	{
		free(dev);
		printf("[IOTBUS] I2C init error\n");
		dev = NULL;
	}

	return dev;
}

/************************************************************************************
 * Name: iotbus_i2c_stop
 *
 * Description:
 *   Uninitialize an I2C bus
 *
 ************************************************************************************/

API int capi_iotbus_i2c_stop(iotbus_i2c_context_h dev)
{
	int ret = IOTBUS_ERROR_NONE;
	/* Free iotbus_i2c_context_h */

	if(dev != NULL)
	{
		ret = capi_iotbus_dbus_i2c(dev, I2C_NAME, "STOP", 0, 0, I2C_Addr );

		free(dev);
		dev = NULL;
	}

	return ret;
}


/************************************************************************************
 * Name: i2c_setfrequency
 *
 * Description:
 *   Set the I2C frequency
 *
 ************************************************************************************/
API int capi_iotbus_i2c_set_frequency(iotbus_i2c_context_h dev, iotbus_i2c_mode_e mode)
{

	/* Set the clocking for the selected frequency */

	return capi_iotbus_dbus_i2c(dev, I2C_NAME, "SET_FREQ", mode, 0, I2C_Addr );


}

/************************************************************************************
 * Name: i2c_setaddress
 *
 * Description:
 *   Set the I2C slave address
 *
 ************************************************************************************/
API int capi_iotbus_i2c_set_address(iotbus_i2c_context_h dev, int address)
{
	/* Set the i2c slave address */

	//I2C_Addr = address;
	return capi_iotbus_dbus_i2c(dev, I2C_NAME, "SET_ADDR", address, 0, I2C_Addr );
	//return IOTBUS_ERROR_NONE;

}

/************************************************************************************
 * Name: cc2538_i2c_read
 *
 * Description:
 *   Read I2C data
 *
 ************************************************************************************/
API int capi_iotbus_i2c_read(iotbus_i2c_context_h dev, uint8_t *data, int length)
{

 	/* Read i2c data */
	return capi_iotbus_dbus_i2c(dev, I2C_NAME, "READ", length, data, I2C_Addr);

 }

/************************************************************************************
 * Name: i2c_write
 *
 * Description:
 *   Write I2C data
 *
 ************************************************************************************/
API int capi_iotbus_i2c_write(iotbus_i2c_context_h dev, const uint8_t *data, int length)
{

	/* Write i2c data */
	return capi_iotbus_dbus_i2c(dev, I2C_NAME, "WRITE", length, data, I2C_Addr);

}


#ifdef __cplusplus
}
#endif
