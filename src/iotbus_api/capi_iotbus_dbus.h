#ifndef __CAPI_IOTBUS_DBUS_H__
#define __CAPI_IOTBUS_DBUS_H__

#include "capi_iotbus_i2c.h"
#include "capi_iotbus_gpio.h"
#include "capi_iotbus_pwm.h"

int capi_iotbus_dbus_gpio(iotbus_gpio_context_h dev, char * sensorid, char *funcname, int write_value, int *read_value);
int capi_iotbus_dbus_i2c(iotbus_i2c_context_h dev, char * sensorid, char *funcname, int value, unsigned char *data, int addr);
int capi_iotbus_dbus_pwm(iotbus_pwm_context_h dev, char * sensorid, char *funcname);
#endif /* __CAPI_IOTBUS_DBUS_H__ */
