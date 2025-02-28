![2025-02-28](https://github.com/user-attachments/assets/8e4f008e-7681-429a-a0f8-32d5a44f4586)

Youtube-запись лекции от ```2025-02-28```: https://youtu.be/s84SIS6sWA0

# Структуры данных для себя, любимых

Начнём сразу с примера

<aside>
<img src="/icons/alien_purple.svg" alt="/icons/alien_purple.svg" width="40px" />

И с очень страшного кода

</aside>

## Дойди за 7 шагов

- Можно с гарантией сделать шаг вперёд на одну клетку…
- или с равной вероятностью либо прыгнуть вперёд, либо отскочить назад

```mermaid
flowchart LR
0((0))
1((1))
2((2))
3((3))
4((4))
4:::back
5((5))
6((6))
6:::here
7((7))
8((8))
9((9))
9:::forward

0 ~~~ 1 ~~~ 2 ~~~ 3 ~~~ 4 ~~~ 5 ~~~ 6 ~~~ 7 ~~~ 8 ~~~ 9

6 -.-> |50%| 9
6 -.-> |50%| 4
6 --> |100%| 7

classDef back fill:#bb3629,color:#fff;
classDef here fill:#cd9b3e;
classDef forward fill:#979736,color:#fff;
```

## Что нам нужно?

- Упорядоченный список клеток
- В каждой клетке — длины шагов вперёд и назад

```c
game[i] = forward * 10 + back;
```

## А что если…

- Хотим задавать произвольное число шагов

> динамический массив
> 

- Хотим, чтобы в некоторых клетках лежали сокровища

```c
g * 100 + f * 10 + b
```

- Хотим гулять по кругу, пока не соберём все сокровища

```c
if(i == PATH_LEN) { i = 0; }
```

- Хотим двумерную карту… нет, трёхмерную… нет,…

```c
game[x][y][a] // и так далее
```

- Хотим открывать новые куски карты по мере продвижения

> динамический массив
> 

- Хотим в некоторых точках проваливаться в специальную игру

```c
s * 1000 + g * 100 + f * 10 + b
```

<aside>
<img src="/icons/baseball-cap_red.svg" alt="/icons/baseball-cap_red.svg" width="40px" />

Кошмар какой-то

</aside>

## Нас спасут структуры с указателями

- Хотим задавать произвольное число шагов —— динамический массив
- Хотим, чтобы в некоторых клетках лежали сокровища

```c
struct cell {
	int x;
	int forward;
	int back;
	int treasure;
};
```

- Хотим гулять по кругу, пока не соберём все сокровища

```c
typedef struct cell {} cell;

struct cell {
	int forward;
	int back;
	int treasure;
	cell * next;
};
```

- Хотим двумерную карту… нет, трёхмерную… нет,…

```c
typedef ...
struct cell {
...
	cell ** directions;
};
```

- Хотим открывать новые куски карты по мере продвижения
- Хотим в некоторых точках проваливаться в специальную игру

```mermaid
flowchart LR

subgraph struct_a
a_number((number))
a_value((value))
a_next{{next}}
a_prev{{prev}}
end

subgraph struct_b
b_number((number))
b_value((value))
b_next{{next}}
b_prev{{prev}}
end

subgraph struct_c
c_number((number))
c_value((value))
c_next{{next}}
c_prev{{prev}}
end

a_next ==> struct_b
b_prev ==> struct_a

b_next ==> struct_c
c_prev ==> struct_b

```

- Перейти к следующему элементу
- Понять, что мы дошли до конца
- Обратиться к элементу напрямую по номеру — получаем список
- Находить значение по имени структуры — получаем словарь

1. Структура «элемент».
2. Дополнить указателями для связываний и переходов.
3. Остаться в статике — поштучная динамика — массивы всех видов.
