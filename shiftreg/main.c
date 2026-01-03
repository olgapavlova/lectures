#include <avr/io.h>
#include <avr/interrupt.h>

#define DATA_PIN   PB0
#define CLOCK_PIN  PB3
#define LATCH_PIN  PB1

// LOW — светодиод горит, HIGH — светодиод выключен
static inline void data_high(void) { PORTB &= ~(1 << DATA_PIN); }
static inline void data_low(void)  { PORTB |=  (1 << DATA_PIN); }

// Снятие показаний со входа
static inline void clock_high(void) { PORTB |=  (1 << CLOCK_PIN); }
static inline void clock_low(void)  { PORTB &= ~(1 << CLOCK_PIN); }

// Выгрузка из внутреннего регистра
static inline void latch_high(void) { PORTB |=  (1 << LATCH_PIN); }
static inline void latch_low(void)  { PORTB &= ~(1 << LATCH_PIN); }

// Снимаем показания со входа
static void pulse_clock(void) {
    clock_high();
    clock_low();
}

// Выгружаем значения внутреннего регистра
void shift_out(uint8_t value) {
    latch_low();

    for (int8_t i = 0; i < 8; i++) {
        if (value & (1 << i))
            data_high();
        else
            data_low();

        pulse_clock();
    }

    latch_high();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

// Работа с таймером
volatile uint16_t ms_counter = 0;  // счётчик миллисекунд
volatile uint8_t step = 0;         // счётчик секунд (и чисел на светодиодах)
volatile uint8_t flag = 0;         // флаг увеличения счётчика

ISR(TIMER0_COMPA_vect) {
    ms_counter++;

    if (ms_counter >= 1000) {   // 1000 мс = 1 сек
        ms_counter = 0;
        step++;
        flag = 1;
    }
}

// Инициация прерывания по таймеру
static void timer0_init(void) {
    TCCR0A = (1 << WGM01);
    TCCR0B = 0;
    TCCR0B |= (1 << CS02);
    OCR0A = 64;
    TIMSK |= (1 << OCIE0A);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int main(void) {
    // настройка выходов
    DDRB |= (1 << DATA_PIN) | (1 << LATCH_PIN) | (1 << CLOCK_PIN);

    timer0_init();
    sei();   // глобально разрешаем прерывания

    while (1) {
      if (flag) {
        shift_out(step);
        flag = 0;
      }
    }
}
