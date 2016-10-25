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

#include "capi_iotbus_adc.h"
#include "capi_iotbus_error.h"
#include "capi_iotbus.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "iotbus_adc.h"


iotbus_adc_context_h capi_iotbus_adc_open (int channel)
{
	iotbus_adc_context_h dev = NULL;

	dev = (iotbus_adc_context_h)malloc(sizeof(struct _iotbus_adc_s) + DEVICE_NAME_SIZE);

	dev->channel = channel;
	if (adc_get_device_name(dev->device_name) != IOTBUS_ERROR_NONE)
	{
		free(dev);
		dev = NULL;
	}

	return dev;
}

int capi_iotbus_adc_read(iotbus_adc_context_h dev, int *data)
{
	if (dev == NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;
	if (data == NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	return adc_get_data(dev->channel, dev->device_name, data);

}

int capi_iotbus_adc_close (iotbus_adc_context_h dev)
{
	if (dev != NULL)
		free(dev);

	dev = NULL;

	return IOTBUS_ERROR_NONE;
}
