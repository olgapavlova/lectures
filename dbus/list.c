/** @file  list.c
  * @brief Список сервисов на шине DBus
  */

#include <gio/gio.h>
#include <stdio.h>

int main(void) {
    GError *error = NULL;

    // Подключаемся к шине
    GDBusConnection *conn = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (!conn) {
        fprintf(stderr, "g_bus_get_sync: %s\n", error->message);
        g_error_free(error);
        return 1;
    }

    // Обращаемся к шине
    GVariant *result = g_dbus_connection_call_sync(
        conn,                            // соединение
        "org.freedesktop.DBus",          // сервис
        "/org/freedesktop/DBus",         // объект
        "org.freedesktop.DBus",          // интерфейс
        "ListNames",                     // метод
        NULL,                            // аргументы
        G_VARIANT_TYPE("(as)"),          // ожидаемый тип ответа
        G_DBUS_CALL_FLAGS_NONE,          // флаги вызова
        -1,                              // ждём в мс
        NULL,                            // отменяемость
        &error                           // куда ошибку
    );
    if (!result) {
        fprintf(stderr, "call failed: %s\n", error->message);
        g_error_free(error);
        g_object_unref(conn);
        return 1;
    }

    // Получаем имена
    gchar **names = NULL;
    g_variant_get(result, "(^as)", &names);

    // Выводим результат
    for (gsize i = 0; names[i] != NULL; i++) {
        puts(names[i]);
    }

    // Освобождаем память
    g_strfreev(names);
    g_variant_unref(result);
    g_object_unref(conn);

    return 0;
}
