#include "spi.h"
#include <stdio.h>

int init_spi(uint16_t miso, uint16_t mosi, uint16_t sck) {
    if (!IS_MISO_PIN(miso)) {
        printf("ERR: Invalid MISO\r\n");
        return -1;
    }
    if (!IS_MOSI_PIN(mosi)) {
        printf("ERR: Invalid MOSI\r\n");
        return -1;
    }
    if (!IS_SCK_PIN(sck)) {
        printf("ERR: Invalid SCK\r\n");
        return -1;
    }
    _init_spi(miso, mosi, sck);
    return 0;
}