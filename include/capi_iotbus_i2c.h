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
#include <stdint.h>

#ifndef IOTBUS_I2C_H_
#define IOTBUS_I2C_H_

/**
 * @brief Struct for iotbus_gpio_s
 */
struct _iotbus_i2c_s {
	int fd;
};
typedef struct _iotbus_i2c_s *iotbus_i2c_context_h;

typedef enum {
	IOTBUS_I2C_STD = 0,
	IOTBUS_I2C_FAST = 1,
	IOTBUS_I2C_HIGH = 2
} iotbus_i2c_mode_e;

#ifdef __cplusplus
extern "C" {
#endif

iotbus_i2c_context_h capi_iotbus_i2c_init(int bus);

int capi_iotbus_i2c_stop(iotbus_i2c_context_h hnd);

int capi_iotbus_i2c_set_frequency(iotbus_i2c_context_h hnd, iotbus_i2c_mode_e mode);

int capi_iotbus_i2c_set_address(iotbus_i2c_context_h hnd, int address);

int capi_iotbus_i2c_read(iotbus_i2c_context_h hnd, uint8_t *data, int length);

int capi_iotbus_i2c_write(iotbus_i2c_context_h hnd, const uint8_t *data, int length);

#ifdef __cplusplus                                                                  
}                                                                                   
#endif                                                                              
                                                                                    
#endif /* IOTBUS_I2C_H_ */                                                          
