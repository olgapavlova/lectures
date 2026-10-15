<img width="1280" height="720" alt="2025-07-11" src="https://github.com/user-attachments/assets/912a67c9-d11f-480c-853a-7f859e666761" />

Youtube-запись от `2025-07-11`: https://youtu.be/ZFVOB4V7ss4

# Экономим нервы: две фишки git’а, затейливый bash-скрипт и настройки в отдельном файле

## Одни и те же свои модули в разных проектах


Всё время пишем **одно и то же**:
— логи;
— тесты;
— работа со строками;
— символьный терминал;
— диагностика через UART…


…но в каждом проекте **чуть-чуть** по новому


Как развивать проект и модуль **одновременно**?


- Подцепим
    
    ```bash
    git submodule add [адрес репозитория] [в какую папку проекта класть]
    ```
    
    - И у нас появился файл `.gitmodules`
    - Не забудем обновить родителя
- Коммитимся дважды: подмодуль (**к себе**) + основной код (**к себе**)
- Можно скрещивать [Gitlab](https://about.gitlab.com) и [Github](https://github.com)
- Как теперь клонировать такой репозиторий?
    
    ```bash
    git clone --recurse-submodules [адрес]
    ```
    
- А если уже склонировали без?
    
    ```bash
    git submodule init
    git submodule update
    ```
    

## Настройки среды, терминала, редактора и т. д.

- Github-репозитории `dotfiles` (или `.dotfiles`) — целая [субкультура](http://dotfiles.github.io)
- Заводить `.git/` в корне — ну, можно, конечно, но так лучше:
    
    ```bash
    alias dotfiles='/usr/bin/git --git-dir=/home/op/.dotfiles/ --work-tree=/home/op'
    ```
    
- И после этого `dotfiles pull` в скрипте запуска (например, `.bashrc` или `.zshrc`)

## Дела на потом

- `TODO` — метка подсвечивается почти любым редактором кода
- Ищем и находим
    
    ```bash
    find . -type f -exec grep -nHT TODO {} ';'
    ```
    
- Переход сразу
    
    ```bash
    vim +normal\ zR +[номер строки] [название файла]
    ```
    


И сразу хочется большего. Но **нет**.


## Сильная зависимость поведения от предопределённых параметров

- Обычно пары `ключ=значение` можно положить в файл `.env`
- **Зачем?** Менять поведение программы «на лету» (или хотя бы не в коде)
- Загружать перед запуском, не внутри кода:
    
    ```bash
    env $(cat .env | grep -v '#' | xargs) ./app.run
    ```
    
    Ещё одна причина использовать `Makefile`
    

    
- 🧐 Добавить в `.gitignore`, может содержать ключи
- Минимум: логирование (`DEBUG`), пути к каталогам, ключи
- Можно так, но не всюду:
    
    ```c
    int main(int argc, char *argv[], char *envp[]) {}
    ```
    
- Чаще всего так:
    
    ```c
    #include <stdlib.h>
    ...
    char * env_or_default(const char * name, const char * default_value) {
        char * value = getenv(name);
        return value ? value : default_value;
    }
    ```
    
- Давайте проверим:
    
    ```c
    #include <stdio.h>
    #include <stdlib.h>
    
    int main(void) {
        char * debug = getenv("DEBUG");
        printf("DEBUG = %s\n", debug ? debug : "undefined");
        return 0;
    }
    ```
