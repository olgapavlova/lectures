// Смотрим на переменную скважность 

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // Настраиваем PB3 (D11) как выход
    DDRB |= (1 << PB3);

    // Настраиваем таймер 2 в Fast PWM режиме, без делителя
    TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20);  // несмещённый PWM, Fast PWM
    TCCR2B = (1 << CS20);  // делитель на 8
    OCR2A = 64;           // установить скважность (яркость)

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
}
