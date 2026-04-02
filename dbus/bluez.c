/** @file  bluez.c
  * @brief Интроспекция Bluetooth-сервиса
  */

#include <gio/gio.h>
#include <stdio.h>

int main(void) {
    GError *error = NULL;

    GDBusConnection *conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (!conn) {
        fprintf(stderr, "g_bus_get_sync: %s\n", error->message);
        g_error_free(error);
        return 1;
    }

    GVariant *result = g_dbus_connection_call_sync(
        conn,
        "org.bluez",
        "/org/bluez/hci0",
        "org.freedesktop.DBus.Introspectable",
        "Introspect",
        NULL,
        G_VARIANT_TYPE("(s)"),
        G_DBUS_CALL_FLAGS_NONE,
        -1,
        NULL,
        &error
    );

    if (!result) {
        fprintf(stderr, "call failed: %s\n", error->message);
        g_error_free(error);
        g_object_unref(conn);
        return 1;
    }

    const gchar *xml = NULL;
    g_variant_get(result, "(&s)", &xml);
    puts(xml);

    g_variant_unref(result);
    g_object_unref(conn);
    return 0;
}
