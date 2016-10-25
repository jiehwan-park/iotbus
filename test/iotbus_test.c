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
/// @file iotbus_test.c
/// @brief Iotbus test sample

#include <capi_iotbus_gpio.h>
#include <capi_iotbus_i2c.h>
#include <capi_iotbus_adc.h>
#include "capi_iotbus_error.h"
#include "capi_iotbus_pwm.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern int iotbus_gpio_test();
extern int iotbus_i2c_test();
extern int iotbus_spi_test();
extern int iotbus_adc_test();
extern int iotbus_pwm_test();
extern int iotbus_uart_test();

int gpio_test(void)
{
	int ret = 0;
	int cnt = 0;
	int num = 135;
	iotbus_gpio_context_h dev = NULL;

	printf("artik5 : 135 \n");
	printf("artik10 : 22 \n");
	printf(">> PIN NUMBER : ");
	scanf("%d", &num);

	printf("num %d\n", num);
	dev = capi_iotbus_gpio_open(num);

	if(dev == NULL)
	{
		printf("iotbus_test dev is null\n");
		return 0;
	}

	ret = capi_iotbus_gpio_set_direction(dev, IOTBUS_GPIO_DIRECTION_OUT);


	if(ret != 0)
	{
		printf("iotbus_test set direction error!!!");
		return 0;
	}

	while(cnt < 5){
		printf("write~\n");
		ret = capi_iotbus_gpio_write(dev, 1);
		sleep(1);
		ret = capi_iotbus_gpio_write(dev, 0);
		sleep(1);
		cnt++;
	}
	printf("write finish\n");
	ret = capi_iotbus_gpio_close(dev);
	printf("iotbus_test_basic!\n");

	return 1;
}

int i2c_test(void)
{
	int num = 0;
	int cnt = 0;
	int addr = 0x23;
	unsigned char buf[10];
	unsigned char reg = 0x10;	/* Start measurement at 1lx resolution. Measurement time is approx 120ms. */
	iotbus_i2c_context_h dev;

	printf(">> I2C bus number : ");
	scanf("%d", &num);

	dev = capi_iotbus_i2c_init(num);

	if(dev == NULL)
	{
		printf("iotbus_test dev is null\n");
		return 0;
	}

	//printf(">> set address(ex:0x23) : ");
	//scanf("%d", &num);

	if(capi_iotbus_i2c_set_address(dev, addr) == 0)
	{
		//capi_iotbus_i2c_set_frequency(dev, IOTBUS_I2C_STD);

		buf[0] = reg;
		if(-1 != capi_iotbus_i2c_write(dev, buf, 1)){
			cnt = 0;
			while(cnt < 15){
				sleep(1);
				capi_iotbus_i2c_read(dev, buf, 2);
				int val = ((buf[0]<<8)|buf[1])/1.2;
				printf("Val[%d]\n", val);
				cnt++;
			}
		}
	}
	capi_iotbus_i2c_stop(dev);

	return 1;
}

int adc_test(void)
{
#if 0
	int channel = 0;
	int data = 0;
	iotbus_adc_context_h dev = NULL;

	printf(">>channel :");
	scanf("%d", &channel);

	dev = capi_iotbus_adc_open(channel);

	if(!dev)
	{
		printf("open error!\n");
		return 1;
	}

	capi_iotbus_adc_read(dev, &data);

	capi_iotbus_adc_close(dev);
#endif
	return 1;
}

int pwm_test_led(void)
{
	int device = 0, channel = 0;
	int period = 1 * 1000;
	int duty_cycle = 1 * 1000 / 100;
	int cnt = 0;

	int set_duty_cycle;
	int get_period, get_duty_cycle;
	iotbus_pwm_context_h dev;


	printf("<<< iotbus_pwm_test >>>\n");

	dev = capi_iotbus_pwm_open(device, channel);
	capi_iotbus_pwm_set_period(dev, period);	/* period: nanosecond */
	capi_iotbus_pwm_set_duty_cycle(dev, duty_cycle);	/* duty_cycle: nanosecond */
	capi_iotbus_pwm_set_enabled(dev, 1);	/* 0: disable, 1: enable */

	while(cnt < 5) {

		for(set_duty_cycle = period; set_duty_cycle > 0; set_duty_cycle -= 50) {
			/* set duty cycle */
			capi_iotbus_pwm_set_duty_cycle(dev, set_duty_cycle);
			capi_iotbus_pwm_get_period(dev, &get_period);
			capi_iotbus_pwm_get_duty_cycle(dev, &get_duty_cycle);
			printf("period(%d), duty_cycle(%d)\n", get_period, get_duty_cycle);
			usleep(500000);
		}
		for(set_duty_cycle = 0; set_duty_cycle < period; set_duty_cycle += 50) {
			/* set duty cycle */
			capi_iotbus_pwm_set_duty_cycle(dev, set_duty_cycle);
			capi_iotbus_pwm_get_period(dev, &get_period);
			capi_iotbus_pwm_get_duty_cycle(dev, &get_duty_cycle);
			printf("period(%d), duty_cycle(%d)\n", get_period, get_duty_cycle);
			usleep(500000);
		}
		cnt++;
	}
	capi_iotbus_pwm_set_enabled(dev, 0);	/* 0: disable, 1: enable */
	capi_iotbus_pwm_close(dev);

	return 0;
}

int pwm_test_motor(void)
{
	int device = 0, channel = 0;
	int period = 20000000;
	int duty_cycle = 1500000;
	int cnt = 0, idx = 0;
	int set_duty_cycle;
	int get_period, get_duty_cycle;
	int degree[3] = {0, 45, 90};
	iotbus_pwm_context_h dev;

	printf("<<< iotbus_pwm_test_motor >>>\n");

	dev = capi_iotbus_pwm_open(device, channel);
	for(cnt=0; cnt <5; cnt++){
		for(idx = 0; idx <3; idx++){
			switch(degree[idx]){
				case 0 :
					duty_cycle = 1000000;
					break;
				case 45 :
					duty_cycle = 1500000;			
					break;
				case 90 :
					duty_cycle = 2000000;
					break;
				default :
					duty_cycle = 2000000;
					break;
			}		
			printf("set degree: %d\n", degree[idx]);
			capi_iotbus_pwm_set_period(dev, period);
			capi_iotbus_pwm_set_duty_cycle(dev, duty_cycle);
			capi_iotbus_pwm_set_enabled(dev, 1);		/* 0: disable, 1: enable */
			usleep(500000);
		}
	}

	capi_iotbus_pwm_set_enabled(dev, 0);	/* 0: disable, 1: enable */
	capi_iotbus_pwm_close(dev);

	return 0;
}

int main(int argc, char **argv)
{
	int num = 1;
	int ret = 0;

	printf("===================\n");
	printf("  iotbus_test Menu\n");
	printf("===================\n");
	printf(" 1. GPIO Test\n");
	printf(" 2. I2C Test\n");
	printf(" 3. pwm led test\n");
	printf(" 4. pwm motor test\n");

	printf(" 11. H/W IF GPIO Test\n");
	printf(" 12. H/W IF I2C Test\n");
	printf(" 13. H/W IF PWM Test\n");
	printf(" 14. H/W IF SPI Test\n");
	
	scanf("%d", &num);
	
	switch(num)
	{
	case 1:
		ret = gpio_test();
		break;
	case 2:
		ret = i2c_test();
		break;
	case 3:
		ret = pwm_test_led();		
		break;
	case 4:
		ret = pwm_test_motor();
		break;
	case 11:
		ret = iotbus_gpio_test();
		break;
	case 12:
		ret = iotbus_i2c_test();
		break;
	case 13:
		ret = iotbus_spi_test();
		break;
	case 14:
		ret = iotbus_adc_test();
		break;
	case 15:
		ret = iotbus_pwm_test();
		break;
	case 16:
		ret = iotbus_uart_test();
		break;
	default:
		printf("Not support \n");
	}
	
	return 1;
}
