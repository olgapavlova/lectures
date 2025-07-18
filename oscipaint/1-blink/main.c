// «Мигаем» (напряжением) на пине D11

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB |= (1 << PB3);       // выход на пин D11
    while (1) {
        PORTB ^= (1 << PB3);  // мигаем
        _delay_ms(50);       // задержка в лоб
    }
}
