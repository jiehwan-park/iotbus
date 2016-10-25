#include <stdio.h>
#include <stdlib.h>
#include <gio/gio.h>
#include <capi_iotbus_pwm.h>
#include <capi_iotbus_error.h>
#include "iotbus_pwm.h"

int iotbus_server_pwm_open(int device, int channel)
{
	return pwm_open(device, channel);
}

int iotbus_server_pwm_close(int device, int channel)
{
	return pwm_close(device, channel);
}

int iotbus_server_pwm_setduty(int device, int channel, int duty_cycle)
{
	return pwm_set_duty_cycle(device, channel, duty_cycle);
}

int iotbus_server_pwm_setperiod(int device, int channel, int period)
{
	return pwm_set_period(device, channel, period);
}

int iotbus_server_pwm_setenable(int device, int channel, int enable)
{
	return pwm_set_enabled(device, channel, enable);
}

int iotbus_server_pwm_getduty(int device, int channel, int * duty_cycle)
{
	return pwm_get_duty_cycle(device, channel, duty_cycle);
}

int iotbus_server_pwm_getperiod(int device, int channel, int *period)
{
	return pwm_get_period(device, channel, period);
}

int iotbus_server_pwm_process(iotbus_pwm_context_h dev, char *function)
{
	int ret = IOTBUS_ERROR_NONE;

	if(dev == NULL)
	{
		printf("dev null error \n");
		return IOTBUS_ERROR_INVALID_PARAMETER;
	}

	if(!g_strcmp0(function, "OPEN"))
	{
		ret = iotbus_server_pwm_open(dev->device, dev->channel);
	}
	else if(!g_strcmp0(function, "CLOSE"))
	{
		ret = iotbus_server_pwm_close(dev->device, dev->channel);
	}
	else if(!g_strcmp0(function, "SET_DUTY"))
	{
		ret = iotbus_server_pwm_setduty(dev->device, dev->channel, dev->duty_cycle);
	}
	else if(!g_strcmp0(function, "SET_PERIOD"))
	{
		ret = iotbus_server_pwm_setperiod(dev->device, dev->channel, dev->period);
	}
	else if(!g_strcmp0(function, "SET_ENABLE"))
	{
		ret = iotbus_server_pwm_setenable(dev->device, dev->channel, dev->enabled);
	}
	else if(!g_strcmp0(function, "GET_DUTY"))
	{
		ret = iotbus_server_pwm_getduty(dev->device, dev->channel, &dev->duty_cycle);
	}
	else if(!g_strcmp0(function, "GET_PERIOD"))
	{
		ret = iotbus_server_pwm_getperiod(dev->device, dev->channel, &dev->period);
	}

	return ret;
}
