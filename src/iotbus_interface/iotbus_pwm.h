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

#ifndef IOTBUS_PWM_H_
#define IOTBUS_PWM_H_

/**
* @brief pwm_open() init pwm channel.
*
* @param[in] device pwm chip number
* @param[in] channel pwm channel number
* @return On success, 0 is returned. On failure, a negative value is returned.
*/
int pwm_open(int device, int channel);

/**
* @brief pwm_close() deinit pwm channel.
*
* @param[in] device pwm chip number
* @param[in] channel pwm channel number
* @return On success, 0 is returned. On failure, a negative value is returned.
*/
int pwm_close(int device, int channel);

/**
* @brief pwm_set_period() sets the pwm period.
*
* @param[in] device pwm chip number
* @param[in] channel pwm channel number
* @param[in] period pwm period
* @return On success, 0 is returned. On failure, a negative value is returned.
*/
int pwm_set_period(int device, int channel, int period);

/**
* @brief pwm_set_duty_cycle() sets the pwm duty cycle.
*
* @param[in] device pwm chip number
* @param[in] channel pwm channel number
* @param[in] duty_cycle pwm duty cycle
* @return On success, 0 is returned. On failure, a negative value is returned.
*/
int pwm_set_duty_cycle(int device, int channel, int duty_cycle);

/**
* @brief pwm_set_enabled() sets the pwm state.
*
* @param[in] device pwm chip number
* @param[in] channel pwm channel number
* @param[in] enable pwm enable/disabled state value
* @return On success, 0 is returned. On failure, a negative value is returned.
*/
int pwm_set_enabled(int device, int channel, int enable);

/**
* @brief pwm_is_enabled() checks if pwm state is enabled.
*
* @param[in] device pwm chip number
* @param[in] channel pwm channel number
* @param[in] enable pwm enable/disabled state value
* @return On success, current pwm state value is returned. On failure, a negative value is returned.
*/
int pwm_get_enabled(int device, int channel, int *enable);

/**
* @brief pwm_get_period() gets the pwm period.
*
* @param[in] device pwm chip number
* @param[in] channel pwm channel number
* @param[in] period pwm period
* @return On success, current pwm period is returned. On failure, a negative value is returned.
*/
int pwm_get_period(int device, int channel, int *period);

/**
* @brief pwm_get_duty_cycle() gets the pwm duty cycle.
*
* @param[in] device pwm chip number
* @param[in] channel pwm channel number
* @param[in] duty_cycle pwm duty cycle
* @return On success, current pwm duty cycle is returned. On failure, a negative value is returned.
*/
int pwm_get_duty_cycle(int device, int channel, int *duty_cycle);

#endif /* IOTBUS_PWM_H_ */
