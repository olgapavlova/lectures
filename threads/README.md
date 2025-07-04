
![2025-03-21](https://github.com/user-attachments/assets/06c1c161-6ff5-4ca3-86e7-f67c9251c673)

Youtube-запись лекции от ```2025-03-21```: https://youtu.be/DetO0YF-DqE

# Самое-самое начало многопоточности

## Мы на базаре!
👾 Или даже где похуже

- Программ много, это мы уже знаем.
- Постоянная **битва за ресурсы**: время процессора, доступы, данные, память, да что угодно.
- Как ни странно, это игра по правилам. **Правилам операционной системы.**

```mermaid
flowchart TB

subgraph Прикладные пограммы
p1((1))
p2((2))
p3((3))
p4((4))
end

subgraph Система
s1(A)
s2(B)
s3(C)
s4(D)
s5(E)
end

subgraph Ресурсы
processor(процессор)
memory(память)
network(сеть)
files(файлы)
devices(устройства)
end

s1:::sys ==o processor:::res
s2:::sys ==o memory:::res
s3:::sys ==o network:::res
s4:::sys ==o files:::res
s5:::sys ==o devices:::res

p1:::apply --> s1
p2:::apply --> s1
p3:::apply --> s1
p4:::apply --> s1

p1 --> s2
p2 --> s2
p3 --> s2
p4 --> s2

p1 --> s3
p2 -.-> s3
p3 -.-> s3
p4 --> s3

p1 -.-> s4
p2 -.-> s4
p3 --> s4
p4 -.-> s4

p1 --> s5
p2 -.-> s5
p3 --> s5
p4 -.-> s5

linkStyle 0,1,2,3 stroke:#A59986,stroke-width:11px,color:red;
linkStyle 4 stroke:#D65D0E,stroke-width:11px,color:red;
classDef apply fill:#979736,color:#fff;
classDef sys fill:#D79921;
classDef res fill:#272727,color:#E8DBB6;

```

Но что если ты и есть операционная система?!

**Будь как все.** Как все прикладные программы.

## Как все — это как? Что там за порядки?

очереди — буферы — **потоки** — семафоры — мьютексы — балансировщики — …


**Это не про язык программирования.**
Это про операционную систему и архитектурные приёмы.


## Коллективам действовать проще, чем одиночкам


«Я позабочусь о **ресурсах**, а вы **действуйте**»


```mermaid
flowchart LR

res((что смог добыть))

main([босс–родитель-главный])
t1([Ваня])
t2([Маша])
t1:::apply ==> |мам, купи!| main:::sys
t2:::apply ==> |мне в школу надо| main

main <-.-> res:::res

classDef apply fill:#979736,color:#fff;
classDef sys fill:#D79921;
classDef res fill:#272727,color:#E8DBB6;

```

- **Много что в семье общее** — но есть и своё.
- При добыче и удержании ресурсов есть феномены, о которых **детям лучше не знать**.

```bash
# Сколько у нас логических ядер?
nproc

# Сколько у нас оперативной памяти?
free -h
```

## Пора кодить


«Отселяем» функцию в отдельный поток


`-lpthread`

`pthread.h`

`pthread_t`

`int pthread_create (pthread_t * THREAD_ID, void * ATTR, void *(*THREAD_FUNC*) (*void*), void * ARG)`


А вот и утечки памяти — ой


Получаем значения из потоков


…или просто ждём


`void pthread_exit(void *return_value)`

`int pthread_join (pthread_t THREAD_ID, void ** DATA)`

`int pthread_detach(pthread_t *thread)`


Работаем с общей памятью


`pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;`

`pthread_mutex_lock(&lock)`

`pthread_mutex_unlock(&lock)`

## Ошибки валятся со всех сторон

- Функции стандартных библиотек далеко не всегда потокобезопасны (`static` — ой).
- Потоки запросто могут испортить общие данные («гонка»).
- Умер родитель — умрут и потомки (если специально кое-что не предпринять).
