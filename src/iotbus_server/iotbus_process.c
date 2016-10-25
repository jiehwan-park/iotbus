#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <capi_iotbus_gpio.h>
#include <capi_iotbus_i2c.h>
#include <capi_iotbus_error.h>
#include "iotbus_dbus.h"
#include "iotbus_server.h"

static GDBusNodeInfo *introspection_data = NULL;

static void
handle_method_call (GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *interface_name,
                    const gchar           *method_name,
                    GVariant              *parameters,
                    GDBusMethodInvocation *invocation,
                    gpointer               user_data)
{

	gchar * function;
	gint32 ret = IOTBUS_ERROR_NONE;

	if(parameters == NULL)
	{
		g_print("Client : parameters Null !\n");
		return ;
	}

	if(invocation == NULL)
	{
		g_print("client : invocation NULL !\n");
		return ;
	}

	if(method_name == NULL)
	{
		g_print("Client : method_name NULL ! \n");
		return ;
	}

	if(!strcmp(method_name, IOTBUS_METHOD_GPIO))
	{
		gint32 pin;
		gint32 dir;
		gint32 edge;
		gint32 read_value = 0;
		gint32 write_value;
		struct _iotbus_gpio_s st_gpio;

		g_variant_get (parameters, "(siiii)", &function, &pin, &dir, &edge, &write_value);

		st_gpio.pin = pin;
		st_gpio.edge = edge;
		st_gpio.dir = dir;

		ret = iotbus_server_gpio_process(&st_gpio, function, write_value, &read_value);

		g_dbus_method_invocation_return_value (invocation,
											 g_variant_new ("(iiiii)", st_gpio.pin, st_gpio.dir, st_gpio.edge, read_value, ret));


	}
	else if(!strcmp(method_name, IOTBUS_METHOD_I2C))
	{
		gint32 fd;
		GVariant *data;
		gchar *str;
		gchar data_value[100];
		GVariantBuilder *builder;
		gint32 value = 0;
		gint32 addr;
		int i = 0;
		struct _iotbus_i2c_s st_i2c;

		g_variant_get (parameters, "(siiayi)", &function, &value, &fd, &data, &addr);

		if(!strcmp(function, "READ") || !strcmp(function, "WRITE"))
		{
			while(g_variant_iter_loop(data, "y", &str))
			{
				data_value[i] = str;
				i++;
			}
		}

		g_variant_iter_free(data);

		st_i2c.fd = fd;

		ret = iotbus_server_i2c_process(&st_i2c, function, value, data_value, addr);

		builder = g_variant_builder_new(G_VARIANT_TYPE ("ay"));

		if(!strcmp(function, "READ") || !strcmp(function, "WRITE"))
		{
			for(i = 0; i< value; i++)
			{
				g_variant_builder_add(builder, "y", data_value[i]);
			}
		}
		else
		{
			g_variant_builder_add(builder, "y", 0x10);
			g_variant_builder_add(builder, "y", 0x10);
		}


		g_dbus_method_invocation_return_value (invocation,
											 g_variant_new ("(iayi)", st_i2c.fd,builder, ret));

		g_print("server OK!!\n");

	}
	else if(!strcmp(method_name, IOTBUS_METHOD_PWM)) //pwm
	{
		gint32 fd;
		guchar *data;
		gint32 value = 0;
		struct _iotbus_pwm_s st_pwm;

		g_variant_get (parameters, "(siiiii)", &function, &st_pwm.device, &st_pwm.channel, &st_pwm.period, &st_pwm.duty_cycle, &st_pwm.enabled);

		ret = iotbus_server_pwm_process(&st_pwm, function);

		g_dbus_method_invocation_return_value (invocation,
											 g_variant_new ("(iii)", st_pwm.period, st_pwm.duty_cycle, ret));
	}


}

static const GDBusInterfaceVTable interface_vtable =
{
  handle_method_call,
  NULL,
  NULL,
};

guint registration_id = 0;

static gboolean on_new_connection(GDBusServer *server,
										GDBusConnection *connection,
										gpointer user_data)
{

	GCredentials *credentials;
	gchar *s;

	//if(registration_id == 0)
	//{
	//	credentials = g_dbus_connection_get_peer_credentials(connection);
	//	if(credentials == NULL)
	//	{
	//		s = g_strdup("(no credentials received)");
	//	}
	//	else
	//	{
	//		s = g_credentials_to_string(credentials);
	//	}
	//}
		g_print("Client connected. \n");

		g_object_ref(connection);
		registration_id = g_dbus_connection_register_object(connection,
															IOTBUS_DBUS_SERVICE,
															introspection_data->interfaces[0],
															&interface_vtable,
															NULL,
															NULL,
															NULL);


	return TRUE;
}

int
main (int argc, char *argv[])
{
	GDBusServer *server;
	gchar *guid;
	GMainLoop *loop;
	GDBusServerFlags server_flags;
	GError *error;


	gint ret = 1;


	g_type_init();

	introspection_data = g_dbus_node_info_new_for_xml(iotbus_data_xml, NULL);
	g_assert (introspection_data != NULL);

	guid = g_dbus_generate_guid();

	server_flags = G_DBUS_SERVER_FLAGS_NONE;

	error = NULL;
	printf("server start!!!!\n");
	server = g_dbus_server_new_sync(IOTBUS_DBUS_PATH,
									server_flags,
									guid,
									NULL, /* GDBusAuthObserver*/
									NULL, /* GCancellable */
									&error);
	if(server == NULL)
	{
		g_printerr ("Error creating server at address %s: %s\n", IOTBUS_DBUS_PATH, error->message);
		g_error_free(error);
		return ret;
	}
	printf("server start 2!!!!\n");
	g_dbus_server_start(server);
	printf("server start 3!!!!\n");
	if(guid != NULL)
		g_free(guid);
	printf("server start 4!!!!\n");
	g_signal_connect(server,
					"new-connection",
					G_CALLBACK(on_new_connection),
					NULL);
	printf("server start 5!!!!\n");
	loop = g_main_loop_new(NULL, FALSE);
	printf("server start 6!!!!\n");
	g_main_loop_run(loop);
	printf("server start 7!!!!\n");
	if(server != NULL)
		g_object_unref(server);
	printf("server start 8!!!!\n");
	if(loop != NULL)
		g_main_loop_unref(loop);

	return 0;
}
