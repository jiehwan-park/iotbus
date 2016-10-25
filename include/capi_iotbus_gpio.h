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
/// @file iotbus_gpio.h
/// @brief Iotbus APIs for Gpio


#ifndef IOTBUS_GPIO_H_
#define IOTBUS_GPIO_H_

/**
 * @brief Enumeration of Gpio output mode
 */
typedef enum {
 IOTBUS_GPIO_DRIVE_NONE= 0,   /* Default */
 IOTBUS_GPIO_DRIVE_PULLUP= 1,   /* Resistive High */
 IOTBUS_GPIO_DRIVE_PULLDOWN = 2, /* Resistive Low */
 IOTBUS_GPIO_DRIVE_FLOAT = 3,
 IOTBUS_GPIO_DRIVE_PUSHPULL = 4,
 IOTBUS_GPIO_DRIVE_MAX
} iotbus_gpio_drive_e;

/**
 * @brief Enumeration of Gpio direction options
 */
typedef enum {
 IOTBUS_GPIO_DIRECTION_NONE = 0,
 IOTBUS_GPIO_DIRECTION_OUT = 1,      /* Output. A Mode can also be set */
 IOTBUS_GPIO_DIRECTION_IN = 2,       /* Input */
 IOTBUS_GPIO_DIRECTION_MAX,
} iotbus_gpio_direction_e;

/**
 * @brief Enumeration of Gpio edge type for interrupt
 */
typedef enum {
 IOTBUS_GPIO_EDGE_NONE = 0,   /* No interrupt on Gpio */
 IOTBUS_GPIO_EDGE_BOTH = 1,   /* Interrupt on rising & falling */
 IOTBUS_GPIO_EDGE_RISING = 2, /* Interrupt on rising only */
 IOTBUS_GPIO_EDGE_FALLING= 3, /* Interrupt on falling only */
 IOTBUS_GPIO_EDGE_MAX,
} iotbus_gpio_edge_e;

/**
 * @brief Struct for iotbus_gpio_s
 */
struct _iotbus_gpio_s {
	int pin;
	iotbus_gpio_drive_e drive;
	iotbus_gpio_direction_e dir;
	iotbus_gpio_edge_e edge;
	//int value_fp; /**< the file pointer to the value of the gpio
	//iotbus_adv_func_s* advance_func; /**< override function table
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Pointer definition to the internal struct iotbus_gpio_s
 */
typedef struct _iotbus_gpio_s* iotbus_gpio_context_h;


typedef void(*gpio_isr_cb)(void *user_data);

/**
 * @brief Initializes gpio_context, based on Gpio pin.
 */
iotbus_gpio_context_h  capi_iotbus_gpio_open (int gpiopin);

/**
 * @brief Closes the gpio_context.
 */
int capi_iotbus_gpio_close (iotbus_gpio_context_h dev);


/**
 * @brief Sets Gpio direction.
 */
int capi_iotbus_gpio_set_direction (iotbus_gpio_context_h dev, iotbus_gpio_direction_e dir);

/**
 * @brief Sets the edge mode on the Gpio.
 */
int capi_iotbus_gpio_set_edge_mode (iotbus_gpio_context_h dev, iotbus_gpio_edge_e edge);

/**
 * @brief Sets Gpio output mode.
 */
int capi_iotbus_gpio_set_drive_mode (iotbus_gpio_context_h dev, iotbus_gpio_drive_e drive);


/**
 * @brief Registers event handler callback for interrupt.
 */
int capi_iotbus_gpio_register_cb (iotbus_gpio_context_h dev, iotbus_gpio_edge_e edge, gpio_isr_cb isr_cb, void *user_data);

/**
 * @brief Unregisters event handler callback for interrupt.
 */
int capi_iotbus_gpio_unregister_cb (iotbus_gpio_context_h dev);


/**
 * @brief Reads the gpio value.
 */
int capi_iotbus_gpio_read (iotbus_gpio_context_h dev);

/**
 * @brief Writes to the gpio value.
 */
int capi_iotbus_gpio_write (iotbus_gpio_context_h dev, int value);


/**
 * @brief Gets a direction of the Gpio.
 */
int capi_iotbus_gpio_get_direction (iotbus_gpio_context_h dev, iotbus_gpio_direction_e *dir);

/**
 * @brief Gets a pin number of the Gpio.
 */
int capi_iotbus_gpio_get_pin (iotbus_gpio_context_h dev);

/**
 * @brief Gets a edge mode of the Gpio.
 */
int capi_iotbus_gpio_get_edge_mode (iotbus_gpio_context_h dev, iotbus_gpio_edge_e *edge);

/**
 * @brief Gets a drive mode of the Gpio.
 */
int capi_iotbus_gpio_get_drive_mode (iotbus_gpio_context_h dev, iotbus_gpio_drive_e *drive);

#ifdef __cplusplus
}
#endif

#endif /* IOTBUS_GPIO_H_ */

