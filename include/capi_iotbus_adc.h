
#ifndef IOTBUS_ADC_H_
#define IOTBUS_ADC_H_

/**
 * @brief Struct for iotbus_gpio_s
 */

#define DEVICE_NAME_SIZE	20

struct _iotbus_adc_s {
	char device_name[DEVICE_NAME_SIZE];
	int channel;
};

/**
 * @brief Pointer definition to the internal struct iotbus_adc_s
 */
typedef struct _iotbus_adc_s* iotbus_adc_context_h;

iotbus_adc_context_h capi_iotbus_adc_open (int channel);

int capi_iotbus_adc_read(iotbus_adc_context_h dev, int *data);

int capi_iotbus_adc_close (iotbus_adc_context_h dev);

#endif /* IOTBUS_ADC_H */
