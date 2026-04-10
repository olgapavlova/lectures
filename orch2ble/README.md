<img width="1280" height="720" alt="2026-04-10" src="https://github.com/user-attachments/assets/7ea27063-c406-441d-8a1a-a3cccb3fbc3f" />

# Изобретаем велосипед для кубика — придумываем бинарный протокол

Youtube-запись от `2026-04-10`: https://youtu.be/awWuWUPUrGc

## Много-много процессов, аж целый сервер
- **На входе** — неизвестно как зашифрованные байты
- А **на выходе** нужен JSON-поток событий с кубиком

```mermaid
---
config:
  theme: 'base'
  themeVariables:
    primaryColor: '#fff'
    primaryTextColor: '#000' 
    secondaryColor: '#fff' 
    tertiaryColor: '#fff'
---
flowchart TB
discover([Устройство]):::yellow

subgraph Протокол
orch((•)):::black
ble{{BLE-данные}}:::green
crypto{{Расшифровка}}:::green
semantic{{Упаковка}}:::green
orch <-.-> |получать| ble
orch <-.-> |понимать| crypto
orch <-.-> |переводить| semantic
end

server([WebSocket]):::red

discover ==> Протокол
Протокол ==> server

classDef yellow fill:#CC9105,stroke:#CC9105,color:#FDF9EC;
classDef green fill:#9A9A00,stroke:#9A9A00,color:#FDF9EC;
classDef red fill:#D65D0E,stroke:#D65D0E,color:#FDF9EC;
classDef black fill:#282828,stroke:#282828,color:#FFFFFF;
classDef white fill:#FDF9EC,stroke:#FDF9EC,color:#282828;

```

- Процессы общаются как угодно, но вряд ли JSON'ом :LiSmile: \
Между ними по сокетам ходят бинарные данные. В байтах.\
`00100111` `00110001` `10001111` `01100100` `10100111`\
&nbsp; \
&nbsp;
- Нам нужно **решить**, что, кому и зачем передавать
- А потом понять, как это делать **сырыми байтами**

## Что там у оркестратора с BLE?

```mermaid
sequenceDiagram
autonumber
participant orch
participant ble

rect rgb(0,200,200)
ble ->> orch: 😈 Пусти меня
orch -->> ble: 🐰 Заходи
activate ble
end

rect rgb(200,200,0)
orch ->> ble: 😈 Подключайся к 🌈
activate ble
ble -->> orch: 🐰 Подключился
end

rect rgb(200,200,0)
orch ->> ble: 😈 Подписывайся
activate ble
ble -->> orch: 🐰 Подписался
end

ble -) orch: 🍀 Услышал – сообщаю: 🚂
ble -) orch: 🍀 Услышал – сообщаю: ✈️
ble -) orch: 🍀 Услышал – сообщаю: ✈️
ble -) orch: 🔥 Что-то пошло не так: 🥊
ble -) orch: 🍀 Услышал – сообщаю: 🚂

rect rgb(200,200,0)
orch ->> ble: 😈 Отключайся
deactivate ble
deactivate ble
ble -->> orch: 🐰 Отключился
end

deactivate ble
```

### Содержание
1. Есть 😈 запросы и 🐰 ответы.
2. Есть просто сообщения — 🍀 хорошие и 🔥 так себе.
3. В запросах будут 🌈 подробности: куда подключиться?
4. В сообщениях точно будет 🚂 ✈️ 🥊 паровозик уточняющего контента.

### Форма
1. Ответ должен быть связан с запросом.
2. Для всего на свете полезны номера.

### Лучшие практики
1. Приличные процессы при встрече здороваются.
2. На старые письма не отвечаем.
3. ~~Анекдоты~~ Ошибки лучше сообщать по номерам.
4. Как всегда, стараемся форму отделить от содержания.


## И дальше прорабатываем бинарную реализацию

```mermaid
flowchart TB
subgraph struct
a(uint8_t):::yellow
b(uint16_t):::yellow
c(uint32_t):::yellow
d(uint16_t):::yellow
end

subgraph bytes
0((0)):::green
1((1)):::green
2((2)):::green
3((3)):::green
4((4)):::green
5((5)):::green
6((6)):::green
7((7)):::green
8((8)):::green
end

a ==> 0
b ==> 1
b ==> 2
c ==> 3
c ==> 4
c ==> 5
c ==> 6
d ==> 7
d ==> 8

classDef yellow fill:#CC9105,stroke:#CC9105,color:#FDF9EC;
classDef green fill:#9A9A00,stroke:#9A9A00,color:#FDF9EC;

```


1. Little endian — типичное дело для бинарных протоколов внутри системы.
2. Лучше анонсировать каждое сообщение кодовым ~~звонком~~ словом.
3. Идея: фиксированный размер для формы и переменный — для содержания.
4. А кстати, какой будет размер у формы? Надо договориться.
5. Полезную нагрузку ограничим сверху, чтобы не зависать.
6. И дадим ей минимальный объём, чтобы не использовать динамическую память.

> [!TIP]
> Можно знать — но можно и догадаться.\
> Все эти идеи бросаются в глаза.\
> Но, конечно, удобно к ним привыкнуть.\
> Понаписав разных протоколов и поначитавшись чужих.
