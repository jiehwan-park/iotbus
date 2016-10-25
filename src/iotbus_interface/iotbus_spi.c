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
/// @file iotbus_spi.c
/// @brief hardware interface for iotbus main loop
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include "iotbus_spi.h"
#include "../include/iotbus_error.h"

int spi_open(unsigned int bus, int *file_hndl)
{
	int fd = 0;
	char spi_dev[SPI_BUFFER_MAX] = {0,};

	snprintf(spi_dev, sizeof(spi_dev)-1, SYSFS_SPI_DIR"%d", bus);
	
	fd = open(spi_dev, O_RDWR);
	if (file_hndl < 0) {
		fprintf(stderr, "Can't Open /dev/spidev%d.0 :%s\n", bus, strerror(errno));
		return IOTBUS_ERROR_UNKNOWN;
	}

	*file_hndl = fd;

	return IOTBUS_ERROR_NONE;
}

int spi_close(int file_hndl)
{
	if (file_hndl == (int)NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	close(file_hndl);

	return IOTBUS_ERROR_NONE;
}

int spi_set_mode(int file_hndl, spi_mode_e mode)
{
	int status;

	if (file_hndl == (int)NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	status = ioctl(file_hndl, SPI_IOC_WR_MODE, &mode);
	if (status < 0) {
		fprintf(stderr, "Error: spi_set_mode %d fail\n", mode);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

int spi_set_bits(int file_hndl, int bits)
{
	int status;

	if (file_hndl == (int)NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	status = ioctl(file_hndl, SPI_IOC_WR_BITS_PER_WORD, &bits);

	if (status < 0) {
		fprintf(stderr, "Error: spi_set_bits %d fail\n", bits);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

int spi_set_frequency(int file_hndl, int freq)
{
	int status;

	if (file_hndl == (int)NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	status = ioctl(file_hndl, SPI_IOC_WR_MAX_SPEED_HZ, &freq);

	if (status < 0) {
		fprintf(stderr, "Error: spi_set_frequency %d fail\n", freq);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

int spi_set_chip_select(int file_hndl, int cs)
{
	int mode = 0;
	int status;

	if (file_hndl == (int)NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	status = ioctl(file_hndl, SPI_IOC_RD_MODE, &mode);
	if (status < 0) {
		fprintf(stderr, "Error: Get: spi_set_chip_select fail\n");
		return IOTBUS_ERROR_UNKNOWN;
	}

	if (cs == 1){
		mode |= SPI_CS_HIGH;	
	}
	else{
		mode &= SPI_CS_HIGH;
	}

	printf("mode[%x]\n", mode);
	
	status = ioctl(file_hndl, SPI_IOC_WR_MODE, &mode);
	if (status < 0) {
		fprintf(stderr, "Error: Set : spi_set_chip_select %d fail\n", mode);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

int spi_get_mode(int file_hndl, spi_mode_e *mode)
{
	int value = 0;
	int status;

	if (file_hndl == (int)NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	status = ioctl(file_hndl, SPI_IOC_RD_MODE, &value);
	if (status < 0) {
		fprintf(stderr, "Error: spi_get_mode fail\n");
		return IOTBUS_ERROR_UNKNOWN;
	}

	*mode = value;

	return IOTBUS_ERROR_NONE;
}

int spi_get_bits(int file_hndl, int *bits)
{
	int value = 0;
	int status;

	if (file_hndl == (int)NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	status = ioctl(file_hndl, SPI_IOC_RD_BITS_PER_WORD, &value);
	if (status < 0) {
		fprintf(stderr, "Error: spi_get_bits fail\n");
		return IOTBUS_ERROR_UNKNOWN;
	}

	*bits = value;

	return IOTBUS_ERROR_NONE;
}

int spi_get_frequency(int file_hndl, int *freq)
{
	int value = 0;
	int status;

	if (file_hndl == (int)NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	status = ioctl(file_hndl, SPI_IOC_RD_MAX_SPEED_HZ, &value);

	if (status < 0) {
		fprintf(stderr, "Error: spi_get_frequency fail\n");
		return IOTBUS_ERROR_UNKNOWN;
	}

	*freq = value;

	return IOTBUS_ERROR_NONE;
}

int spi_get_lsb(int file_hndl, int *lsb)
{
	int value = 0;
	int status;

	if (file_hndl == (int)NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	status = ioctl(file_hndl, SPI_IOC_RD_LSB_FIRST, &value);

	if (status < 0) {
		fprintf(stderr, "Error: spi_get_lsb fail\n");
		return IOTBUS_ERROR_UNKNOWN;
	}

	*lsb = value;

	return IOTBUS_ERROR_NONE;
}

int spi_write_data(int file_hndl, char *txbuf, int length)
{
	int status;
	struct spi_ioc_transfer xfer;

	if (file_hndl == (int)NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	memset(&xfer, 0, sizeof(xfer));
	xfer.tx_buf = (unsigned long)txbuf;
	xfer.len = length;

	status = ioctl(file_hndl, SPI_IOC_MESSAGE(1), xfer);
	if (status < 0) {
		fprintf(stderr, "Error: spi_write_data fail\n");
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

int spi_read_data(int file_hndl, char *rxbuf, int length)
{
	int status;
	struct spi_ioc_transfer xfer;

	if (file_hndl == (int)NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	memset(&xfer, 0, sizeof(xfer));
	xfer.rx_buf = (unsigned long)rxbuf;
	xfer.len = length;

	status = ioctl(file_hndl, SPI_IOC_MESSAGE(1), &xfer);
	if (status < 0) {
		fprintf(stderr, "Error: spi_read_data fail\n");
		return IOTBUS_ERROR_UNKNOWN;
	}
	return IOTBUS_ERROR_NONE;
}

int spi_exchange_data(int file_hndl, char *txbuf, char *rxbuf, int length)
{
	int status;
	struct spi_ioc_transfer xfer[2];

	if (file_hndl == (int)NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	if (!txbuf || length < 0)
		return -1;

	memset(xfer, 0, sizeof(xfer));
	xfer[0].tx_buf = (unsigned long)txbuf;
	xfer[0].len = length;

	xfer[1].rx_buf = (unsigned long)rxbuf;
	xfer[1].len = length;

	status = ioctl(file_hndl, SPI_IOC_MESSAGE(2), xfer);
	if (status < 0) {
		fprintf(stderr, "Error: spi_exchange_data fail\n");
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}
