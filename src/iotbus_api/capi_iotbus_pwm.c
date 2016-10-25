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
/// @file iotbus_gpio.c
/// @brief Iotbus APIs for Gpio

#include "capi_iotbus_pwm.h"
#include "capi_iotbus_error.h"
#include "capi_iotbus.h"
#include "capi_iotbus_dbus.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "iotbus_pwm.h"

#define PWM_NAME	"pwm"
#define PWM_ENABLE	1
#define PWM_DISABLE	0

#ifdef __cplusplus
extern "C" {
#endif

API iotbus_pwm_context_h capi_iotbus_pwm_open (int device, int channel)
{
	iotbus_pwm_context_h dev = NULL;
	int ret = IOTBUS_ERROR_NONE;

	/* Initialize */
	dev = (iotbus_pwm_context_h)malloc(sizeof(struct _iotbus_pwm_s));

	if (dev == NULL)
	{
		return NULL;
	}

	dev->device = device;
	dev->channel = channel;

	ret = capi_iotbus_dbus_pwm(dev, PWM_NAME, "OPEN");

	if(ret != IOTBUS_ERROR_NONE)
	{
		free(dev);
		dev = NULL;
	}

	return dev;
}

API int capi_iotbus_pwm_close (iotbus_pwm_context_h pwm)
{
	int ret = IOTBUS_ERROR_NONE;

	ret = capi_iotbus_dbus_pwm(pwm, PWM_NAME, "CLOSE");

	if(ret == IOTBUS_ERROR_NONE)
	{
		free(pwm);
		pwm = NULL;
	}

	return ret;
}


API int	capi_iotbus_pwm_set_duty_cycle (iotbus_pwm_context_h pwm, int duty_cycle)
{
	int ret = IOTBUS_ERROR_NONE;

	if(pwm->duty_cycle != duty_cycle)
	{
		int duty_value = 0;

		duty_value = pwm->duty_cycle;
		pwm->duty_cycle = duty_cycle;
		ret = capi_iotbus_dbus_pwm(pwm, PWM_NAME, "SET_DUTY");

		if(ret != IOTBUS_ERROR_NONE)
			pwm->duty_cycle = duty_value;

	}

	return ret;
}

API int capi_iotbus_pwm_set_period (iotbus_pwm_context_h pwm, int period)
{
	int ret = IOTBUS_ERROR_NONE;

	if(pwm->period != period)
	{
		int period_value = 0;

		period_value = pwm->period;
		pwm->period = period;
		ret = capi_iotbus_dbus_pwm(pwm, PWM_NAME, "SET_PERIOD");

		if(ret != IOTBUS_ERROR_NONE)
			pwm->period = period_value;
	}

	return ret;
}

API int	capi_iotbus_pwm_set_enabled (iotbus_pwm_context_h pwm, iotbus_pwm_state_e enable)
{
	int ret = IOTBUS_ERROR_NONE;

	if(pwm->enabled != enable)
	{
		int enable_value = 0;

		enable_value = pwm->enabled;
		pwm->enabled = enable;
		ret = capi_iotbus_dbus_pwm(pwm, PWM_NAME, "SET_ENABLE");

		if(ret != IOTBUS_ERROR_NONE)
			pwm->enabled = enable_value;
	}

	return IOTBUS_ERROR_NONE;
}

API int capi_iotbus_pwm_is_enabled (iotbus_pwm_context_h pwm)
{
	if(pwm->enabled == PWM_ENABLE)
		return PWM_ENABLE;
	else
		return PWM_DISABLE;

}

API int capi_iotbus_pwm_get_duty_cycle (iotbus_pwm_context_h pwm, int *duty_cycle)
{
	int duty_value = 0;
	int ret = IOTBUS_ERROR_NONE;

	duty_value = pwm->duty_cycle;

	ret = capi_iotbus_dbus_pwm(pwm, PWM_NAME, "GET_DUTY");

	(*duty_cycle) = pwm->duty_cycle;
	pwm->duty_cycle = duty_value;

	return ret;
}

API int capi_iotbus_pwm_get_period (iotbus_pwm_context_h pwm, int *period)
{
	int ret = IOTBUS_ERROR_NONE;
	int period_value = 0;

	period_value = pwm->period;

	ret = capi_iotbus_dbus_pwm(pwm, PWM_NAME, "GET_PERIOD");

	(*period) = pwm->period;
	pwm->period = period_value;

	return ret;
}

#ifdef __cplusplus
}
#endif
