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
/// @file iotbus_i2c.c
/// @brief hardware interface for iotbus main loop
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include "iotbus_i2c.h"
#include "../include/iotbus_error.h"

int i2c_open(int bus, int *file_hndl)
{
	int fd;
	char i2c_dev[I2C_BUFFER_MAX] = {0,};

	snprintf(i2c_dev, sizeof(i2c_dev)-1, SYSFS_I2C_DIR"-%d", bus);

	fd = open(i2c_dev, O_RDWR);
	
	if (fd < 0) {
		fprintf(stderr, "Can't Open /dev/i2c-%d : %s\n", bus, strerror(errno));
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	*file_hndl = fd;
	//*file_hndl = bus;

	//close(fd);

	return IOTBUS_ERROR_NONE;
}

int i2c_close(int file_hndl)
{
	if(file_hndl == (int)NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	close(file_hndl);
	
	return IOTBUS_ERROR_NONE;
}

int i2c_set_frequency(int file_hndl, i2c_mode_e speed)
{
	int status;
	int fd;
	char i2c_dev[I2C_BUFFER_MAX] = {0,};

	snprintf(i2c_dev, sizeof(i2c_dev)-1, SYSFS_I2C_DIR"-%d", file_hndl);

	fd = open(i2c_dev, O_RDWR);

	int frequency = 0;
	
	if(fd == (int)NULL)
		return IOTBUS_ERROR_INVALID_PARAMETER;

	if (speed == I2C_STD) {
		frequency = 10000;
	}
	else if (speed == I2C_FAST) {
		frequency = 400000;
	}
	else if (speed == I2C_HIGH) {
		frequency = 3400000;
	}
	else {
		fprintf(stderr, "Error: speed is not supported [%d]\n", speed);
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	status = ioctl(fd, I2C_FREQUENCY, (unsigned long)((unsigned int*)&frequency));
	
	if (status < 0) {
		fprintf(stderr, "Error  I2C_FREQUENCY, speed[%d]:\n",  speed);
		return IOTBUS_ERROR_UNKNOWN;
	}
	close(fd);
	return IOTBUS_ERROR_NONE;
}

int i2c_set_address(int file_hndl, int address)
{
	int status;

	printf("I2C SLAVE address = [%x]\n", address);

	status = ioctl(file_hndl, I2C_SLAVE, address);

	if (status < 0) {
		fprintf(stderr, "Error I2C_SLAVE, address[%x]:\n", address);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

int i2c_read(int file_hndl, unsigned char *data, int length, int addr)
{
	int status;
	//int fd;
	//char i2c_dev[I2C_BUFFER_MAX] = {0,};

	//snprintf(i2c_dev, sizeof(i2c_dev)-1, SYSFS_I2C_DIR"-%d", file_hndl);

	//fd = open(i2c_dev, O_RDWR);

	//if (fd < 0) {
	//	fprintf(stderr, "Can't Open /dev/i2c-%d : %s\n", file_hndl, strerror(errno));
	//	return IOTBUS_ERROR_INVALID_PARAMETER;
	//}
	//status = ioctl(fd, I2C_SLAVE, addr);
	//if (status < 0) {
	//	fprintf(stderr, "Error I2C_SLAVE, address[%x]:\n", addr);
	//	return IOTBUS_ERROR_UNKNOWN;
	//}
	printf("[Read] file_hndle = %d\n", file_hndl);
	status = read(file_hndl, data, length);
	
	if (status != length) {
		fprintf(stderr, "i2c transaction read failed\n");
		return IOTBUS_ERROR_UNKNOWN;
	}
	else
		printf("[SUCCESS] data[%02x][%02x]\n", data[0], data[1]);

	//close(fd);

	return IOTBUS_ERROR_NONE;
}

int i2c_write(int file_hndl, const unsigned char *data, int length, int addr)
{
	int status;
	//int fd;
	//char i2c_dev[I2C_BUFFER_MAX] = {0,};

	//snprintf(i2c_dev, sizeof(i2c_dev)-1, SYSFS_I2C_DIR"-%d", file_hndl);

	//fd = open(i2c_dev, O_RDWR);

	//if (fd < 0) {
	//	fprintf(stderr, "Can't Open /dev/i2c-%d : %s\n", file_hndl, strerror(errno));
	//	return IOTBUS_ERROR_INVALID_PARAMETER;
	//}

//	status = ioctl(fd, I2C_SLAVE, addr);
	//if (status < 0) {
	//	fprintf(stderr, "Error I2C_SLAVE, address[%x]:\n", addr);
	//	return IOTBUS_ERROR_UNKNOWN;
	//}

	status = write(file_hndl, data, length);

	if (status != length) {
		fprintf(stderr, "i2c transaction wrtie failed \n");
	//	close(fd);
		return IOTBUS_ERROR_UNKNOWN;
	}

	//close(fd);

	return IOTBUS_ERROR_NONE;
}
