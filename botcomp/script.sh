#!/bin/bash

MAC="96:90:16:63:2D:25"
HANDLE="0x0010"
CMD="6b7570"

# Запускаем gatttool интерактивно в фоне и сохраняем PID
gatttool -b "$MAC" -I > /tmp/gatttool.log 2>&1 &
GT_PID=$!
GT_STDIN="/proc/$GT_PID/fd/0"

# Ждём, пока соединение установится
sleep 10

# Отправляем команду
echo "char-write-req $HANDLE $CMD" > "$GT_STDIN"

sleep 10

# Можно завершить gatttool
kill $GT_PID

