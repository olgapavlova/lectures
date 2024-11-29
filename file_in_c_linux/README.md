![02-cover](https://github.com/user-attachments/assets/061262e2-9d60-453c-9f85-8bc132a147aa)

# Нюансы работы с файлами в C и Linux
Материалы к стриму 2024-11-29.
> [Запись на Youtube](https://youtu.be/W86H9NbHVqA?si=uhbu7YjIQgPreip7) (1 час 10 минут)

---

### Низкоуровневая работа с файлами

Основная идея: пусть работой с файлами занимается операционная система.

> **open()** — открываем файл, получаем дескриптор
> 

> **read()** — читаем из файла в буфер
> 

> **write()** — записываем буфер в файл
> 

> **lseek()** — перемещаем курсор
> 

> **close()** — закрываем файл
> 

https://www.opennet.ru/docs/RUS/zlp/005.html — база по низкоуровневому вводу-выводу

https://www.opennet.ru/docs/RUS/zlp/appendix02.html#4  — таблица флагов режима открытия

https://en.wikipedia.org/wiki/C_file_input/output — stdio.h

```c

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
...
int desc = open(path, flags, mode);
...
#define BUFFER_SIZE 64;  // а лучше 4096
char * buffer = malloc(BUFFER_SIZE + 1);
ssize_t received = read(desc, buffer, BUFFER_SIZE);  // не size_t, потому что -1 — ошибка
...
ssize_t sended = write(1, buffer, received);
if (sended != received) exit(EXIT_FAILURE);
...
close(desc);
```

### Сгенерим большой файл

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char ** argv) {
	FILE * f = fopen(argv[1], "w+t");
	int rand = open("/dev/random", O_RDONLY, 0);

	if ((f == NULL) || (argc == 1)) exit(EXIT_FAILURE);

	char buffer = '\0';
	int max = INT_MAX / 10;
	for (int i = 0; i < MAX; ++i) {
		read(rand, &buffer, sizeof(char));
		char c = (buffer < 0) ? -buffer : buffer;
		fprintf(f, "%c", c); 
	}

	close(rand);
	fclose(f);
	exit(EXIT_SUCCESS);
}

```

### Как узнать размер файла?

> **stat()** — структура из файлового индекса
> 

https://www.opennet.ru/man.shtml?topic=stat&category=2&russian=0 — использование структуры и команды

https://pubs.opengroup.org/onlinepubs/7908799/xsh/sysstat.h.html — константы и макросы

```c
#include <sys/stat.h>
...
struct stat fs;
stat(filename, &fs);
...
long long int file_size;
if (fs.st_mode & S_IFMT) == S_IFDIR) { printf("DIR\n"); }
else { file_size = fs.st_size; }
...
S_ISDIR()
```

### Как узнать, существует ли файл?

**stat()** вернёт -1

### Какие «магические» файлы можно использовать в Linux?

`stdin` `stdout` `stderr` стандартные потоки

`/dev` `/tmp` служебные файловые системы

`/dev/null` пустота

`/dev/random` близко к истинной случайности, но иногда зависает

`/dev/urandom` псевдослучайное

### Как использовать стандартные потоки?

```c
#include <stdio.h>
...
FILE * f = ...
int fd = fileno(f);
```

### Как измерить скорость работы разных способов считывания?

```bash
=time -p util > /dev/null
```

- Всё зависит от размера буфера

### Как считывать текстовый файл построчно?

> [**GNU C**](https://www.gnu.org/software/libc/manual/html_node/index.html) — библиотека со всеми удобствами
> 

> **getline()** — построчное чтение
> 

> [Макросы тестируемых свойств](https://ru.manpages.org/feature_test_macros/7)

- Перенос строки `\n` сохраняется
- Буфер кратен 16
- Специфические типы данных
- `-1` — «длина» непрочтённого
- Обязательно освобождать буфер

https://www.gnu.org/software/libc/manual/html_node/index.html — документация библиотеки

https://www.opennet.ru/man.shtml?topic=getline&category=3&russian=0 — документация getline

https://www.gnu.org/software/libc/manual/html_node/Line-Input.html — по всем строковым функциям GNU C

https://c-for-dummies.com/blog/?p=1112 — подробный разбор

https://ru.manpages.org/getline/3— простая справка 

```c
#define _GNU_SOURCE
...
size_t buffer = 0;
ssize_t len;
...
len = getline(&line, &buffer, file);
...
free(buffer);
```

> Память выделяется в нужном количестве


### Как использовать систему временных файлов?

```c
#include <stdio.h>
#include <unistd.h>
char * filename = "/tmp/flecXXXXXX";  // потом заменит на настоящее
int tempdesc = mkstemp(filename);
unlink(tempdesc);
// read write как обычно
```

### Как работать с очень большими файлами?

> **mmap()** — отображение файла в память (адресное пространство программы)
> 

http://asu.cs.nstu.ru/~evgen/man/mmap.html — справка из учебного курса НГУ

https://habr.com/ru/articles/820591/ — если коротко, то «используйте только для чтения, но не для записи»

https://www.ibm.com/docs/ru/aix/7.1?topic=techniques-mapped-files — короткая справка IBM

https://habr.com/ru/articles/55716/ — зачем + база + нюансы в комментах

https://ru.wikipedia.org/wiki/Mmap — немного архитектурной базы

https://www.opennet.ru/cgi-bin/opennet/man.cgi?topic=mmap&category=2 — синтаксис и коды ошибок

```c
#include <sys/mman.h>
```

### Как сделать буфер «для себя»?

### Как рекурсивно обработать файлы в каталоге?

### Сколько файлов можно открыть одновременно?

### Какие есть ограничения по размеру файлов?

### Как хранить базу данных в бинарном файле?
