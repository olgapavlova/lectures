![2025-03-07](https://github.com/user-attachments/assets/b0bd84a2-8618-44bd-a7f7-9a3a215481ae)

Youtube-запись лекции от ```2025-03-07```: https://youtu.be/dYg2zZPXJTE

# Механизм setjmp()/longjmp()

## …но сначала про системное программирование

## Ух ты, программа!

> Структурное программирование
> 
- Один компьютер — одна программа
- Фокус на структуре кода

## Вас много, я одна(ин)

> Системное программирование
> 
- Много программ в одном компьютере
- Фокус на ресурсах компьютера
- Прикладные программы отдельно

<aside>
<img src="/icons/conceal_brown.svg" alt="/icons/conceal_brown.svg" width="40px" />

**1950+**

</aside>

<aside>
<img src="/icons/electric-guitar_orange.svg" alt="/icons/electric-guitar_orange.svg" width="40px" />

**1960+**

</aside>

<aside>
<img src="/icons/apple_green.svg" alt="/icons/apple_green.svg" width="40px" />

**Кто-то должен сделать** то, чем другие программисты пользуются

</aside>

- Меньше абстракций и прочих фреймворков
- Ближе к ~~земле~~ железу
- Производительность — наше всё
- Оптимизация под процессор
- Ассемблер за углом
- Безопасность, доступы, потоки, память напрямую
- Драйвера и микропроцессоры (будет отдельно)

> Забудьте всё то, чему вас учили в школе — вот оно!
> 

<aside>
<img src="/icons/help-alternate_orange.svg" alt="/icons/help-alternate_orange.svg" width="40px" />

**Да как забыть-то?!**

</aside>

<aside>
<img src="/icons/sign-out_orange.svg" alt="/icons/sign-out_orange.svg" width="40px" />

**Почувствуй боль перехода.** Особенно если ты из 2020-х с AI и зоопарком железа

</aside>

# Первая жертва  — безусловный переход

<aside>
<img src="/icons/thinking_blue.svg" alt="/icons/thinking_blue.svg" width="40px" />

**А вот бы нам уметь сохраняться!** Респауниться. Возрождаться. Как в играх.

</aside>

- Да-да, это `goto()`, и это серьёзно
- Сохраняться **где**? Респауниться **куда**? Восстанавливать **что**?
- Как вообще устроена выполняющаяся программа?

https://en.cppreference.com/w/c/program/setjmp

https://en.cppreference.com/w/c/program/longjmp

```mermaid
flowchart LR

subgraph Память
stack(Локальные переменные)
heap(Динамическая память)
static(static)
text(Машинные инструкции)

end

subgraph Регистры_Процессора
pc(Адрес текущей инструкции)
sp(Адрес вершины стека)
bp(Адрес начала кадра)
other(Всякое другое)
end

pc -.-> |Что я делаю сейчас?| text
sp -.-> |Где кончается стек?| stack
bp -.-> |Где кадр функции?| stack

```

<aside>
<img src="/icons/dna_orange.svg" alt="/icons/dna_orange.svg" width="40px" />

**Разобраться стоит.** Но потом.

</aside>

## Управляем поведением

- `setjmp()` — сохраняем точку возврата
- `longjmp()` — возвращаемся в эту точку
- туда-сюда передаём код выхода из точки возврата

> Точек возврата и кодов может быть **несколько**
> 

## Ничего не обещаем про данные

- глобальные сохраняются
- динамическая память остаётся как была
- 👾 **локальные непредсказуемы** 👾 👾 👾
    - тут поможет `volatile` — жёсткая привязка к памяти, никаких регистров и прочих оптимизаций
- файлы и т. п. остаются как были

<aside>
<img src="/icons/fireworks_green.svg" alt="/icons/fireworks_green.svg" width="40px" />

**Прыгну** «назад» куда хочу + **передам** туда **информацию** к размышлению

</aside>

```mermaid
flowchart LR

s1(Шаг 1)
setjmp([setjmp])
if((•))
s2(Шаг 2)
s3(Шаг 3)
s4(Шаг 4)
longjmp([longjmp])

s1:::def ==> setjmp:::def
setjmp ==> |возвращаю кое-что…| if:::def
if ==> |…это был 0| s2:::def
s2 -.-> |ой, всё| longjmp:::poss
longjmp -.->|теперь возвращай 30| setjmp
s2 --> |едем дальше| s4:::def
if -.->|…а теперь это 30| s3:::poss
s3 -.-> |и тут дальше| s4

classDef def fill:#979736,color:#fff;
classDef poss fill:#f0bf4f;
```

# Конечно, придумали типовые трюки

### Транзакции и обработка исключений (аналог try/catch)

```mermaid
flowchart TB

s0(До)
subgraph Транзакция
s1(1)
s2(2)
s3(3)
end
s4(Обработка Т-ошибки)
s5(После)
if((•))

s0:::step ==> if:::step
if ===> |начало| s1:::step
s1 ==> |дальше| s2:::step
s2 ==> |дальше| s3:::step
s1 -.-> |ой| if
s2 -.-> |ой| if
s3 -.-> |ой| if
s3 ==> |1-2-3 сделали| s5:::step
if -.-> |1-2-3 не вышло| s4:::catch
s4 -.-> |ну ладно тогда| s5

linkStyle 0,1,2,3,7 stroke:#979736,stroke-width:4px;
linkStyle 4,5,6,8,9 stroke:#c7642a,stroke-width:2px;
classDef step fill:#fff;
classDef catch fill:#c7642a,color:#fff;

```

```c
#include <stdio.h>
#include <setjmp.h>

jmp_buf env;

void critical_error() {
    printf("Ошибка! Немедленный выход.\n");
    longjmp(env, 1);  // Вернёт выполнение в setjmp()
}

int main() {
    if (setjmp(env) == 0) { // Сразу и создали точку возврата 
        printf("Запуск программы\n");
        critical_error();  // Вместо return используем longjmp()
    } else {
        printf("Обработали ошибку!\n");
    }
    return 0;
}
```

### Эмуляция многозадачности (планировщик задач)

```mermaid
flowchart LR

subgraph task_1
s1_1((1))
s2_1((2))
s3_1((3))
s4_1((4))
s1_1 ==> s2_1 ==> s3_1 ==> s4_1
end

subgraph task_2
s1_2((1))
s2_2((2))
s3_2((3))
s4_2((4))
s1_2 ==> s2_2 ==> s3_2 ==> s4_2
end

planner((&))

planner ==> s1_1
planner ==> s1_2

s1_1 -.-> s1_2
s1_2 -.-> s2_1
s2_1 -.-> s2_2
s2_2 -.-> s3_1
s3_1 -.-> s3_2
s3_2 -.-> s4_1
s4_1 -.-> s4_2

s4_1 ==> end_
s4_2 ==> end_

end_((•))

```

```c
#include <stdio.h>
#include <setjmp.h>
#include <unistd.h>

jmp_buf task_a, task_b;
int current = 1;
int i = 0;

void switch_task() {
    if (current == 1) {
        current = 2;
        longjmp(task_b, 1);
    } else {
        current = 1;
        longjmp(task_a, 1);
    }
    sleep(1);
}

void task_A() {
    if (setjmp(task_a) == 0) {} else {
        printf("Задача A, шаг %d\n", i);
        switch_task();
    }
}

void task_B() {
    if (setjmp(task_b) == 0) {} else {
        printf("Задача B, шаг %d\n", i++);
        switch_task();
    }
}

int main() {
    task_A();  // запуски с нулями, сохранение состояний
    task_B();  // или, что тоже, инициация точек входа (возврата)
    switch_task();
    return 0;
}
```

### Обработка сигналов

```c
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>

jmp_buf env;

void handler(int sig) {
    printf("Пойман сигнал %d! Возвращаемся в main()\n", sig);
    longjmp(env, 1);
}

int main() {
    signal(SIGINT, handler);  // настраиваем мониторинг сигнала SIGINT
    
    if (setjmp(env) == 0) {
        while (1) {
            printf("Работаем... (нажми Ctrl+C для теста)\n");
            sleep(1);
        }
    } else {
        printf("Программа продолжает работу после сигнала\n");
    }
    return 0;
}
```

`SIGSEGV` — сигнал обращения к недопустимой памяти (интересно обработать)

`SIGFPE` — ошибка деления (например, делим на ноль)

`SIGINT` — нажатие Ctrl+C
