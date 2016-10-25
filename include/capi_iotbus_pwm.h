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
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#ifndef IOTBUS_PWM_H
#define IOTBUS_PWM_H

struct _iotbus_pwm_s
{
	int device;
	int channel;
	int period;
	int duty_cycle;
	int enabled;
};
typedef struct _iotbus_pwm_s *iotbus_pwm_context_h;

typedef enum {
	IOTBUS_PWM_DISABLE = 0,
   	IOTBUS_PWM_ENABLE,
} iotbus_pwm_state_e;

#ifdef __cplusplus
extern "C" {
#endif

iotbus_pwm_context_h capi_iotbus_pwm_open (int device, int channel);

int capi_iotbus_pwm_close (iotbus_pwm_context_h pwm);

int	capi_iotbus_pwm_set_duty_cycle (iotbus_pwm_context_h pwm, int duty_cycle);

int capi_iotbus_pwm_set_period (iotbus_pwm_context_h pwm, int period);

int	capi_iotbus_pwm_set_enabled (iotbus_pwm_context_h pwm, iotbus_pwm_state_e enable);

int capi_iotbus_pwm_is_enabled (iotbus_pwm_context_h pwm);

int capi_iotbus_pwm_get_duty_cycle (iotbus_pwm_context_h pwm, int *duty_cycle);

int capi_iotbus_pwm_get_period (iotbus_pwm_context_h pwm, int *period);

#ifdef __cplusplus
}
#endif

#endif /* IOTBUS_PWM_H */
