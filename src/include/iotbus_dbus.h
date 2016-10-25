#include <gio/gio.h>

#define IOTBUS_DBUS_INTERFACE	"org.tizen.iotBus"
#define IOTBUS_DBUS_PATH	"unix:abstract=/org/tizen/iotbus"
#define IOTBUS_DBUS_SERVICE	"/org/tizen/iotbus"

#define IOTBUS_METHOD_GPIO	"gpio"
#define IOTBUS_METHOD_I2C	"i2c"
#define IOTBUS_METHOD_PWM	"pwm"


const gchar iotbus_data_xml[] =
	"<node>"
	"  <interface name='org.tizen.iotBus'>"
	"    <method name='gpio'>"
	"      <arg type='s' name='function' direction='in'/>"
	"      <arg type='i' name='pin' direction='in'/>"
	"      <arg type='i' name='dir' direction='in'/>"
	"      <arg type='i' name='edge' direction='in'/>"
	"      <arg type='i' name='value' direction='in' />"
	"      <arg type='i' name='re_pin' direction='out'/>"
	"      <arg type='i' name='re_drive' direction='out'/>"
	"      <arg type='i' name='re_edge' direction='out'/>"
	"      <arg type='i' name='re_value' direction='out' />" // read value
	"      <arg type='i' name='re_status' direction='out' />" // return value
	"    </method>"
	"    <method name='i2c'>"
	"      <arg type='s' name='function' direction='in' />"
	"      <arg type='i' name='value' direction='in' />"
	"      <arg type='i' name='fd' direction='in' />"
	"      <arg type='ay' name='data' direction='in' />"
	"      <arg type='i' name='addr' direction='in' />"
	"      <arg type='i' name='re_fd' direction='out' />"
	"      <arg type='ay' name='re_data' direction='out' />"
	"      <arg type='i' name='re_status' direction='out' />" //return value
	"    </method>"
	"    <method name='pwm'>"
	"      <arg type='s' name='function' direction='in' />"
	"      <arg type='i' name='device' direction='in' />"
	"      <arg type='i' name='channel' direction='in' />"
	"      <arg type='i' name='period' direction='in' />"
	"      <arg type='i' name='duty_cycle' direction='in' />"
	"      <arg type='i' name='enabled' direction='in' />"
	"      <arg type='i' name='re_period' direction='out' />"
	"      <arg type='i' name='re_duty' direction='out' />"
	"      <arg type='i' name='re_status' direction='out' />" //return value
	"    </method>"
	"  </interface>"
	"</node>";
