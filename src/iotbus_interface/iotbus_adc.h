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

#ifndef __IOT_BUS_ADC_LIBRARY_H__
#define __IOT_BUS_ADC_LIBRARY_H__

/**
* @brief adc_init() find adc device name.
*
* @param[in] *devName adc device name
* @return On success, 0 is returned. On failure, a negative value is returned.
*/
int adc_get_device_name(char *devName);

/**
* @brief adc_get_data() get adc data.
*
* @param[in] channel adc channel number
* @param[in] *devName adc device name
* @param[in] *data adc value
* @return On success, voltage is returned. On failure, a negative value is returned.
*/
int adc_get_data(int channel, char *devName, int *data);

#endif /* __IOT_BUS_ADC_LIBRARY_H__ */
