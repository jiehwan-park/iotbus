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
/// @file iotbus_i2c.h
/// @brief hardware interface for iotbus main loop
*/
#ifndef _IOTBUS_I2C_H_
#define _IOTBUS_I2C_H_

/* Define *******************************************************************/
#define SYSFS_I2C_DIR "/dev/i2c"
#define I2C_BUFFER_MAX 64
#define I2C_FREQUENCY 0X801
#define I2C_SLAVE 0x0703

typedef enum {
	I2C_STD = 0,
	I2C_FAST = 1,
	I2C_HIGH = 2
} i2c_mode_e;

int i2c_open(int bus, int *file_hndl);
int i2c_close(int file_hndl);
int i2c_set_frequency(int file_hndl, i2c_mode_e speed);
int i2c_set_address(int file_hndl, int address);
int i2c_read(int file_hndl, unsigned char *data, int length, int addr);
int i2c_write(int file_hndl, const unsigned char *data, int length, int addr);

#endif/*_IOTBUS_I2C_H*/
