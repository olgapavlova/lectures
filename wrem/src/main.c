#include <avr/io.h>
#include <avr/interrupt.h>

#define LED_PIN PB4
#define CHECK_EVERY_TICK 5
#define TICKS_IN_TIME_UNIT 10
#define UNITS_TO_ALARM 4

typedef enum {
    STATE_START,
    STATE_WAIT,
    STATE_CHECK,
    STATE_ALARM,
    STATE_DROP
} state_t;

state_t current_state_g = STATE_START;

uint16_t timer_check(void);
uint16_t timer_units(void);
uint16_t timer_ticks(void);

void light_init(void) {
    DDRB  |= (1 << LED_PIN);      // настроить LED_PIN как выход
}

void light_switch(void) {
    PORTB ^= (1 << LED_PIN);
}

void light_on(void) {
  PORTB |= (1 << LED_PIN);
}

void light_off(void) {
    PORTB &= ~(1 << LED_PIN);
}


void do_start(void) {

}

void do_wait(void) {

}

void do_check(void) {
    light_switch();
}

void do_alarm(void) {
    light_on();
}

void do_drop(void) {

}


void state_do(void) {
    light_off();

    switch (current_state_g) {
        case (STATE_START): { do_start(); break; }
        case (STATE_WAIT): { do_wait(); break; }
        case (STATE_CHECK): { do_check(); break; }
        case (STATE_ALARM): { do_alarm(); break; }
        case (STATE_DROP): { do_drop(); break; }
        default: {}
    }
}

uint8_t is_time_to_check(void) {
    return (timer_check() == 0);
}

uint8_t is_ready_to_alarm(void) {
    return (timer_units() > UNITS_TO_ALARM);
}

uint8_t is_reacted(void) {
    return 0;
}


void state_update(void) {
    light_off();

    switch (current_state_g) {
        case (STATE_START): { 
                              // Безусловно на WAIT
                              current_state_g = STATE_WAIT;
                              break; }
        case (STATE_WAIT): {
                             // Если пора, уходим на CHECK
                             if (is_time_to_check()) {
                                current_state_g = STATE_CHECK;
                             } // Если не пора, остаёмся в WAIT
                             break; }
        case (STATE_CHECK): { 
                             // Если пора, включаем ALARM
                             if (is_ready_to_alarm()) {
                                current_state_g = STATE_ALARM;
                             } else { // Если не пора, возвращаемся в WAIT
                                current_state_g = STATE_WAIT;
                             }
                              break; }
        case (STATE_ALARM): { 
                              // Если пользователь отреагировал, идём в DROP
                             if (is_reacted()) {
                                current_state_g = STATE_DROP;
                             }
                              // Если нет, остаёмся в ALARM
                              break; }
        case (STATE_DROP): {
                             // Безусловно переходим в WAIT
                             current_state_g = STATE_WAIT;
                             break; }
        default: {}
    }
}

void timer_init() {
    TCCR0A = 0;                          // normal mode
    TCCR0B |= (1 << CS02) | (1 << CS00); // делитель /1024
    TIMSK0 |= (1 << TOIE0);              // разрешить прерывание по переполнению
    TCNT0 = 0;                           // стартовое значение счётчика
}

volatile uint16_t ticks_g = 0;
volatile uint16_t check_g = 0;
volatile uint16_t units_g = 0;

ISR(TIM0_OVF_vect) {
    ticks_g++;                             // volatile!
    check_g = ticks_g % CHECK_EVERY_TICK;  // фокус с периодичностью
    if (ticks_g >= TICKS_IN_TIME_UNIT) {   // считаем в юнитах
        ticks_g = 0;
        units_g++;
    }
}

uint16_t timer_ticks() {
  cli(); // временно отключим прерывания
  uint16_t result = ticks_g;
  sei(); // включим обратно
  return result;
}

uint16_t timer_check() {
  cli(); // временно отключим прерывания
  uint16_t result = check_g;
  sei(); // включим обратно
  return result;
}

uint16_t timer_units() {
  cli(); // временно отключим прерывания
  uint16_t result = units_g;
  sei(); // включим обратно
  return result;
}


int main(void) {

  light_init();
  light_on();

  timer_init();
  sei();

  while(1) {
    state_do();
    state_update();
  }

    return 0;
}
