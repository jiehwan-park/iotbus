#include <stdio.h>
#include <stdlib.h>
#include <gio/gio.h>
#include <capi_iotbus_gpio.h>
#include <capi_iotbus_error.h>
#include "iotbus_gpio.h"


int iotbus_server_gpio_open(iotbus_gpio_context_h dev)
{
	int edge, dir;

	if (gpio_open(dev->pin) < 0)
	{
		printf("gpio_pin error \n");
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if(gpio_get_edge_mode(dev->pin, &edge ) < 0)
	{
		gpio_close(dev->pin);
		printf("gpio edge error \n");
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}
	dev->edge = edge;

	if(gpio_get_direction(dev->pin, &dir) < 0)
	{
		gpio_close(dev->pin);
		printf("gpio direction error \n");
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}
	dev->dir = dir;

	return IOTBUS_ERROR_NONE;
}

int iotbus_server_gpio_set_direction(iotbus_gpio_context_h dev)
{
	return gpio_set_direction(dev->pin, dev->dir);
}

int iotbus_server_gpio_get_direction(iotbus_gpio_context_h dev)
{
	return gpio_get_direction(dev->pin, &dev->dir);
}

int iotbus_server_gpio_set_edge(iotbus_gpio_context_h dev)
{
	return gpio_set_edge_mode(dev->pin, dev->edge);
}

int iotbus_server_gpio_get_edge(iotbus_gpio_context_h dev)
{
	return gpio_get_edge_mode(dev->pin, &dev->edge);
}

int iotbus_server_gpio_write(iotbus_gpio_context_h dev, int value)
{
	return gpio_write(dev->pin, value);
}

int iotbus_server_gpio_read(iotbus_gpio_context_h dev, int *read_value)
{
	return gpio_read(dev->pin, read_value);
}

int iotbus_server_gpio_close(iotbus_gpio_context_h dev)
{
	return gpio_close(dev->pin);
}

int iotbus_server_gpio_process(iotbus_gpio_context_h dev, char *func_name, int write_value, int * read_value)
{
	int ret = IOTBUS_ERROR_NONE;

	if(dev == NULL)
	{
		printf("dev null error \n");
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if(!g_strcmp0(func_name, "OPEN"))
	{
		ret = iotbus_server_gpio_open(dev);
	}
	else if(!g_strcmp0(func_name, "SET_DIR"))
	{
		ret = iotbus_server_gpio_set_direction(dev);
	}
	else if(!g_strcmp0(func_name, "GET_DIR"))
	{
		ret = iotbus_server_gpio_get_direction(dev);
	}
	else if(!g_strcmp0(func_name, "SET_EDGE"))
	{
		ret = iotbus_server_gpio_set_edge(dev);
	}
	else if(!g_strcmp0(func_name, "GET_EDGE"))
	{
		ret = iotbus_server_gpio_get_edge(dev);
	}
	else if(!g_strcmp0(func_name, "WRITE"))
	{
		ret = iotbus_server_gpio_write(dev, write_value);
	}
	else if(!g_strcmp0(func_name, "READ"))
	{
		ret = iotbus_server_gpio_read(dev, read_value);
	}
	else if(!g_strcmp0(func_name, "CLOSE"))
	{
		ret = iotbus_server_gpio_close(dev);
	}

	return ret;
}
