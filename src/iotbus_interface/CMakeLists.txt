CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (iotbus_test)
FIND_PACKAGE ( Threads REQUIRED )
SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_C_COMPILER   /usr/bin/arm-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabi-g++)

# set dir for header
INCLUDE_DIRECTORIES (../include)

# sources
SET (LIBSRCS iotbus_adc.c iotbus_gpio.c iotbus_i2c.c iotbus_spi.c iotbus_pwm.c iotbus_uart.c iotbus_interface_test.c)

# build libcalib.a with LIBSRCS sources
ADD_LIBRARY(iot_bus SHARED ${LIBSRCS})

ADD_EXECUTABLE (iotbus_test iotbus_interface_test.c)

TARGET_LINK_LIBRARIES(iotbus_test iot_bus)
TARGET_LINK_LIBRARIES(iotbus_test pthread)
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
