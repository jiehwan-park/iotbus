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
#include <termios.h>
#include <sys/ioctl.h>
#include "iotbus_uart.h"
#include "../include/iotbus_error.h"

#define SYSFS_UART_PATH     "/dev/ttySAC"

#define PATH_BUF_MAX    64
#define UART_BUF_MAX    16

#define UART_BAUDRATE_SIZE 19

int g_iotbus_uart_br_input[UART_BAUDRATE_SIZE] = {
	0,			50,			75,			110,		134,
	150,		200,		300,		600,		1200,
	1800,		2400,		4800,		9600,		19200,
	38400,		57600,		115200,		230400
};
int g_iotbus_uart_br[UART_BAUDRATE_SIZE] = {
	B0,			B50,		B75,		B110,		B134,
	B150,		B200,		B300,		B600,		B1200,
	B1800,		B2400,		B4800,		B9600,		B19200,
	B38400,		B57600,		B115200,	B230400
};

int uart_valid_baudrate(unsigned int baudrate)
{
	int i;
	for(i=0; i<UART_BAUDRATE_SIZE; i++) {
		if(baudrate == g_iotbus_uart_br_input[i]) return g_iotbus_uart_br[i];
	}
	return -1;
}

int uart_open(int port, int *file_hndl)
{
	int fd;
	char fName[PATH_BUF_MAX] = {0};

	sprintf(fName, SYSFS_UART_PATH "%d", port);
	if((fd = open(fName, O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0) {
		fprintf(stderr, "Error[%d]: can't open %s, %s--[%d]\n", errno, fName, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_UNKNOWN;
	}
	*file_hndl = fd;
	return IOTBUS_ERROR_NONE;
}

int uart_close(int file_hndl)
{
	if(!file_hndl) {
		fprintf(stderr, "Error[%d]: Invalid parameter, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}
	close(file_hndl);
	return IOTBUS_ERROR_NONE;
}

int uart_flush(int file_hndl)
{
	int ret;
	if(!file_hndl) {
		fprintf(stderr, "Error[%d]: Invalid parameter, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	ret = tcflush(file_hndl, TCIOFLUSH);
	if(ret < 0) {
		fprintf(stderr, "FAILED[%d]: tcflush, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_NOT_SUPPORTED;
	}

	return IOTBUS_ERROR_NONE;
}

int uart_set_baudrate(int file_hndl, unsigned int baud)
{
	int ret, baudrate;
	struct termios tio;

	memset(&tio, 0, sizeof(tio));
	if(!file_hndl) {
		fprintf(stderr, "Error[%d]: Invalid parameter, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if((baudrate = uart_valid_baudrate(baud)) < 0) {
		fprintf(stderr, "Error[%d]: Invalid parameter, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	ret = tcgetattr(file_hndl, &tio);
	if(ret) {
		fprintf(stderr, "Error[%d]: tcgetattr, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_UNKNOWN;
	}
	tio.c_cflag = baudrate;
	tio.c_iflag = IGNPAR;
	tio.c_oflag = 0;
	tio.c_lflag = 0;
	tio.c_cc[VMIN] = 1;
	tio.c_cc[VTIME] = 0;

	uart_flush(file_hndl);
	ret = tcsetattr(file_hndl, TCSANOW, &tio);
	if(ret) {
		fprintf(stderr, "Error[%d]: tcsetattr, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

int uart_set_mode(int file_hndl, int bytesize, char *parity, int stopbits)
{
	int ret;
	struct termios tio;
	int byteinfo[4] = {CS5, CS6, CS7, CS8};
	iotbus_uart_parity_e parityinfo;

	if(!file_hndl) {
		fprintf(stderr, "Error[%d]: Invalid parameter, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	ret = tcgetattr(file_hndl, &tio);
	if(ret) {
		fprintf(stderr, "Error[%d]: tcgetattr, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_UNKNOWN;
	}

	/* set byte size */
	if(bytesize < 5 || bytesize > 8) {
		fprintf(stderr, "Error[%d]: Invalid parameter bytesize, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}
	tio.c_cflag &= ~CSIZE;
	tio.c_cflag |= byteinfo[bytesize - 5];
	tio.c_cflag |= (CLOCAL | CREAD);

	/* set parity info */
	if (strcmp(parity, "even") == 0)
		parityinfo = IOTBUS_UART_PARITY_EVEN;
	else if(strcmp(parity, "odd") == 0)
		parityinfo = IOTBUS_UART_PARITY_ODD;
	else
		parityinfo = IOTBUS_UART_PARITY_NONE;

	switch(parityinfo) {
		case IOTBUS_UART_PARITY_EVEN:
			tio.c_cflag |= PARENB;
			tio.c_cflag &= ~PARODD;
			break;
		case IOTBUS_UART_PARITY_ODD:
			tio.c_cflag |= PARENB;
			tio.c_cflag |= PARODD;
			break;
		case IOTBUS_UART_PARITY_NONE:
			tio.c_cflag &= ~PARENB;
			tio.c_cflag &= ~PARODD;
			break;
		default:
			fprintf(stderr, "Error[%d]: Invalid parameter parity, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
			return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	/* set stop bit */
	switch(stopbits) {
		case 1:
			tio.c_cflag &= ~CSTOPB;
			break;
		case 2:
			tio.c_cflag |= CSTOPB;
			break;
		default:
			fprintf(stderr, "Error[%d]: Invalid parameter stopbits, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
			return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	uart_flush(file_hndl);
	ret = tcsetattr(file_hndl, TCSANOW, &tio);
	if(ret) {
		fprintf(stderr, "Error[%d]: tcsetattr, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

int uart_set_flowcontrol(int file_hndl, int xonxoff, int rtscts)
{
	int ret;
	struct termios tio;

	if(!file_hndl) {
		fprintf(stderr, "Error[%d]: Invalid parameter, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	ret = tcgetattr(file_hndl, &tio);
	if(ret) {
		fprintf(stderr, "Error[%d]: tcgetattr, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_UNKNOWN;
	}

	/* rtscts => 1: rts/cts on, 0: off */
	if(rtscts == 1) {
		tio.c_cflag |= CRTSCTS;
	}
	else if(rtscts == 0) {
		tio.c_cflag &= ~CRTSCTS;
	}
	else {
		fprintf(stderr, "Error[%d]: Invalid parameter rtscts, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	/* xonxoff => 1: xon/xoff on, 0: off */
	if(xonxoff == 1) {
		tio.c_iflag |= (IXON | IXOFF | IXANY);
	}
	else if(xonxoff == 0) {
		tio.c_iflag &= ~(IXON | IXOFF | IXANY);
	}
	else {
		fprintf(stderr, "Error[%d]: Invalid parameter xonxoff, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	ret = tcsetattr(file_hndl, TCSANOW, &tio);
	if(ret) {
		fprintf(stderr, "Error[%d]: tcsetattr, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return IOTBUS_ERROR_NONE;
}

int uart_read(int file_hndl, char *buf, unsigned int length)
{
	int ret;
	if(!file_hndl) {
		fprintf(stderr, "Error[%d]: Invalid parameter, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	ret = read(file_hndl, (void *)buf, length);
	if((errno != EAGAIN && errno != EINTR) && ret < 0) {
		fprintf(stderr, "Error[%d]: read, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return ret;
}

int uart_write(int file_hndl, char *buf, unsigned int length)
{
	int ret;
	if(!file_hndl) {
		fprintf(stderr, "Error[%d]: Invalid parameter, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	ret = write(file_hndl, buf, length);
	if(ret < 0) {
		fprintf(stderr, "Error[%d]: write, %s--[%d]\n", errno, __FUNCTION__, __LINE__);
		return IOTBUS_ERROR_UNKNOWN;
	}

	return ret;
}
