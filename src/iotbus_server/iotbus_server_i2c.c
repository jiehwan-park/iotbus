#include <stdio.h>
#include <stdlib.h>
#include <gio/gio.h>
#include <capi_iotbus_i2c.h>
#include <capi_iotbus_error.h>
#include "iotbus_i2c.h"

int iotbus_server_i2c_init(iotbus_i2c_context_h dev, int bus)
{
	return i2c_open(bus, &dev->fd);
}

int iotbus_server_i2c_set_freq(iotbus_i2c_context_h dev, int mode)
{
	return i2c_set_frequency(dev->fd, mode);
}

int iotbus_server_i2c_set_addr(iotbus_i2c_context_h dev, int addr)
{
	return i2c_set_address(dev->fd, addr);
}

int iotbus_server_i2c_read(iotbus_i2c_context_h dev, int length, unsigned char * data, int addr)
{
	return i2c_read(dev->fd, data, length, addr);
}

int iotbus_server_i2c_write(iotbus_i2c_context_h dev, int length, unsigned char * data, int addr)
{
	return i2c_write(dev->fd, data, length, addr);
}

int iotbus_server_i2c_stop(iotbus_i2c_context_h dev)
{
	return i2c_close(dev->fd);
}

int iotbus_server_i2c_process(iotbus_i2c_context_h dev, char *func_name, int value, unsigned char *data, int addr)
{
	int ret = IOTBUS_ERROR_NONE;

	if(dev == NULL)
	{
		printf("dev null error \n");
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if(!g_strcmp0(func_name, "INIT"))
	{
		ret = iotbus_server_i2c_init(dev, value);
	}
	else if(!g_strcmp0(func_name, "SET_FREQ"))
	{
		ret = iotbus_server_i2c_set_freq(dev, value);
	}
	else if(!g_strcmp0(func_name, "SET_ADDR"))
	{
		ret = iotbus_server_i2c_set_addr(dev, value);
	}
	else if(!g_strcmp0(func_name, "READ"))
	{
		ret = iotbus_server_i2c_read(dev, value, data, addr);
	}
	else if(!g_strcmp0(func_name, "WRITE"))
	{
		ret = iotbus_server_i2c_write(dev, value, data, addr);
	}
	else if(!g_strcmp0(func_name, "STOP"))
	{
		ret = iotbus_server_i2c_stop(dev);
	}

	return ret;
}
