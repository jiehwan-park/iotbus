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

#include "capi_iotbus_gpio.h"
#include "capi_iotbus_error.h"
#include "capi_iotbus_dbus.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "capi_iotbus.h"
#include "iotbus_gpio.h"
#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes gpio_context, based on Gpio pin.
 */

#define GPIO_NAME	"gpio"

API iotbus_gpio_context_h  capi_iotbus_gpio_open (int gpio_pin)
{
	iotbus_gpio_context_h dev = NULL;
	int ret = IOTBUS_ERROR_NONE;

	/* Initialize */
	dev = (iotbus_gpio_context_h)malloc(sizeof(struct _iotbus_gpio_s));

	if (dev == NULL)
	{
		return NULL;
	}

	dev->pin = gpio_pin;

	printf("GPIO_OPEN!!! \n");

	ret = capi_iotbus_dbus_gpio(dev, GPIO_NAME, "OPEN", 0 , 0);

	if(ret != IOTBUS_ERROR_NONE)
	{
		free(dev);
		dev = NULL;
	}

	return dev;
}

/**
 * @brief Closes the gpio_context.
 */
API int capi_iotbus_gpio_close (iotbus_gpio_context_h dev)
{
	int ret = IOTBUS_ERROR_NONE;
	/* check validation of gpio context handle */

	/* call gpio_close */

	ret = capi_iotbus_dbus_gpio(dev, GPIO_NAME, "CLOSE", 0 , 0);

	if (dev != NULL)
		free(dev);

	dev = NULL;
	
	return ret;
}

/**
 * @brief Sets Gpio direction.
 */
API int capi_iotbus_gpio_set_direction (iotbus_gpio_context_h dev, iotbus_gpio_direction_e dir)
{
	int ret = IOTBUS_ERROR_NONE;

	/* check resource capability */

	if (dir >= IOTBUS_GPIO_DIRECTION_MAX)
	{
		ret = IOTBUS_ERROR_NOT_SUPPORTED;
	}
	else
	{
		if (dev->dir != dir)
		{
			dev->dir = dir;
			ret = capi_iotbus_dbus_gpio(dev, GPIO_NAME, "SET_DIR", 0 , 0);
		}
	}
	/* call gpio_set_direction */

	return ret;
}

/**
 * @brief Sets the edge mode on the Gpio.
 */
API int capi_iotbus_gpio_set_edge_mode (iotbus_gpio_context_h dev, iotbus_gpio_edge_e edge)
{
	int ret = IOTBUS_ERROR_NONE;

	/* check resource capability */

	if (edge >= IOTBUS_GPIO_EDGE_MAX)
	{
		ret = IOTBUS_ERROR_NOT_SUPPORTED;
	}
	else
	{
		if (dev->edge != edge)
		{
			dev->edge = edge;
			ret = capi_iotbus_dbus_gpio(dev, GPIO_NAME, "SET_EDGE", 0 , 0);
		}
	}
	/* call gpio_set_edge_mode */

	return ret;
}

/**
 * @brief Sets Gpio output mode.
 */
API int capi_iotbus_gpio_set_drive_mode (iotbus_gpio_context_h dev, iotbus_gpio_drive_e drive)
{
	/* check resource capability */
	/* call gpio_set_drive_mode */

	return IOTBUS_ERROR_NOT_SUPPORTED;
}

/**
 * @brief Registers event handler callback for interrupt.
 */
API int capi_iotbus_gpio_register_cb (iotbus_gpio_context_h dev, iotbus_gpio_edge_e edge, gpio_isr_cb isr_cb, void *user_data)
{
	//TODO
	return IOTBUS_ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief Unregisters event handler callback for interrupt.
 */
API int capi_iotbus_gpio_unregister_cb (iotbus_gpio_context_h dev)
{
	//TODO
	return IOTBUS_ERROR_NOT_IMPLEMENTED;
}

/**
 * @brief Reads the gpio value.
 */
API int capi_iotbus_gpio_read (iotbus_gpio_context_h dev)
{
	int state = 0;
	int ret = IOTBUS_ERROR_NONE;
	/* check resource capability */
	ret = capi_iotbus_dbus_gpio(dev, GPIO_NAME, "READ", 0, &state);
	/* call gpio_read */

	return state;
}

/**
 * @brief Writes to the gpio value.
 */
API int capi_iotbus_gpio_write (iotbus_gpio_context_h dev, int value)
{
	int ret = IOTBUS_ERROR_NONE;
	/* check resource capability */
	ret = capi_iotbus_dbus_gpio(dev, GPIO_NAME, "WRITE", value , 0);
	/* call gpio_write */
	return ret;
}

/**
 * @brief Gets a direction of the Gpio.
 */
API int capi_iotbus_gpio_get_direction (iotbus_gpio_context_h dev, iotbus_gpio_direction_e *dir)
{
	int ret = IOTBUS_ERROR_NONE;
	/* check resource capability */

	ret = capi_iotbus_dbus_gpio(dev, GPIO_NAME, "GET_DIR", 0 , 0);

	if(ret == IOTBUS_ERROR_NONE)
	{
		(*dir) = dev->dir;
	}
	/* call gpio_get_direction */
	return ret;
}

/**
 * @brief Gets a pin number of the Gpio.
 */
API int capi_iotbus_gpio_get_pin (iotbus_gpio_context_h dev)
{
	/* check resource capability */
	if (!dev)
	{
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	return dev->pin;
	/* call gpio_get_pin */
}

/**
 * @brief Gets a edge mode of the Gpio.
 */
API int capi_iotbus_gpio_get_edge_mode (iotbus_gpio_context_h dev, iotbus_gpio_edge_e *edge)
{
	int ret = IOTBUS_ERROR_NONE;
	/* check resource capability */
	//ret = gpio_get_edge_mode(dev->pin, edge);

	ret = capi_iotbus_dbus_gpio(dev, GPIO_NAME, "GET_EDGE", 0 , 0);

	if(ret == IOTBUS_ERROR_NONE)
	{
		(*edge) = dev->edge;
	}
	/* call gpio_get_direction */
	return ret;
}

/**
 * @brief Gets a drive mode of the Gpio.
 */
API int capi_iotbus_gpio_get_drive_mode (iotbus_gpio_context_h dev, iotbus_gpio_drive_e *drive)
{
	/* check resource capability */

	/* call gpio_get_drive_mode */
	return IOTBUS_ERROR_NOT_SUPPORTED;
}



#ifdef __cplusplus
}
#endif
