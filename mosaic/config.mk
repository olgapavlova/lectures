# Все настройки и переменные Makefile
desc_config: __desc_config_stuff \
			 __desc_config_cc
	@echo

PRJ = $(shell pwd)
SHELL := /bin/bash
MAKEPOSTFLAGS := --no-print-directory
__desc_config_stuff:
	@echo
	# Всякие разные переменные
	@echo "	Каталог проекта:  " $(PRJ)
	@echo "	Оболочка в Docker:" $(SHELL)
	@echo "	Постфлаги make:   " $(MAKEPOSTFLAGS)

CC         := gcc
CFLAGS     := -Wall -Wextra -Werror -std=c11 -g \
              -I. \
              -Ihelpers \
              -Is21 \
              -Ilib\
              -DLOGDEF
LDFLAGS    := -lm -lpthread
__desc_config_cc:
	@echo
	# Настройки компилятора
	@echo " Компилятор:        " $(CC)
	@echo " Флаги компиляции:  " $(CFLAGS)
	@echo " Флаги линковки:    " $(LDFLAGS)

