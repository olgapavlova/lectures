<img width="1280" height="720" alt="2025-11-14" src="https://github.com/user-attachments/assets/77228424-6609-4c2e-ab58-b35b564ba505" />

Youtube-запись от `2025-11-14`: https://youtu.be/47-qdqS88AE
# Управляем робособакой через CLI

## Устанавливаем Bluetooth-соединение
В собаке стоит BLE-модуль (не HM-10 или HM-11. а родной). Он работает по BLE. Не поддерживают RFCOMM, поддерживают GATT.
### Подключиться к :LiDog: как к физическому устройству
Установить утилиты:
```bash
sudo apt install bluez bluez-hcidump
```
Запустить утилиту работы с Bluetooth:
```bash
bluetoothctl
```
Посмотреть список зарегистрированных устройств:
```bash
[bluetooth]# devices
Device 96:90:16:63:2D:25 PetoiBLE-2D25
```
Просканировать устройства:
```bash
[bluetooth]# scan on
... (тут среди прочего будет MAC-адрес собаки)
[bluetooth]# scan off
```
Подключиться к собаке по Bluetooth с помощью MAC-адреса:
```bash
pair 96:90:16:63:2D:25
Attempting to pair with 96:90:16:63:2D:25
[PetoiBLE-2D25]# [CHG] Device 96:90:16:63:2D:25 Connected: yes
[PetoiBLE-2D25]# [NEW] Primary Service (Handle 0x000a)
	/org/bluez/hci0/dev_96_90_16_63_2D_25/service000a
	00001801-0000-1000-8000-00805f9b34fb
	Generic Attribute Profile
[PetoiBLE-2D25]# [NEW] Characteristic (Handle 0x000b)
	/org/bluez/hci0/dev_96_90_16_63_2D_25/service000a/char000b
	00002a05-0000-1000-8000-00805f9b34fb
	Service Changed
[PetoiBLE-2D25]# [NEW] Descriptor (Handle 0x1d31)
	/org/bluez/hci0/dev_96_90_16_63_2D_25/service000a/char000b/desc000d
	00002902-0000-1000-8000-00805f9b34fb
	Client Characteristic Configuration
[PetoiBLE-2D25]# [NEW] Primary Service (Handle 0x000e)
	/org/bluez/hci0/dev_96_90_16_63_2D_25/service000e
	0000ffe0-0000-1000-8000-00805f9b34fb
	Unknown
[PetoiBLE-2D25]# [NEW] Characteristic (Handle 0x000f)
	/org/bluez/hci0/dev_96_90_16_63_2D_25/service000e/char000f
	0000ffe1-0000-1000-8000-00805f9b34fb
	Unknown
[PetoiBLE-2D25]# [NEW] Descriptor (Handle 0x1d32)
	/org/bluez/hci0/dev_96_90_16_63_2D_25/service000e/char000f/desc0011
	00002902-0000-1000-8000-00805f9b34fb
	Client Characteristic Configuration
[PetoiBLE-2D25]# [NEW] Characteristic (Handle 0x0012)
	/org/bluez/hci0/dev_96_90_16_63_2D_25/service000e/char0012
	0000ffe2-0000-1000-8000-00805f9b34fb
	Unknown
[PetoiBLE-2D25]# [CHG] Device 96:90:16:63:2D:25 ServicesResolved: yes
[PetoiBLE-2D25]# [CHG] Device 96:90:16:63:2D:25 Paired: yes
[PetoiBLE-2D25]# Pairing successful
```
Подключиться к собаке через Bluetooth:
```bash
connect 96:90:16:63:2D:25
Attempting to connect to 96:90:16:63:2D:25
[PetoiBLE-2D25]# Connection successful
```
Это чисто для проверки, нам не пригодится. Отключаемся:
```bash
bluetoothctl disconnect 96:90:16:63:2D:25
```
(или прямо эта же команда `disconnect`из `bluetoothctl`)

Выходим в bash: `quit` или `exit`

Получить информацию о собаке:
```bash
bluetoothctl info 96:90:16:63:2D:25
Device 96:90:16:63:2D:25 (public)
	Name: PetoiBLE-2D25
	Alias: PetoiBLE-2D25
	Appearance: 0x0002 (2)
	Icon: unknown
	Paired: no
	Bonded: no
	Trusted: yes
	Blocked: no
	Connected: no
	LegacyPairing: no
	UUID: Generic Access Profile    (00001800-0000-1000-8000-00805f9b34fb)
	UUID: Generic Attribute Profile (00001801-0000-1000-8000-00805f9b34fb)
	UUID: Tencent Holdings Limited. (0000fee7-0000-1000-8000-00805f9b34fb)
	UUID: Unknown                   (0000ffe0-0000-1000-8000-00805f9b34fb)
	ManufacturerData.Key: 0x0c37 (3127)
	ManufacturerData.Value:
  88 a0 96 90 16 62 2d 25                          .....b-%
	AdvertisingFlags:
  06                                               .
```
В UUID видно, что мы имеем дело с BLE-модулями: `…f9b34fb`

### Соединиться с :LiDog: через CLI для отправки команд
> [!WARNING] Очень низкоуровневый способ
> Работает всегда. А толку, если нам нужны уровни повыше.

Подключиться через `gatttool` (GATT):
```bash
sudo gatttool -b 96:90:16:63:2D:25 -I
```

Соединиться:
```bash
connect
```
Получить список handles:
```bash
char-desc
```
Результат:
```
handle: 0x0001, uuid: 00002800-0000-1000-8000-00805f9b34fb
handle: 0x0002, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x0003, uuid: 00002a00-0000-1000-8000-00805f9b34fb
handle: 0x0004, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x0005, uuid: 00002a01-0000-1000-8000-00805f9b34fb
handle: 0x0006, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x0007, uuid: 00002a02-0000-1000-8000-00805f9b34fb
handle: 0x0008, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x0009, uuid: 00002a04-0000-1000-8000-00805f9b34fb
handle: 0x000a, uuid: 00002800-0000-1000-8000-00805f9b34fb
handle: 0x000b, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x000c, uuid: 00002a05-0000-1000-8000-00805f9b34fb
handle: 0x000d, uuid: 00002902-0000-1000-8000-00805f9b34fb
handle: 0x000e, uuid: 00002800-0000-1000-8000-00805f9b34fb
handle: 0x000f, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x0010, uuid: 42696f74-6550-090e-ffff-00ffe2ffe1ff
handle: 0x0011, uuid: 00002902-0000-1000-8000-00805f9b34fb
handle: 0x0012, uuid: 00002803-0000-1000-8000-00805f9b34fb
handle: 0x0013, uuid: 454c4269-6f74-6550-090e-ffff00ffe2ff
```

Видим разные handle. Фактически это сервисы для информирования про разное. Служебные BLE-сервисы.
Тут нет ожидаемой строки с `uuid: 0000ffe1` — и это значит, что модуль не HM-10, а что-то другое. И GATT-профиль там кастомный.
В этом профиле на приём данных собака использует handle 0x0010, а на отправку — handle 0x0013. Видно, что их UUID отличаются от остальных.
Дальше команды отправлять так:
```bash
char-write-req 0x0010 [16-ричный код команды без 0x]
```
Квадратные скобки не нужны. Просто число.

Подписаться на все сообщения от собаки:
```bash
char-write-req 0x0011 0100
```
Отписаться от сообщений (уведомлений):
```bash
char-write-req 0x0011 0000
```
Почему `0x0011`? Потому что handle стоит следующим после канала отправки (0x010, RX, write-канал). Такая уж собака. Называется этот handle CCCD (Client Characteristic Configuration Descriptor).
Кстати, 0x0013 — это TX-канал, по нему бы принимать. Но принимать по своему желанию нельзя. Только получать нотификации.

Напишем микропрограмму для превращения инструкций в 16-ричные коды.

### Соединиться по шине D-Bus
> [!ALERT] Это высокоуровневое соединение
> Оно использует `bluetoothctl` — что не то же самое, что делает `gatttool` напрямую. Могут быть неожиданности.

На всякий случай перезапустим демон:
```bash
sudo systemctl stop bluetooth
sudo systemctl start bluetooth
```

Подключимся именно по BLE (не по классическому Bluetooth):
```bash
bluetoothctl
menu scan
transport le
back
scan on
```
По ходу сканирования увидим два устройства:
```bash
devices
Device 96:90:16:63:2D:25 PetoiBLE-2D25
Device 96:90:16:62:2D:25 Petoi-2D25
```
Дальше коннект:
```bash
trust [MAC]
connect [MAC]
pair [MAC]
```

Посмотрим на файловую структуру BlueZ:
```bash
busctl tree org.bluez
```
> [!TIP] Показывает не все сервисы
> Только служебные и те, к которым уже было обращение

Узнаем вообще всё что можно:
```bash
busctl introspect org.bluez /org/bluez/hci0/dev_96_90_16_63_2D_25
```
## Пишем код
Подбираемся к C. Вот что может пригодиться:
- Linux BLE API (BlueZ)
- D-Bus API BlueZ (гибче, сложней, зато библиотеки встроены)
- [gattlib](https://github.com/labapart/gattlib) (обёртка над BlueZ, удобнее)

Полезно узнать версию и детали про BlueZ:
```bash
dpkg -l | grep bluez
```
### Работаем через библиотеку `gattlib`
Устанавливаем gattlib:
```bash
git clone git@github.com:labapart/gattlib.git
sudo apt install libglib2.0-dev libbluetooth-dev libreadline-dev libpcre3-dev
cd gattlib/
mkdir build && cd build
cmake ..
make
sudo make install
sudo ldconfig
``` 
Может жаловаться на отсутствие ещё каких-нибудь библиотек. Встаёт, в общем, не очень гладко. Но встаёт.
Проверим:
```bash
ldconfig -p | grep gattlib
```
(динамический линкер, показывает всё, что может динамически подключить)
После этого используем в C-коде:
```c
#include <gattlib.h>
```

### Используем D-Bus API BlueZ напрямую
Для разработки нужно установить кое-какие dev-пакеты:
```bash
sudo apt install libglib2.0-dev libudev-dev libdbus-1
```
> [!TIP]- Что за dev-пакеты такие?
> Библиотека = часть для запуска + часть для разработки
> <br/>&nbsp;<br/>
> Часть для запуска — `.so` (динамическая библиотека); только используем в уже откомпилированных программах
> <br/>&nbsp;<br/>
> Часть для разработки — `.h` (заголовки) + `.pc` (pkgconf отсюда берёт флаги и прочую инфу для компиляции) + `.a` (статическая библиотека); цепляем к своим программам

При компиляции на них нужно ссылаться через утилиту `pkgconf`:
```make
CFLAGS = $(pkgconf --cflags --lib [название библиотеки])
```

> [!WARNING] …ииии ничего не работает

Спойлер: и не заработает. Почему? Надо разбираться.

А пока решение грубой силой: использовать код на языке expect.
Да, есть такой язык. Код в репозитории. `sleep` там важен. Есть буфер вывода, не паникуем.
