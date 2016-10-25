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
/// @file iotbus_error.h
/// @brief Definition of iotbus_error

#ifndef IOTBUS_SPI_H
#define IOTBUS_SPI_H

typedef enum
{
	IOTBUS_SPI_MODE0 = 0,
	IOTBUS_SPI_MODE1,
	IOTBUS_SPI_MODE2,
	IOTBUS_SPI_MODE3
} iotbus_spi_mode_e;

struct iotbus_spi_config_s{
	int fd;
	char bits_per_word;
	int lsb;
	unsigned int chip_select;
	unsigned int frequency;
	iotbus_spi_mode_e mode;
};

typedef struct iotbus_spi_config_s * iotbus_spi_context_h;

iotbus_spi_context_h capi_iotbus_spi_open (unsigned int bus, iotbus_spi_context_h config);

int	capi_iotbus_spi_write (iotbus_spi_context_h hnd, char *txbuf, int length);

int	capi_iotbus_spi_recv (iotbus_spi_context_h hnd, char *rxbuf, int length);

int capi_iotbus_spi_transfer_buf (iotbus_spi_context_h hnd, char *txbuf, char *rxbuf, int length);

int	capi_iotbus_spi_close (iotbus_spi_context_h hnd);

#endif /* IOTBUS_SPI_H */
