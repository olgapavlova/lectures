<img width="1280" height="720" alt="2025-07-18" src="https://github.com/user-attachments/assets/4c23f7b4-0d52-4cc0-ae8b-2bd02de96a05" />

Youtube-запись от `2025-07-18`: https://youtu.be/j6P9VzOvUic


# Рисуем на осциллографе

Зачем?

- Скучно изучать осциллограф по инструкции
- В любой непонятной ситуации — рисуй

<img width="1200" height="857" alt="1-boa-constrictor-digesting-an-elephant-coloring-page" src="https://github.com/user-attachments/assets/ba85fa65-f23f-44c0-a268-c3e87d378fe5" />


…и заодно посмотрим, как сделать интересный **pet-проект** буквально из ничего



## Превратим blink в что-нибудь на осциллографе

```c
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB |= (1 << PB3);      // выход на пин D11
    while (1) {
        PORTB ^= (1 << PB3);  // мигаем
        _delay_ms(500);       // задержка в лоб
    }
}
```

- Так-то мы это уже делали
- Только мигали на другом пине
- Datasheet на микроконтроллер всё помнит!

## Воспользуемся ШИМ-возможностями контроллера

- Мы не можем менять высоту ступеньки
- Но можем менять её ширину
- Термин «скважность» — «обратный» к ширине

```c
…
    // Настраиваем таймер 2 и канал
    TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20);  // несмещённый PWM, Fast PWM
    TCCR2B = (1 << CS21);  // делитель на 8

    while (1) {
        for (uint8_t i = 0; i < 255; i++) {
            OCR2A = i;         // установить скважность (яркость)
            _delay_ms(100);
        }

        for (int i = 255; i >= 0; i--) {
            OCR2A = i;
            _delay_ms(100);
        }
    }
…
```

- Таймеры работают сами по себе — **не нужно писать циклы**
- На некотором временном интервале — `TCCR2B` …
- … таймер растёт от нуля до максимума.
- А тем временем на привязанном к таймеру выводе `PB3` (ну, почти так) …
- … напряжение меняется с 0 на 1 по отсечке `OCR2A`

```mermaid
flowchart LR

time{{растянуто по заданному времени}}:::white

counter((счётчик))
check(сравнить)
div((порог))

subgraph вывод
0((0))
1((1))
end

counter:::yellow ==> |+1| check:::black

check <--> |больше?| div:::yellow

div ==> |да| 1:::green
div ==> |нет| 0:::red

check -.-> |сбросить| counter

classDef yellow fill:#FABD2F;
classDef black fill:#504945,color:#fff;
classDef green fill:#B8BA46;
classDef red fill:#FE8019;
classDef white fill:#fff;
```



Зачем?



**Получить «порцию» электричества**
Потом придумаем, как растянуть в линию



## Получим горизонтальную линию
<img width="640" height="480" alt="IMG_0680" src="https://github.com/user-attachments/assets/b9028bef-3a4f-4e36-abe0-07f86cf41f95" />


- «Порция» — это интеграл под графиком
- Проводим новую линию так, чтобы интеграл не изменился
- И так на каждом отрезке
- **Ура**, у нас прямая!



Это называется RC-цепочка



Засады в ассортименте



## А теперь — под углом

```c
…
    OCR2A = 64;  // установить порог

    while (1) {
        for(int i = 0; i <= 255; i++) {
            OCR2A = i;
            _delay_ms(1);
        }

        for(int i = 255; i >= 0; i--) {
            OCR2A = i;
            _delay_ms(2);
        }
    }
…
```

- Углы разные
- Чуете, как пахнет библиотекой?
- Это экран? А какой размерности?

## Можно рисовать слона, он же удав
<img width="800" height="480" alt="DS1Z_QuickPrint1" src="https://github.com/user-attachments/assets/275ee047-2efb-495d-83b1-c8679c8293c8" />

```c
OCR2A = elefant[i];
```

- «Просто» возьмём значения из массива
- А какую брать размерность?
- Осторожно, микроконтроллер!


## Закрасим слоника?

```c
OCR2A = elefant[i];
_delay_ms(32);
int y = ((boa[i] == 0) ? 0 : 126 - (boa[i] - 240));
OCR2A = y;
_delay_ms(32);
```
<img width="800" height="480" alt="DS1Z_QuickPrint8" src="https://github.com/user-attachments/assets/118e13ea-21a9-4f77-9f39-6b64ff7112f5" />

- И снова влияет качество RC-цепочки?


**Но где взять данные** для слоно-удаво-массива? Об этом в следующий раз
