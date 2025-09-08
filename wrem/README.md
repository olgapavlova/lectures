
<img width="1280" height="720" alt="2025-08-01" src="https://github.com/user-attachments/assets/8518856f-5bf8-4a92-9fb5-94763c56e515" />

# Крохотный конечный автомат на ATtiny13A и C

Youtube-запись от `2025-08-01`: https://youtu.be/EJmQKRU_cpw

## Напоминает, когда пора пить

- [x]  Просто таймер
- [ ]  Зависимость от температуры
- [ ]  Зависимость от веса человека
- [ ]  Симпатичные светоэффекты
- [ ]  Звук
- [ ]  Портативность
- [ ]  SMD-вариант
- [ ]  Гироскоп на кружку
- [ ]  …

![IMG_9108.jpeg](attachment:476d972e-514c-4cc8-a5b5-c69254d964c5:IMG_9108.jpeg)

## Состояния и связь между ними

```mermaid
flowchart TB
start(Начинаем)
waiting(Ждём)
check(Проверяем)
alarm(Пора пить)
drop(Заново)

start:::black ==> waiting:::green
waiting ==> check:::black
check ==> waiting
check --> alarm:::red
alarm --> drop:::yellow
drop --> waiting

classDef red fill:#FE8019;
classDef green fill:#B8BA46;
classDef yellow fill:#FABD2F;
classDef black fill:#504945,color:#fff;
```

<aside>
<img src="/icons/log-out_blue.svg" alt="/icons/log-out_blue.svg" width="40px" />

**Ищем только выходы**
Потому что всякий вход — это выход откуда-то.

</aside>

<aside>
<img src="/icons/condense_green.svg" alt="/icons/condense_green.svg" width="40px" />

**Внутри блока — свои дела**
Что-то вроде игры по станциям, или карты с квестами, или разных уроков/пар/объектов

</aside>

<aside>
<img src="/icons/apron_yellow.svg" alt="/icons/apron_yellow.svg" width="40px" />

**Дела-то разные!**
Дела зависят от результатов проверки. Эти результаты и есть «состояния».

</aside>

## Здравствуй, машина состояний

> Способ ~~разложить всё по полочкам~~ структурировать поведение системы.
> 

```mermaid
flowchart TB
do(Делаем)
check(Обновляемся)
do:::green <==> check:::black

classDef green fill:#B8BA46;
classDef black fill:#504945,color:#fff;
```

<aside>
<img src="/icons/sync-reverse_purple.svg" alt="/icons/sync-reverse_purple.svg" width="40px" />

**Туда-сюда без остановки**
Как закончили делать — сразу обновляем. Как закончили проверять — сразу делаем.

</aside>

<aside>
<img src="/icons/archery_orange.svg" alt="/icons/archery_orange.svg" width="40px" />

**Впервые или снова?**
Мы не знаем, когда дело будет сделано в последний раз. Но знаем, когда в первый (или даже в N-й).

</aside>

<aside>
<img src="/icons/alarm_lightgray.svg" alt="/icons/alarm_lightgray.svg" width="40px" />

**Одно обновление на все дела?!**
Да. Не усложняем пока что.

</aside>

```c
int main(void) {
	while(1) {
		state_do();
		state_update();
	}
	return 0;
}
```

- Порядок важен
- Как `state_do();` узнает, что там наизменял `state_update();`?
- И как `state_update();` изменит состояние, о котором должен знать `state_do();`?

> Глобальная переменная?! 😑
Или передавать по ссылке.
> 

## Желания и возможности

```mermaid
flowchart LR
subgraph Возможности
light(Светодиод):::green
temp(Температура):::yellow
time(Время):::green
drop(Кнопка сброса):::green
end

subgraph Паттерны
shim(ШИМ):::black
wire(Протокол):::black
ii(Прерывание):::green
end

subgraph Контроллер
timer(Таймер):::red
dpin(Цифровые вх-вых):::green
apin(Аналоговые входы):::black
end

light -.-> |мерцание| timer

light -.-> |яркость| shim
shim -.-> timer

temp -.-> |цифровой| wire
wire -.-> timer

temp -.-> |аналоговый| ii
ii -.-> apin

drop -.-> ii
ii -.-> dpin

time -.-> ii

classDef red fill:#FE8019;
classDef green fill:#B8BA46;
classDef yellow fill:#FABD2F;
classDef black fill:#504945,color:#fff;
```

<aside>
<img src="/icons/alarm_red.svg" alt="/icons/alarm_red.svg" width="40px" />

Таймер — узкое место

</aside>

<aside>
<img src="/icons/hammer_gray.svg" alt="/icons/hammer_gray.svg" width="40px" />

Грубо можно сделать программно

</aside>

<aside>
<img src="/icons/wifi_pink.svg" alt="/icons/wifi_pink.svg" width="40px" />

Протокол капризен ко времени

</aside>

## Код на регистрах

### Базовые библиотеки

```c
#include <avr/io.h>
#include <avr/interrupt.h>
```

### Пины

```c
#define LED_PIN PB4
#define BUTTON_PIN PB3
```

```c
    DDRB  |= (1 << LED_PIN);      // настроить LED_PIN как выход
    DDRB  &= ~(1 << BUTTON_PIN);  // настроить BUTTON_PIN как вход
    PORTB &= ~(1 << BUTTON_PIN);  // отключить внутреннюю подтяжку (есть внешняя)
```

### Инициация счётчика времени

```c
int main(void) {
	...
	timer_init();
  sei();
  while(1) {
	  ...
  }
}
```

```c
void timer_init() {
    TCCR0A = 0;                          // normal mode
    TCCR0B |= (1 << CS02) | (1 << CS00); // делитель /1024
    TIMSK0 |= (1 << TOIE0);              // разрешить прерывание по переполнению
    TCNT0 = 0;                           // стартовое значение счётчика
}
```

### Прерывание по переполнению счётчика времени

```c
ISR(TIM0_OVF_vect) {
    ticks_g++;                             // volatile!
    check_g = ticks_g % CHECK_EVERY_TICK;  // фокус с периодичностью
    if (ticks_g >= TICKS_IN_TIME_UNIT) {   // считаем в юнитах
        ticks_g = 0;
        units_g++;
    }
}
```

### Операции с volatile-переменными

```c
void func() {
    cli(); // временно отключим прерывания
		...
    sei(); // включим обратно
}
```

### Нажата ли кнопка

```c
((PINB & (1 << BUTTON_PIN)) == 0)
```

### Вкл/выкл/перекл светодиод

```c
PORTB |= (1 << LED_PIN);
PORTB &= ~(1 << LED_PIN);
PORTB ^= (1 << LED_PIN);
```
