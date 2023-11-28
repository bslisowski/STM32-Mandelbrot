#ifndef _BUTTON_H_
#define _BUTTON_H_
#include "hal_common.h"

#define ACTIVE_LOW  0
#define ACTIVE_HIGH 1

struct button {
    const uint16_t pin;
    const uint8_t active;
    const uint8_t pull;
    uint16_t state;
    uint16_t db_state;
    void (*on_press)(void);
};
typedef struct button button;


// state = (state << 1) | read(btn) | 1111111000000000      0xfe00
// return (state == 1111111100000000)                       0xff00

void update_btn(button *btn);
void init_btn(button *btn);

#endif