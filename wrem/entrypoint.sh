#!/bin/bash

# Получаем UID и GID из окружения (снаружи docker run)
USER_ID=${HOST_UID:-1000}
GROUP_ID=${HOST_GID:-1000}

# Создаём пользователя внутри контейнера с нужным UID/GID
addgroup --gid $GROUP_ID builduser
adduser --disabled-password --gecos "" --uid $USER_ID --gid $GROUP_ID builduser

# Передаём управление make от имени builduser
exec gosu builduser "${@:-make}"
