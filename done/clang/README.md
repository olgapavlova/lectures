
Youtube-запись от 2026-05-08: https://youtu.be/uB1SZtYQ2Ws

# Clang vs GCC — найди 10 отличий

### Apple по уши в Clang
###### Clang очень хорошо маскируется
```bash
which gcc
gcc --version
```

###### Помним про настройку Make через встроенные переменные
```bash
CC=gcc-15
```

Для CMake аналогично:
```bash
CMAKE_C_COMPILER=gcc-15
```

### Собственно компиляторное
###### Что там с языком C?
> [!IMPORTANT]
> The only major difference between the two is that Clang defaults to gnu99 mode while GCC defaults to gnu89 mode

Остальные различия нужно [искать под микроскопом](https://clang.llvm.org/c_dr_status.html).

Но найти можно:
- warning — разные сообщения
- nested functions — clang не может
- undefined behavior — разные результаты
- variable length array — видно при `-Werror=vla`
- statement expression — видно при `-pedantic-errors`


##### Итого — три `C` в одной букве
- `ISO C` — семейство стандартов
- `GNU C` — плюс расширения GNU
- `Clang C` — старается быть совместимым с GNU C

### А вот что действительно важно

> [!TIP]
Clang — всё ради прозрачности

> [!CAUTION]
GCC — всё ради компиляции

###### Какие инструменты вызываются при «обычном» запуске Clang?
```bash
clang -### test.c
```
- Видим: `clang-22` • `ld` + куча флагов

###### Как прокинуть флаги внутрь «оболочки» Clang?
```bash
clang -Xclang -ast-dump -fsyntax-only test.c
clang -Xclang -dump-tokens -fsyntax-only test.c
```
- АСТ — абстрактное семантическое дерево программы
- Проще говоря, «где тут что с точки зрения языка C»
- И кто ж не любит токены!

###### При чём тут IDE?
```bash
clang -Xclang -ast-dump=json -fsyntax-only test.c
```
- АСТ в JSON-формате — даже и не знаю, кому бы могло такое понадобиться…
- Есть даже сервер `clangd` — и IDE очень его любят
- Суть `clangd` — «фоновый компилятор»
- Пока GCC думал, CLang завоевал мир IDE

###### Так что, компилятор в чистом виде не компилирует?
```bash
clang -cc1 test.c
```
- Видно, что он старается
- Но без точных параметров не может ничего

###### Через какой pipeline проходит код?
```bash
clang -ccc-print-phases test.c
```
- Несколько шагов обычно в одном инструменте
- Основные инструменты: `clang -cc1` • `as` • `ld` 

###### Знаменитый LLVM IR
```bash
clang -S -emit-llvm test.c
```
- Получаем `*.ll`-файл
- Хорошо описывает вычисления и поток управления

### Ну и совсем коротко

> [!WARNING]
> Разработка != сборка

- `Clang` хорош для процесса разработки (особенно с IDE)
- `GCC` часто (не всегда) хорош для финальной компиляции
- Потому что они туда развивались. **И развились.**
