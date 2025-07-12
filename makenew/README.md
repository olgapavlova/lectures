<img width="1280" height="720" alt="2025-07-04-2" src="https://github.com/user-attachments/assets/4f0d96c1-f792-406b-a8a3-465c7dbbdfa5" />

Youtube-запись от `2025-07-04`: https://youtu.be/zUNq2oYcMx4

# Makefile для новичков: пара приёмов и общий настрой использования

## Компилируем разный код с «именем» `module`

### Простой код из `src`

```mermaid
flowchart LR
subgraph src
src_c(module.c)
src_h(module.h)
end

subgraph include
include_h(module.h)
end

subgraph lib
lib_h(lib.h)
end

subgraph build
build_o(module.o)
end

compile_point(•)

src_c:::yellow --> compile_point:::black
src_h -.-> compile_point
include_h -.-> compile_point
lib_h -.-> compile_point

compile_point --> build_o:::yellow

classDef yellow fill:#FABD2F;
classDef black fill:#504945,color:#fff;
```

### Код из `test`

```mermaid
flowchart LR
subgraph test
test_c(test_module.c)
test_h(test.h)
end

subgraph include
include_h(module.h)
end

subgraph build
build_o(module.o)
end

compile_point(•)

test_c:::yellow --> compile_point:::black
test_h -.-> compile_point
include_h -.-> compile_point

compile_point --> build_o:::yellow

classDef yellow fill:#FABD2F;
classDef black fill:#504945,color:#fff;
```

### Код из подкаталога `src/app`

```mermaid
flowchart LR
subgraph src
subgraph app
src_app_c(module.c)
src_app_h(module.h)
end
end

subgraph include
include_h(module.h)
end

subgraph lib
lib_h(lib.h)
end

subgraph build
build_o(module.o)
end

compile_point(•)

src_app_c:::yellow --> compile_point:::black
src_app_h -.-> compile_point
include_h -.-> compile_point
lib_h -.-> compile_point

compile_point --> build_o:::yellow

classDef yellow fill:#FABD2F;
classDef black fill:#504945,color:#fff;
```

Три `build/module.o`?!


Так дело не пойдёт


Придумаем **систему `.o`-имён**


## Теперь можно линковать разные `.o`-файлы

### Обычная программа с точкой входа `main()`

```mermaid
flowchart LR

src(src)
src_app(src/app)
test(test)

subgraph build
src_main(src_main.o)
src_module(src_module.o)
src_app_module(src_app_module.o)
test_module(test_module.o)
end

link_main(••)
link_test(••)

main_run(main.run)
test_module_run(test_module.run)
test_sh(test.sh)

src:::white ==> src_main:::yellow
src ==> src_module:::yellow
src_app:::white ==> src_app_module:::yellow
test:::white ==> test_module:::yellow

src_main ==> link_main
src_module ==> link_main:::black
src_app_module ==> link_main

link_main ==> main_run:::green

src_module --> link_test:::black
test_module --> link_test

link_test --> test_module_run:::green
test_module_run -.-> test_sh:::pink

classDef yellow fill:#FABD2F;
classDef green fill:#B8BA46;
classDef black fill:#504945,color:#fff;
classDef white fill:#fff;
classDef pink fill:#FF6DB5,color:#fff;
```

### Библиотека `.a`

```mermaid
flowchart LR

src(src)
src_app(src/app)
test(test)
demo(demo)

subgraph build
src_module(src_module.o)
src_app_module(src_app_module.o)
test_module(test_module.o)
demo_module(demo_module.o)
end

archive_lib(=)
link_test(••)
link_demo(••)

lib_a(lib.a)
test_module_run(test.run)
demo_run(demo.run)

src:::white ==> src_module:::yellow
src_app:::white ==> src_app_module:::yellow
test:::white ==> test_module:::yellow
demo:::white ==> demo_module:::yellow

src_module ==> archive_lib:::black
src_app_module ==> archive_lib

archive_lib ==> lib_a:::cyan

lib_a --> link_test:::black
test_module --> link_test

link_test -->test_module_run:::green

lib_a -.-> link_demo:::black
demo_module -.-> link_demo

link_demo -.-> demo_run:::green

classDef yellow fill:#FABD2F;
classDef green fill:#B8BA46;
classDef black fill:#504945,color:#fff;
classDef white fill:#fff;
classDef cyan fill:#00756F,color:#fff;
 
```

## Вернёмся к `make`

Любимый приём: читаем **справа** *налево*


- Превращать `.c`-файлы в **одноимённые** `.o`-файлы (компилировать, конечно)
    
    > Получать `.o`-файлы из одноимённых `.c`-файлов
    > 
    

```makefile
%.o: %.c
	gcc -c $^ -o $@
	
	# Без проверки .c-файла
	gcc -c $*.c -o $@
	
	# То же самое, но сложней
	gcc -c $(basename $@).c -o $@
```

- При компиляции к `.c`-файлу подключать **только один** — одноимённый — `.h`-файл
    
    > Указывать одинаковые basename у `.c`- и `.h`-файлов
    > 

```makefile
%.o: %.c %.h
	...
```

- Использовать для `.o`-файлов имена, в которых «зашиты» пути к их исходным `.c`-файлам
    
    > В списке на компиляцию использовать `.c`-файл, путь к которому зашифрован в имени `.o`-цели (файла)
    > 

```makefile
# .h — так же, как .c
%.o:
	gcc -c $(subst _,/,$(basename $@)).c -o $@
	...
```

- Превращать наборы `.o`-файлов в `.run`- или `.a`-файлы
    
    > Получать `.run`-файлы из наборов `.o`- и `.a`-файлов (зависимостей)
    > 

```makefile
%.run: $(SET_O) $(SET_A)
	gcc $^ -o $@
	
%.a: $(SET_O)
	ar rc $@ $^
```

- Для каждого `.run`-файла использовать свои `.o`- и `.a`-зависимости (для `.a`-файла аналогично, но только `.o`)
    
    > Получать **имена наборов** зависимостей из имён `.run`-файлов
    > 

```makefile
# Так нельзя :(
~~%.run: $(%_SET_O) $(%_SET_A)~~
```

```makefile

~~~~# Зададим наборы вручную
DEMO_SET_O := demo_main demo_module
DEMO_SET_A := lib cbase
TEST_SET_O := test_module test_mock
TEST_SET_A := lib cbase filedata
SETS       := demo test

# Теперь напишем шаблон правила
# Это просто текст
# $(1) — так пометим место вставки имени набора
define run_template
$(1).run: $($(1)_SET_O) $($(1)_SET_A)
	cc $$^ -o $$@
endef
# Пока это просто многострочная переменная

# Похоже на макрос препроцессора
$(foreach s,$(SETS),$(eval $(call run_template,$(s))))
# call подставляет значения параметров и возвращает текст
# eval превращает текст в make-код
```

- Все `.c`-файлы из одного каталога превращать в `.o`-файлы одинаковым способом
- Для каждого `.run`-файла использовать свои флаги линковщика
- Для каждого `.run`-файла задавать каталог, куда класть результат
- …и всё это делать в Docker-контейнере
