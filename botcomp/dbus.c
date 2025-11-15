#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main() {
    DBusConnection* conn;
    DBusMessage* msg;
    DBusMessageIter args, array, dict;
    DBusError err;

    const char* char_path =
        "/org/bluez/hci0/dev_96_90_16_63_2D_25/service0023/char0044";

    // Байты для записи
    uint8_t data[] = {0x6b, 0x75, 0x70}; // "kup"

    dbus_error_init(&err);

    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (!conn) {
        fprintf(stderr, "Failed to connect to system bus: %s\n", err.message);
        return 1;
    }

    // Создаём D-Bus метод-вызов
    msg = dbus_message_new_method_call(
        "org.bluez",          // сервис
        char_path,            // путь к характеристике
        "org.bluez.GattCharacteristic1", // интерфейс
        "WriteValue"          // метод
    );

    if (!msg) {
        fprintf(stderr, "Failed to create message\n");
        return 1;
    }

    dbus_message_iter_init_append(msg, &args);

    // Первый аргумент — массив байтов
    dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "y", &array);
    for (size_t i = 0; i < sizeof(data); i++) {
        dbus_message_iter_append_basic(&array, DBUS_TYPE_BYTE, &data[i]);
    }
    dbus_message_iter_close_container(&args, &array);

    // Второй аргумент — словарь (dict) опций, обычно пустой
    dbus_message_iter_open_container(&args, DBUS_TYPE_ARRAY, "{sv}", &dict);
    dbus_message_iter_close_container(&args, &dict);

    // Отправка
    dbus_connection_send(conn, msg, NULL);
    dbus_connection_flush(conn);

    dbus_message_unref(msg);

    printf("WriteValue sent!\n");
    return 0;
}

