/*
//
// Copyright 2016 Samsung Electronics All Rights Reserved.
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
/// @file iotbus_spi.h
/// @brief hardware interface for iotbus main loop
*/
#ifndef _IOTBUS_SPI_H_
#define _IOTBUS_SPI_H_
#define SYSFS_SPI_DIR "/dev/spidev"
#define SPI_BUFFER_MAX 64
#define SPI_CR1_LSBFIRST          (1 << 7)  /* Bit 7: Frame Format */

typedef enum {
	SPI_MODE0 = 0,
	SPI_MODE1,
	SPI_MODE2,
	SPI_MODE3
} spi_mode_e;

int spi_open(unsigned int bus, int *file_hndl);
int spi_close(int file_hndl);
int spi_set_mode(int file_hndl, spi_mode_e mode);
int spi_set_bits(int file_hndl, int bits);
int spi_set_frequency(int file_hndl, int freq);
int spi_set_chip_select(int file_hndl, int cs);
int spi_get_mode(int file_hndl, spi_mode_e *mode);
int spi_get_bits(int file_hndl, int *bits);
int spi_get_frequency(int file_hndl, int *freq);
int spi_get_lsb(int file_hndl, int *lsb);

int spi_write_data(int file_hndl, char *txbuf, int length);
int spi_read_data(int file_hndl, char *rxbuf, int length);
int spi_exchange_data(int file_hndl, char *txbuf, char *rxbuf, int length);

#endif/*_IOTBUS_SPI_H*/