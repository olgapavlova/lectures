<img width="1280" height="720" alt="2025-12-19" src="https://github.com/user-attachments/assets/f643c2fa-1711-4dc2-877e-973d24689880" />

Youtube-запись от `2025-12-19`: https://youtu.be/E3TSUvToqCI

# Как, не задумываясь, писать embedded-код для любой железки?
`Дано:` ~20 микроконтроллеров + 38 плат + ~10 микрокомпьютеров

![IMG_0331](https://github.com/user-attachments/assets/738d8f40-a07b-4135-86b6-3d5458e5efa1)


`Требуется:` подключать к USB и писать код, не отвлекаясь на DevOps

`В чём проблема:` *toolchain — SDK — компилятор — отладчик — прошивка — debugger — лучше-бы-вы-взяли-STM32 — UART — IDE — плагины — под-BLE-своя-библиотека — терминал — провод — это-другой-shell — ОС — CMake — Espressif-не-даёт-datasheet — …*

> [!WARNING]
> A-a-a-a-a-a!!!

`Решение:` один набор команд — разные реализации

> [!TIP]
> Переизобретём IDE

## Один набор команд
С набором команд всё просто — это `Makefile`
```bash
make build
gcc -O2 -Wall -Wextra -Werror -std=c11 main.c -o main.run
```
**`make`** `new` — `build` — `flash` — `debug` — `config` — `info` — `monitor` — `clean` — `rebuild` — `dump` — `check` — `size` — …

> Или можно bash-скриптами, если `make` надоел
 
 ## А как сделать разные реализации?
~~Неправильный вопрос.~~ Правильный :LiFileQuestion: как их структурировать?
> [!NOTE]
> У нас же есть Docker!
> И главная фишка докера — слои

```mermaid
flowchart LR
subgraph Docker
a((Должно быть всюду)):::blue
b1((Кое-где)):::blue
b2((Кое-где)):::blue
d1((Ещё реже)):::blue
d2((Ещё реже)):::blue
d3((Ещё реже)):::blue
usb[USB]:::black
end

hard{{Железка}}:::green

a ==> b1
a ==> b2
b1 ==> d1
b1 ==> d2
b2 ==> d3

d1 <-.-> usb
d2 <-.-> usb
d3 <-.-> usb

usb <--> hard

classDef red fill:#be440b,stroke:#be440b,color:#d4c493;
classDef green fill:#7c7b00,stroke:#7c7b00,color:#d4c493;
classDef yellow fill:#af7a00,stroke:#af7a00,color:#d4c493;
classDef blue fill:#00696c,stroke:#00696c,color:#d4c493;
classDef white fill:#d4c493,stroke:#feeaac,color:#23211b;
classDef black fill:#23211b,stroke:#23211b,color:#d4c493;

```
> [!CAUTION]
> Надо как-то придумать систему слоёв
> А потом распихивать по ней наши утилиты

```mermaid
flowchart LR

user(Я):::red

subgraph Docker
commands([команды]):::yellow
container([контейнер]):::blue
usb[USB]:::black
end

hard{{Железка}}:::green

user -.-> commands

commands ==> container

container <==> usb

usb <==> hard

classDef red fill:#be440b,stroke:#be440b,color:#d4c493;
classDef green fill:#7c7b00,stroke:#7c7b00,color:#d4c493;
classDef yellow fill:#af7a00,stroke:#af7a00,color:#d4c493;
classDef blue fill:#00696c,stroke:#00696c,color:#d4c493;
classDef white fill:#d4c493,stroke:#feeaac,color:#23211b;
classDef black fill:#23211b,stroke:#23211b,color:#d4c493;

```
### Кое-что уже придумано до нас

```mermaid
flowchart LR
toolchain((toolchain)):::blue
platform((платформа)):::blue
profile([режим работы]):::blue
project([проект]):::blue

toolchain ==> platform
platform -.-> profile
platform -.-> project
profile -.-> project

classDef blue fill:#00696c,stroke:#00696c,color:#d4c493;
```
> [!WARNING]
> И теперь — сиди себе распихивай утилиты
> Не то чтобы это рутинная работа, о нет

#### `toolchain` • система сборки бинарных файлов под архитектуру
> [!TIP]
> Полностью виртуальная штука
> Архитектура же.

Имя :LiBotMessageSquare: `<архитектрура>-<производитель>-<ОС>-<abi>/<среда>`

> [!WARNING]
> А давайте Dockerfile тоже будем генерить?
> А то что-то скучно стало и слишком просто

```mermaid
flowchart LR
yaml([YAML]):::white
docker([Dockerfile]):::blue

yaml ==> |скрипт| docker

classDef white fill:#d4c493,stroke:#feeaac,color:#23211b;
classDef blue fill:#00696c,stroke:#00696c,color:#d4c493;

```



#### `platform` • приземление `toolchain` на  устройство или класс
Как-то же надо залить прошивку на нашу плату.
И смотреть на вывод платы.
И мало ли что ещё.

Имя `<экосистема>-<семейство>`

> [!IMPORTANT]
> Уже можно интерактивно подключаться
> Но «можно» не всегда значит «нужно»


#### `profile` • режимы работы с конкретной платформой
> [!TIP]
> Ключевой вопрос: что вы собираетесь делать прямо сейчас?
> Писать код, отлаживать, прошивать, анализировать бинарники,…

`dev` — `debug` — `flash` — `ci` — `release` — `analysis` — …


#### `project` • конкретика проекта
> [!WARNING]
> Делать под это контейнер — лишнее
> Достаточно слоя `COPY` или даже монтирования `value`
> Хотя…
