include config.mk

%.docker:
	# Вызываем docker-команду make с целью $*
	@docker run --rm -v $(PRJ):/prj dev make -C /prj $* $(MAKEPOSTFLAGS)

all:
	# Главная цель

test:
	# Тестирование

run:
	# Запуск программы

clean:
	# Очистка всего
