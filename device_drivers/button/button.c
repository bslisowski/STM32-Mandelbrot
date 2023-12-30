#include "button.h"
#include "gpio.h"

// state = (state << 1) | read(btn) | 1111111000000000      0xfe00
// return (state == 1111111100000000)                       0xff00

static inline uint16_t read_button(button *btn) {
    uint16_t val = _gpio_read(btn->pin);
    if (btn->active == ACTIVE_LOW) val = !val;
    return val;
}

static uint16_t debounce(button *btn) {
    uint16_t new_state = read_button(btn);
    btn->db_state = (uint16_t)(btn->db_state << 1u) | new_state | 0xfe00;
    return (btn->db_state == 0xff00);
}

void update_btn(button *btn) {
    if (btn->state) {
        btn->state = read_button(btn);
        if (btn->state == 0) btn->on_press();
    }
    else {
        btn->state = debounce(btn);
        if (btn->state) btn->db_state = 0;
    }
}

void init_btn(button *btn){
    gpio_set_mode(btn->pin, GPIO_MODE_INPUT);
    if (btn->pull) {
        gpio_set_pull(btn->pin, btn->active == ACTIVE_HIGH ? GPIO_PULLDOWN : GPIO_PULLUP);
    }
}