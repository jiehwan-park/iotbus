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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../src/iotbus_interface/iotbus_adc.h"
#include "../src/iotbus_interface/iotbus_pwm.h"
#include "../src/iotbus_interface/iotbus_uart.h"
#include "../src/iotbus_interface/iotbus_gpio.h"
#include "../src/iotbus_interface/iotbus_i2c.h"
#include "../src/iotbus_interface/iotbus_spi.h"
#include "../src/include/iotbus_error.h"
#include <poll.h>
#include <pthread.h>
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */

void test_gpio_interrupt(void)
{
	int ret = 0;
	char buf[255] = {0,};
	int isr_fd;
	struct pollfd fdset;
	int len;
	int poll_state;
	int status;
	int i = 0;

	memset(&fdset, 0, sizeof(fdset));

	printf("test_gpio_intterupt start\n");

	isr_fd = gpio_open_isr(134);
	
	printf("isr_fd[%d]\n", isr_fd);
	fdset.fd = isr_fd;
	//fdset.events = POLLPRI; => it will put the value in gpio_read_isr
	fdset.revents = 0;

	while(1){
		status = gpio_read_isr(&fdset, buf, 1);
		if(status > 0){
			printf("app[%s]\n", buf);
		}
	}

	gpio_close(134);
	gpio_close(135);
}

void test_create_gpio_task(void)
{
	int isr_pin = 134;
	int gpio_pin = 135;
	int isr_fd;

	printf("Start Test gpio ISR\n");
	pthread_t inc_x_thread;

	if (IOTBUS_ERROR_NONE == gpio_open(gpio_pin) && IOTBUS_ERROR_NONE == gpio_open(isr_pin)) {
		printf("[%d] and [%d] GPIO Open Success\n", gpio_pin, isr_pin);
		if (0 == gpio_set_direction(gpio_pin, GPIO_DIRECTION_OUT) && 0 == gpio_set_direction(isr_pin, GPIO_DIRECTION_IN)) {
			printf("GPIO Direction Success\n");
			if(0 == gpio_set_edge_mode(isr_pin, GPIO_EDGE_FALLING)) {

			/*	if (pthread_create(&inc_x_thread, NULL, &test_gpio_interrupt, NULL)){
					printf("Error Creating Thread\n");

				}
				else {
					printf("Thread created successfully\n");
				}
				//test_gpio_interrupt();
				while(1){
					sleep(1);
				}
			*/
			}
			else
			{
				printf("Error GPIO set edge mode\n");
			}
		}
		else 
		{
			printf("Error GPIO set direction\n");
		}
	}
	else
	{
		printf("Error GPIO Pin open\n");
	}
}

void test_gpio(void)
{
	int cnt = 0;
	int ret = 0;
	int pin = 0;
	gpio_direction_e dir;
	gpio_edge_e edge;
	printf("Start Test GPIO\n");

	pin = 135;
	// create gpioX file
	if(IOTBUS_ERROR_NONE == gpio_open(pin)){

		if(0 == gpio_get_direction(pin, &dir)){
			printf("direction[%d]\n", dir);
		}

		if(0 == gpio_get_edge_mode(pin, &edge)){
			printf("edge[%d]\n", edge);
		}
		// set direction
		/*
		gpio_set_direction (pin, GPIO_DIRECTION_OUT);

		// iterate 1, 0
		cnt = 0;
		while(cnt < 5){
			ret = gpio_write (pin, 1);
			sleep(1);
			ret = gpio_write (pin, 0);
			sleep(1);
			cnt++;
		}
		*/
		/// remove gpioX file
		gpio_close(pin);
	}
	else {
		printf(" Unable to open gpio[%d]\n", pin);
	}
}


void test_i2c_gy30_lignt(void)
{
	int ret = 0;
	int addr;	/* BH1750 i2c address */
	unsigned char reg = 0x10;	/* Start measurement at 1lx resolution. Measurement time is approx 120ms. */
	unsigned char buf[10];
	int cnt = 0;
	int bus = 0;
	int fd;
	int speed;
	printf("***** Start Test gy30 I2C *****\n");

	bus = 9;
	// init i2c
	if(IOTBUS_ERROR_NONE == i2c_open(bus, &fd)){
		addr = 0x23;
		ret = i2c_set_address(fd, addr);
		printf("ret %d\n", ret);
		//speed = I2C_STD;
		//ret = i2c_set_frequency(fd, speed);
		//printf("speed ret %d\n", ret);

		buf[0] = reg;
		if(-1 != i2c_write(fd, buf, 1, addr)){
			cnt = 0;
			while(cnt < 10){
				sleep(1);
				i2c_read(fd, buf, 2, addr);
				int val = ((buf[0]<<8)|buf[1])/1.2;
				printf("Val[%d]\n", val);
				cnt++;
			}
		}
		// deinit i2c
		i2c_close(fd);
	}
}

void test_i2c_grove_color(void)
{
	int ret = 0;
	int addr;
	unsigned char buf[10];
	char data[8] = {0,};
	int green;
	int red;
	int blue;
	int i;
	int total;
	int fd;
	int bus;
	unsigned char val;

	double X,Y,Z,x,y;


	printf("***** Start Test I2C Color *****\n");
/*
	bus = 7;
	if(IOTBUS_ERROR_NONE == i2c_open(bus, &fd)){
		addr = 0x39;
		i2c_set_address(fd, addr);
		sleep(1);
		total = 0;
		buf[0] = 0x80;
		buf[1] = 0x03;
		i2c_write(fd, buf, 2);

		for(i = 0; i < 5; ++i){


			while(total < 10){
				buf[0] = 0xD0 + total;
				i2c_write(fd, buf, 1);
				i2c_read(fd, &val, 1);
				data[total] = val;
				total++;
			}

			green = data[1] * 256 + data[0];
			red = data[3] * 256 + data[2];
			blue = data[5] * 256 + data[4];

			X=(-0.14282)*red+(1.54924)*green+(-0.95641)*blue;
			Y=(-0.32466)*red+(1.57837)*green+(-0.73191)*blue;
			Z=(-0.68202)*red+(0.77073)*green+(0.56332)*blue;
			x=X/(X+Y+Z);
			y=Y/(X+Y+Z);

			if((X>0)&&(Y>0)&&(Z>0))
			{
				printf("x=[%lf], y=[%lf]\n", x, y);
				if (x<0.25) {
					//blue
					printf("blue\n");
				}
				if (x>0.25 && x<0.36) {
				//Green
					printf("green\n");
				}

				if (x>=0.36 ) {
				//Red
					printf("Red\n");
				}
			}
			else{
				printf("error:overflow\n");
			}


			sleep(1);
			buf[0] = 0xE0;
			i2c_write(fd, buf, 1);
		}

		buf[0] = 0x80;
		buf[1] = 0x00;
		i2c_write(fd, buf, 2);

		ret = i2c_close(fd);
		printf("ret = %d, %d\n", ret, __LINE__);
	}*/
}

void test_spi_mma7455_accelrometer(void)
{
	int ret = 0;
	int bus;
	int fd;

	printf("***** Start Test spi accelrometer *****\n");

	bus = 1;
	ret = spi_open(bus, &fd);

	if (ret == IOTBUS_ERROR_NONE){
		sleep(1);

	}
}

int iotbus_gpio_test()
{
	test_gpio();

	return 0;
}

int iotbus_i2c_test()
{
	printf("iotbus_i2c_test\n");
	test_i2c_gy30_lignt();
	//test_i2c_grove_color();

	return 0;
}

int iotbus_spi_test()
{
	test_spi_mma7455_accelrometer();
	return 0;
}

int iotbus_adc_test()
{
	int channel = 0;
	char devName[16] = {0};
	int voltage;

	printf("<<< iotbus_adc_test >>>\n");
	adc_get_device_name(devName);

	while(1) {
		adc_get_data(channel, devName, &voltage);
		printf("adc_get_data(%d)\n", voltage);
		sleep(1);
	}
	return 0;
}

int iotbus_pwm_test()
{
	int device = 0, channel = 0;
	int period = 1 * 1000;
	int duty_cycle = 1 * 1000 / 100;

	int set_duty_cycle;
	int get_period, get_duty_cycle;

	printf("<<< iotbus_pwm_test >>>\n");

	pwm_open(device, channel);
	pwm_set_period(device, channel, period);	/* period: nanosecond */
	pwm_set_duty_cycle(device, channel, duty_cycle);	/* duty_cycle: nanosecond */
	pwm_set_enabled(device, channel, 1);	/* 0: disable, 1: enable */

	while(1) {
		for(set_duty_cycle = period; set_duty_cycle > 0; set_duty_cycle -= 50) {
			/* set duty cycle */
			pwm_set_duty_cycle(device, channel, set_duty_cycle);
			pwm_get_period(device, channel, &get_period);
			pwm_get_duty_cycle(device, channel, &get_duty_cycle);
			printf("period(%d), duty_cycle(%d)\n", get_period, get_duty_cycle);
			usleep(500000);
		}
		for(set_duty_cycle = 0; set_duty_cycle < period; set_duty_cycle += 50) {
			/* set duty cycle */
			pwm_set_duty_cycle(device, channel, set_duty_cycle);
			pwm_get_period(device, channel, &get_period);
			pwm_get_duty_cycle(device, channel, &get_duty_cycle);
			printf("period(%d), duty_cycle(%d)\n", get_period, get_duty_cycle);
			usleep(500000);
		}
	}
	pwm_close(device, channel);

	return 0;
}

 int iotbus_uart_test()
{
	int fd;
	int ret;
	char c = 'D';

	int port = 3;
	unsigned int baud = 115200;
	int bytesize = 8;
	char *parity = "none";
	int stopbits = 1;
	int xonxoff = 1;
	int rtscts = 0;

	printf("<<< iotbus_uart_test >>>\n");
	printf("port path: /dev/ttySAC%d (being used at artik)\n", port);
	printf("set baudrate: %d\n", baud);
	printf("set mode: bytesize(%d), parity(%s), stopbits(%d)\n", bytesize, parity, stopbits);
	printf("set flow control: xonxoff(%d), rtscts(%d)\n", xonxoff, rtscts);

	uart_open(port, &fd);
	uart_set_baudrate(fd, baud);
	uart_set_mode(fd, bytesize, parity, stopbits);
	uart_set_flowcontrol(fd, xonxoff, rtscts);

	while(c != 'q') {
		ret = read(STDIN_FILENO, &c, 1);
		if(ret > 0) {
			uart_write(fd, &c, 1);
		}
	}

	uart_close(fd);
	return 0;
}

/*
int main(int argc, char **argv)
{
	int ret, test_case;
	if(argc == 1) {
		printf("[1] gpio_test\n");
		printf("[2] i2c_test\n");
		printf("[3] spi_test\n");
		printf("[4] adc_test\n");
		printf("[5] pwm_test\n");
		printf("[6] uart_test\n");
		return 0;
	}

	test_case = atoi(argv[1]);
	switch(test_case) {
		case 1:
			ret = iotbus_gpio_test();
			break;
		case 2:
			ret = iotbus_i2c_test();
			break;
		case 3:
			ret = iotbus_spi_test();
			break;
		case 4:
			ret = iotbus_adc_test();
			break;
		case 5:
			ret = iotbus_pwm_test();
			break;
		case 6:
			ret = iotbus_uart_test();
			break;
		default:
			break;
	}

	return 0;
}
*/

