#ifndef _GPIO_HAL_H_
#define _GPIO_HAL_H_

#include "hal_common.h"

#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))

#endif