/**
 * @file main.c
 * @brief Основной код программы
 */

#include <avr/io.h>
#include <avr/interrupt.h>

// Конфигурация железа
#define LED_PIN PB4
#define BUTTON_PIN PB3

// Конфигурация формулы подсчёта
#define WEIGHT 79

// Конфигурация проверок
#define CHECK_EVERY_TICK 2
#define ALARM_EVERY_TIME_UNIT 6
#define TICKS_IN_TIME_UNIT 5


// Машина состояний
typedef enum {      // весь набор состояний
    STATE_START,    // начинаем работу
    STATE_WAITING,  // ждём очередной проверки
    STATE_CHECK,    // проверяем, какое состояние следующее
    STATE_ALARM,    // ждём сброса
    STATE_DROP      // обрабатываем сигнал сброса
} state_t;

// Анонсируем функции заранее 
void state_update(void);
void state_do(void);
void light_switch(void);

// Устанавливаем глобальные переменные
// …состояния
state_t current_state_g = STATE_START;

// …счётчика времени
volatile uint16_t ticks_g = 0;
volatile uint8_t check_g = 0;
volatile uint16_t units_g = 0;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// Обработчик прерывания по переполнению
ISR(TIM0_OVF_vect) {
    ticks_g++;
    check_g = ticks_g % CHECK_EVERY_TICK;
    if (ticks_g >= TICKS_IN_TIME_UNIT) {
        ticks_g = 0;
        units_g++;
    }
}

// Инициация счётчика времени через регистры
void timer_init() {
    TCCR0A = 0;                          // Normal mode
    TCCR0B |= (1 << CS02) | (1 << CS00); // делитель /1024
    TIMSK0 |= (1 << TOIE0);              // разрешить прерывание по переполнению
    TCNT0 = 0;                           // стартовое значение счётчика
}

// Сбросить счётчик времени
void time_drop() {
    cli(); // временно отключим прерывания
    ticks_g = 0;
    check_g = 0;
    units_g = 0;
    sei(); // включим обратно
}

// Узнать, сколько прошло времени
uint16_t time_units() {
    uint16_t result;
    cli();
    result = units_g;
    sei();
    return result;
}

// Узнать, пора ли запускать проверку 
uint16_t time_check() {
    uint8_t result;
    cli();
    result = check_g;
    sei();
    return result;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Точка входа
int main(void) {

    // Настроить регистры
    DDRB  |= (1 << LED_PIN);      // настроить LED_PIN как выход
    DDRB  &= ~(1 << BUTTON_PIN);  // настроить BUTTON_PIN как вход
    PORTB &= ~(1 << BUTTON_PIN);  // отключить внутреннюю подтяжку (есть внешняя)

    timer_init();
    sei();
    while (1) {
        state_do();
        state_update();
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Пора ли снова проверять (CHECK)
int is_time_to_check(void) {
    return time_check();
}

// Пора ли напоминать (ALARM)
int is_ready_to_alarm(void) {
    return (time_units() > ALARM_EVERY_TIME_UNIT);
}

// Отреагировал ли человек на напоминалку (DROP)
int is_reacted() {
    int result = ((PINB & (1 << BUTTON_PIN)) == 0);  // нажата ли кнопка
    return result;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void light_on(void) {
    PORTB |= (1 << LED_PIN);
}

void light_off(void) {
    PORTB &= ~(1 << LED_PIN);
}

void light_switch(void) {
    PORTB ^= (1 << LED_PIN);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void do_start(void) {
  light_off();
}

void do_waiting(void) {
}

void do_check(void) {
  light_off();
}

void do_alarm(void) {
  light_on();
}

void do_drop(void) {
  time_drop();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// Выполняем действие в зависимости от состояния
void state_do(void) {
    switch (current_state_g) {
        case STATE_START: { do_start(); break; }
        case STATE_WAITING: { do_waiting(); break; }
        case STATE_CHECK: { do_check(); break; }
        case STATE_ALARM: { do_alarm(); break; }
        case STATE_DROP: { do_drop(); break; }
        default: { break; }
    }
}

// Обновляем состояния
void state_update(void) {
    switch (current_state_g) {
        case STATE_START: {
                        // Сразу переводим в WAITING
                        current_state_g = STATE_WAITING;
                        break;
                     }        
        case STATE_WAITING: {
                        // Проверяем, пора ли снова проверять (CHECK)
                        if (is_time_to_check()) { current_state_g = STATE_CHECK; }
                        break;
                     }        
        case STATE_CHECK: {
                        // Проверяем, пора ли напоминать (ALARM)
                        if (is_ready_to_alarm()) {
                          current_state_g = STATE_ALARM;
                        } else {
                          current_state_g = STATE_WAITING;
                        }
                        break;
                     }        
        case STATE_ALARM: {
                        // Проверяем, отреагировал ли человек на напоминалку (DROP)
                        if (is_reacted()) { current_state_g = STATE_DROP; }
                        break;
                     }        
        case STATE_DROP: {
                        // Сразу переводим в WAITING
                        current_state_g = STATE_WAITING;
                        break;
                     } 
        default: {
                        // По умолчанию ждём
                        current_state_g = STATE_WAITING;
                 }
    }
}
