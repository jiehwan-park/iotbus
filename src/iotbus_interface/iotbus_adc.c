/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "iotbus_adc.h"
#include "../include/iotbus_error.h"

#define SYSFS_ADC_PATH     "/sys/bus/iio/devices/iio:device"

#define PATH_BUF_MAX    64
#define ADC_BUF_MAX     16

int adc_get_device_name(char *devName)
{
	int fd;
	int device = 0;	/* for get adc device name, /sys/bus/iio/devices/iio:device"0" */
	char fName[PATH_BUF_MAX] = {0};

	sprintf(fName, "%s%d%s", SYSFS_ADC_PATH, device, "/name");
	if((fd = open(fName, O_RDONLY)) < 0) {
		fprintf(stderr, "Error[%d]: can't open adc device name, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_UNKNOWN;
	}
	read(fd, devName, PATH_BUF_MAX);
	devName[strlen(devName) - 1] = '\0';
	close(fd);

	return IOTBUS_ERROR_NONE;
}

int adc_get_data(int channel, char *devName, int *data)
{
	int fd;
	int device = 0;	/* for get adc device name, /sys/devices/[devName]/iio:device"0" */
	char fName[PATH_BUF_MAX] = {0};
	char voltage[ADC_BUF_MAX] = {0};

	sprintf(fName, "%s%s%s%d%s%d%s", "/sys/devices/", devName, "/iio:device", device, "/in_voltage", channel, "_raw");
	if((fd = open(fName, O_RDONLY)) < 0) {
		fprintf(stderr, "Error[%d]: can't open adc%d channel, %s--[%d]\n", errno, channel, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_UNKNOWN;
	}
	read(fd, voltage, ADC_BUF_MAX);
	*data = atoi(voltage);
	close(fd);

	return IOTBUS_ERROR_NONE;
}
