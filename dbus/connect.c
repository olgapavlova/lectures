// connect_and_wait.c
#include <gio/gio.h>
#include <stdio.h>

#define BLUEZ_BUS   "org.bluez"
#define DEVICE_PATH "/org/bluez/hci0/dev_EF_C5_C2_AA_96_39"

static gboolean get_bool_property(
    GDBusConnection *conn,
    const char *object_path,
    const char *iface_name,
    const char *prop_name,
    gboolean *out_value
) {
    GError *error = NULL;

    GVariant *result = g_dbus_connection_call_sync(
        conn,
        BLUEZ_BUS,
        object_path,
        "org.freedesktop.DBus.Properties",
        "Get",
        g_variant_new("(ss)", iface_name, prop_name),
        G_VARIANT_TYPE("(v)"),
        G_DBUS_CALL_FLAGS_NONE,
        -1,
        NULL,
        &error
    );

    if (!result) {
        fprintf(stderr, "Get(%s.%s) failed: %s\n",
                iface_name, prop_name, error->message);
        g_error_free(error);
        return FALSE;
    }

    GVariant *boxed = NULL;
    GVariant *inner = NULL;

    g_variant_get(result, "(@v)", &boxed);
    inner = g_variant_get_variant(boxed);

    if (!g_variant_is_of_type(inner, G_VARIANT_TYPE_BOOLEAN)) {
        fprintf(stderr, "Property %s.%s is not boolean\n", iface_name, prop_name);
        g_variant_unref(inner);
        g_variant_unref(boxed);
        g_variant_unref(result);
        return FALSE;
    }

    *out_value = g_variant_get_boolean(inner);

    g_variant_unref(inner);
    g_variant_unref(boxed);
    g_variant_unref(result);
    return TRUE;
}

static gboolean call_connect(GDBusConnection *conn, const char *object_path) {
    GError *error = NULL;

    GVariant *result = g_dbus_connection_call_sync(
        conn,
        BLUEZ_BUS,
        object_path,
        "org.bluez.Device1",
        "Connect",
        NULL,
        NULL,
        G_DBUS_CALL_FLAGS_NONE,
        -1,
        NULL,
        &error
    );

    if (!result) {
        fprintf(stderr, "Connect failed: %s\n", error->message);
        g_error_free(error);
        return FALSE;
    }

    g_variant_unref(result);
    return TRUE;
}

static gboolean wait_until_true(
    GDBusConnection *conn,
    const char *object_path,
    const char *iface_name,
    const char *prop_name,
    int max_attempts,
    int sleep_ms
) {
    for (int i = 0; i < max_attempts; i++) {
        gboolean value = FALSE;

        if (!get_bool_property(conn, object_path, iface_name, prop_name, &value)) {
            return FALSE;
        }

        printf("%s = %s\n", prop_name, value ? "true" : "false");

        if (value) {
            return TRUE;
        }

        g_usleep((gulong)sleep_ms * 1000);
    }

    fprintf(stderr, "Timeout waiting for %s=true\n", prop_name);
    return FALSE;
}

int main(void) {
    GError *error = NULL;

    GDBusConnection *conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (!conn) {
        fprintf(stderr, "g_bus_get_sync failed: %s\n", error->message);
        g_error_free(error);
        return 1;
    }

    if (!call_connect(conn, DEVICE_PATH)) {
        g_object_unref(conn);
        return 1;
    }

    if (!wait_until_true(conn, DEVICE_PATH, "org.bluez.Device1", "Connected", 20, 250)) {
        g_object_unref(conn);
        return 1;
    }

    if (!wait_until_true(conn, DEVICE_PATH, "org.bluez.Device1", "ServicesResolved", 40, 250)) {
        g_object_unref(conn);
        return 1;
    }

    puts("Device is connected and services are resolved.");

    g_object_unref(conn);
    return 0;
}
