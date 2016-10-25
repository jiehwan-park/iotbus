#include <stdio.h>
#include <stdlib.h>
#include <gio/gio.h>
#include "capi_iotbus_gpio.h"
#include "capi_iotbus_i2c.h"
#include "capi_iotbus_pwm.h"
#include "capi_iotbus_error.h"
#include "iotbus_dbus.h"

//static GDBusNodeInfo *introspection_data = NULL;
GDBusConnection *gpio_connection = NULL;
GDBusConnection *i2c_connection = NULL;
GDBusConnection *pwm_connection = NULL;

int capi_iotbus_dbus_gpio(iotbus_gpio_context_h dev, char * sensorid, char *funcname, int write_value, int *read_value)
{
	GError *error = NULL;

	GVariant *ret_value = NULL;
	gchar *function = NULL;
	gint32 read = 0;
	gint32 ret = IOTBUS_ERROR_NONE;

	error = NULL;

	//g_type_init();

	if(gpio_connection == NULL)
	{
		gpio_connection = g_dbus_connection_new_for_address_sync(IOTBUS_DBUS_PATH,
															G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT,
															NULL,
															NULL,
															&error);
			if(gpio_connection == NULL)
			{
				g_printerr("Error connection to D-bus address %s: %s\n", IOTBUS_DBUS_PATH, error->message);
				g_error_free(error);
				return IOTBUS_ERROR_UNKNOWN;
			}
	}

	function = g_strdup(funcname);

	ret_value = g_dbus_connection_call_sync(gpio_connection,
										NULL,
										IOTBUS_DBUS_SERVICE,
										IOTBUS_DBUS_INTERFACE,
										sensorid,
										g_variant_new("(siiii)", function, dev->pin, dev->dir, dev->edge, write_value),
										G_VARIANT_TYPE ("(iiiii)"),
										G_DBUS_CALL_FLAGS_NONE,
										-1,
										NULL,
										&error);

	if(ret_value == NULL)
	{
		g_printerr("Error invoking %s () : %s\n", sensorid, error->message);
		g_error_free(error);
		return IOTBUS_ERROR_UNKNOWN;
	}

	g_variant_get(ret_value, "(iiiii)", &dev->pin, &dev->dir, &dev->edge, &read, &ret);
	//g_print("Server said <pin : %d, dir: %d, edge: %d, read value: %d, return : %d>\n", dev->pin, dev->dir, dev->edge, read, ret);

	g_variant_unref(ret_value);

	if(read_value != 0)
		(*read_value) = read;

	return ret;

}

int capi_iotbus_dbus_i2c(iotbus_i2c_context_h dev, char * sensorid, char *funcname, int value, unsigned char * data, int addr)
{
	GError *error = NULL;
	//GDBusConnection *connection = NULL;
	GVariant *ret_value = NULL;
	gchar *function = NULL;
	gint32 ret = IOTBUS_ERROR_NONE;
	gint32 data_value = 0;
	GVariantBuilder *builder;
	gchar *str;
	GVariant *ret_data;
	//gchar data_val;

	error = NULL;

	g_type_init();

	if(i2c_connection == NULL)
	{
		i2c_connection = g_dbus_connection_new_for_address_sync(IOTBUS_DBUS_PATH,
															G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT,
															NULL,
															NULL,
															&error);
		if(i2c_connection == NULL)
		{
			g_printerr("Error connection to D-bus address %s: %s\n", IOTBUS_DBUS_PATH, error->message);
			g_error_free(error);
			return IOTBUS_ERROR_UNKNOWN;
		}

	}


	function = g_strdup(funcname);

	builder = g_variant_builder_new(G_VARIANT_TYPE ("ay"));
	if(data == NULL)
	{
		g_variant_builder_add (builder, "y", 0x10);
		g_variant_builder_add(builder, "y", 0x00);
	}
	else
	{
		int i = 0;
		for(i = 0; i<value; i++)
			g_variant_builder_add(builder, "y", data[i]);
		g_variant_builder_add(builder, "y", 0x00);
	}

	ret_value = g_dbus_connection_call_sync(i2c_connection,
										NULL,
										IOTBUS_DBUS_SERVICE,
										IOTBUS_DBUS_INTERFACE,
										sensorid,
										g_variant_new("(siiayi)", function, value, dev->fd, builder, addr),
										G_VARIANT_TYPE ("(iayi)"),
										G_DBUS_CALL_FLAGS_NONE,
										-1,
										NULL,
										&error);
	g_variant_builder_unref(builder);

	if(ret_value == NULL)
	{
		g_printerr("Error invoking %s () : %s\n", sensorid, error->message);
		g_error_free(error);
		return IOTBUS_ERROR_UNKNOWN;
	}

	g_variant_get(ret_value, "(iayi)", &(dev->fd), &ret_data, &ret);
	g_variant_unref(ret_value);

	if(data !=NULL)
	{
		int i = 0;
		while(g_variant_iter_loop(ret_data, "y", &str))
		{
			data[i] = str;
			i++;
			if(i == value)
				break;
		}
	}

	return ret;
}

int capi_iotbus_dbus_pwm(iotbus_pwm_context_h dev, char * sensorid, char *funcname)
{
	GError *error = NULL;
	//GDBusConnection *connection = NULL;
	GVariant *ret_value = NULL;
	gchar *function = NULL;
	gint32 ret = IOTBUS_ERROR_NONE;

	error = NULL;

	g_type_init();

	if(pwm_connection == NULL)
	{
		pwm_connection = g_dbus_connection_new_for_address_sync(IOTBUS_DBUS_PATH,
															G_DBUS_CONNECTION_FLAGS_AUTHENTICATION_CLIENT,
															NULL,
															NULL,
															&error);

		if(pwm_connection == NULL)
		{
			g_printerr("Error connection to D-bus address %s: %s\n", IOTBUS_DBUS_PATH, error->message);
			g_error_free(error);
			return IOTBUS_ERROR_UNKNOWN;
		}
	}

	function = g_strdup(funcname);

	ret_value = g_dbus_connection_call_sync(pwm_connection,
										NULL,
										IOTBUS_DBUS_SERVICE,
										IOTBUS_DBUS_INTERFACE,
										sensorid,
										g_variant_new("(siiiii)", function, dev->device, dev->channel, dev->period, dev->duty_cycle, dev->enabled),
										G_VARIANT_TYPE ("(iii)"),
										G_DBUS_CALL_FLAGS_NONE,
										-1,
										NULL,
										&error);
	if(ret_value == NULL)
	{
		g_printerr("Error invoking %s () : %s\n", sensorid, error->message);
		g_error_free(error);
		return IOTBUS_ERROR_UNKNOWN;
	}

	g_variant_get(ret_value, "(iii)", &dev->period, &dev->duty_cycle, &ret);

	g_variant_unref(ret_value);

	return ret;
}
