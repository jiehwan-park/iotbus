#ifndef __IOTBUS_SERVER_H__
#define __IOTBUS_SERVER_H__

#include "capi_iotbus_i2c.h"
#include "capi_iotbus_gpio.h"
#include "capi_iotbus_pwm.h"

int iotbus_server_gpio_process(iotbus_gpio_context_h dev, char *func_name, int write_value, int *read_value);
int iotbus_server_i2c_process(iotbus_i2c_context_h dev, char *func_name, int value, unsigned char *data, int addr);
int iotbus_server_pwm_process(iotbus_pwm_context_h dev, char *function);
#endif /* __IOTBUS_SERVER_H__ */
