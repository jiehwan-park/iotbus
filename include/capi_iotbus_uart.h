struct _iotbus_uart_s
{
	int fd;
};


typedef struct _iotbus_uart_s* iotbus_uart_context_h

typedef enum{
	IOTBUS_UART_PARITY_NONE = 0,
	IOTBUS_UART_PARITY_EVEN,
	IOTBUS_UART_PARITY_ODD,
} iotbus_uart_parity_e;

iotbus_uart_context_h iotbus_uart_init (const char *path);

int iotbus_uart_stop (iotbus_uart_context_h hnd);

int iotbus_uart_flush (iotbus_uart_context_h hnd);

int iotbus_uart_set_baudrate (iotbus_uart_context_h hnd, unsigned int baud);

int iotbus_uart_set_mode (iotbus_uart_context_h hnd, int bytesize, iotbus_uart_parity_e parity, int stopbits);

int iotbus_uart_set_flowcontrol (iotbus_uart_context_h hnd, int xonxoff, int rtscts);

int iotbus_uart_read (iotbus_uart_context_h hnd, char *buf, unsigned int length);

int iotbus_uart_write (iotbus_uart_context_h hnd, const char *buf, unsigned int length);
