.PHONY: all clean cf val
.SILENT: all clean

# Объекты компиляции
SRC = scan config# все имена (без расширений) c-файлов
SRC_O = $(SRC:%=%.o)

# Настройки компиляции
CC = gcc
CFLAGS = -D_DEFAULT_SOURCE -Wall -Wextra -Werror -std=c11 -O2
CPOSTFLAGS = -lm -lpthread # эти флаги должны стоять последними

# Цель по умолчанию
all: clean scan.run

# Проверка стиля (не нормализация, только проверка)
cf:
	cp ../materials/linters/.clang-format .
	clang-format -n *.c *.h

# Проверка на утечки
val:
	valgrind --leak-check=full # что проверяем?

# Линковка
%.run: $(SRC_O)
	@$(CC) $(CFLAGS) $^ -o $@ $(CPOSTFLAGS)

# Компиляция (без линковки)
%.o: %.c %.h
	@$(CC) $(CFLAGS) -c $< -o $@ $(CPOSTFLAGS)

# Очистка
clean:
	@rm -f *.o *.run

# Для удобства, чтобы не было опечаток
clear: clean
