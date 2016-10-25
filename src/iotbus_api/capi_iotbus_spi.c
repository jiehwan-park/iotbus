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

#include "capi_iotbus_spi.h"
#include "capi_iotbus_error.h"
#include "capi_iotbus.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "iotbus_spi.h"


API iotbus_spi_context_h capi_iotbus_spi_open (unsigned int bus, iotbus_spi_context_h config)
{

	iotbus_spi_context_h dev = NULL;
	int fd = 0;
	int ret = IOTBUS_ERROR_NONE;

	ret = spi_open(bus, &fd);

	if(ret != IOTBUS_ERROR_NONE)
		return NULL;

	dev = (iotbus_spi_context_h)malloc(sizeof(struct iotbus_spi_config_s));

	if(dev == NULL)
		return NULL;

	dev->fd = fd;


	return dev;

}

API int	capi_iotbus_spi_write (iotbus_spi_context_h hnd, char *txbuf, int length)
{
	return IOTBUS_ERROR_NONE;
}


API int	capi_iotbus_spi_recv (iotbus_spi_context_h hnd, char *rxbuf, int length)
{
	return IOTBUS_ERROR_NONE;
}

API int capi_iotbus_spi_transfer_buf (iotbus_spi_context_h hnd, char *txbuf, char *rxbuf, int length)
{
	return IOTBUS_ERROR_NONE;
}

API int	capi_iotbus_spi_close (iotbus_spi_context_h hnd)
{
	return IOTBUS_ERROR_NONE;
}
