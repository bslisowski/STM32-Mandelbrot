#ifndef _ST7789V_H_
#define _ST7789V_H_

#include "hal_common.h"
#include "spi.h"

// COMMANDS
#define ST7789V_NOP         0x00
#define ST7789V_SWRESET     0x01    // software reset - wait 5ms after reset
#define ST7789V_RDDID       0x04
#define ST7789V_RDDST       0x09
#define ST7789V_RDDPM       0x0A
#define ST7789V_RDDMADCTL   0x0B
#define ST7789V_RDDCOLMOD   0x0C
#define ST7789V_RDDIM       0x0D
#define ST7789V_RDDSM       0x0E
#define ST7789V_RDDSDR      0x0F
#define ST7789V_SLPIN       0x10    // sleep mode on
#define ST7789V_SLPOUT      0x11    // sleep mode off
#define ST7789V_PTLON       0x12
#define ST7789V_NORON       0x13
#define ST7789V_INVOFF      0x20
#define ST7789V_INVON       0x21    // display inversion
#define ST7789V_GAMSET      0x26    // gama curve
#define ST7789V_DISPOFF     0x28    // display off
#define ST7789V_DISPON      0x29    // display on
#define ST7789V_CASET       0x2A    // column address set
#define ST7789V_RASET       0x2B    // row address set
#define ST7789V_RAMWR       0x2C    // memory write
#define ST7789V_RAMRD       0x2E
#define ST7789V_PTLAR       0x30
#define ST7789V_VSCRDEF     0x33    // vertical scroll def
#define ST7789V_TEOFF       0x34
#define ST7789V_TEON        0x35
#define ST7789V_MADCTL      0x36    // memory data access control
#define ST7789V_VSCSAD      0x37
#define ST7789V_IDMOFF      0x38    // idle mode
#define ST7789V_IDMON       0x39
#define ST7789V_COLMOD      0x3A    // interface pixel format
#define ST7789V_WRMEMC      0x3C    // write memory continue
#define ST7789V_RDMEMC      0x3E
#define ST7789V_STE         0x44
#define ST7789V_GSCAN       0x45
#define ST7789V_WRDISBV     0x51
#define ST7789V_RDDISBV     0x52
#define ST7789V_WRCTRLD     0x53    // write CTRL display
#define ST7789V_RDCTRLD     0x54
#define ST7789V_WRCACE      0x55
#define ST7789V_RDCABC      0x56
#define ST7789V_WRCABMB     0x5E
#define ST7789V_RDCABMB     0x5F
#define ST7789V_RDABCSDR    0x68
#define ST7789V_RDID1       0xDA
#define ST7789V_RDID2       0xDB
#define ST7789V_RDID3       0xDC
#define ST7789V_RAMCTRL     0xB0    // ram control
#define ST7789V_RGBCTRL     0xB1    // rgb interface control
#define ST7789V_PORCTRL     0xB2    // porch setting    
#define ST7789V_FRCTRL1     0xB3    
#define ST7789V_PARCTRL     0xB5
#define ST7789V_GCTRL       0xB7    // gate control 
#define ST7789V_GTADJ       0xB8    // gate on timing adjustment
#define ST7789V_DGMEM       0xBA    // digital gamma enable
#define ST7789V_VCOMS       0xBB    
#define ST7789V_LCMCTRL     0xC0
#define ST7789V_IDSET       0xC1
#define ST7789V_VDVVRHEN    0xC2
#define ST7789V_VRHS        0xC3
#define ST7789V_VDVS        0xC4
#define ST7789V_VCMOSFSET   0xC5
#define ST7789V_FRCTRL2     0xC6
#define ST7789V_CABCCTRL    0xC7
#define ST7789V_REGSEL1     0xC8
#define ST7789V_REGSEL2     0xCA
#define ST7789V_PWMFRSEL    0xCC
#define ST7789V_PWCTRL1     0xD0
#define ST7789V_VAPVANEN    0xD2
#define ST7789V_CMD2EN      0xDF
#define ST7789V_PVGAMCTRL   0xE0
#define ST7789V_NVGAMCTRL   0xE1
#define ST7789V_DGMLUTR     0xE2
#define ST7789V_DGMLUTB     0xE3
#define ST7789V_GATECTRL    0xE4
#define ST7789V_SPI2EN      0xE7
#define ST7789V_PWCTRL2     0xE8
#define ST7789V_EQCTRL      0xE9
#define ST7789V_PROMCTRL    0xEC
#define ST7789V_PROMEN      0xFA
#define ST7789V_NVMSET      0xFC
#define ST7789V_PROMACT     0xFE

#define PIXEL_SIZE  16U


struct st7789v_config {
    SPI_TypeDef *spi;
    uint16_t cs;
    uint16_t dc;
    uint16_t rst;
    uint16_t width, height;
    uint8_t x_offset, y_offset;
    uint8_t vcom, gctrl, vrhs, vdvs, mdac, gamma, colmod, lcm;
    uint8_t porch[5]; 
    uint8_t cmd2en[4];
    uint8_t pwctrl[2];
    uint8_t pvgam[14];
    uint8_t nvgam[14];
    uint8_t ram[2];
    uint8_t rgb[3]; 
};

struct display_buffer {
    uint16_t *buffer;
    uint16_t x, y, height, width;
};

static struct st7789v_config *display;

static inline void _write_st7789v(uint8_t cmd, uint8_t *data, uint16_t size) {
    cs_enable(display->cs);
    if (cmd != ST7789V_NOP) {
        _gpio_write(display->dc, 0);
        spi_write_buf(&cmd, 1, display->cs);
    }

    if (data != NULL) {
        _gpio_write(display->dc, 1);
        spi_write_buf(data, size, display->cs);
    }
    spin(2);
    cs_disable(display->cs);
    spin(1);
}

static inline void _draw(struct display_buffer *b) {
    uint16_t x = b->x + display->x_offset;
    uint16_t y = b->y + display->y_offset;
    uint16_t size = (uint16_t)(b->width * b->height - 1);
    // CASET/ RASET
    uint16_t addr[2];
    addr[0] = x;
    addr[1] = (uint16_t)(x + b->width - (uint16_t)1);
    uint8_t buf[4];
    buf[0] = (uint8_t)(addr[0] >> 8);
    buf[1] = (uint8_t)(addr[0] & 0xFF);
    buf[2] = (uint8_t)(addr[1] >> 8);
    buf[3] = (uint8_t)(addr[1] & 0xFF);
    _write_st7789v(ST7789V_CASET, buf, 4);

    addr[0] = y;
    addr[1] = (uint16_t)(y + b->height - (uint16_t)1);
    buf[0] = (uint8_t)(addr[0] >> 8);
    buf[1] = (uint8_t)(addr[0] & 0xFF);
    buf[2] = (uint8_t)(addr[1] >> 8);
    buf[3] = (uint8_t)(addr[1] & 0xFF);
    _write_st7789v(ST7789V_RASET, buf, 4);
    _write_st7789v(ST7789V_RAMWR, (uint8_t *)b->buffer, size * 2);
}

int set_background(struct display_buffer *b);

int draw(struct display_buffer *b);

int init_st7789v(struct st7789v_config *config);

#endif