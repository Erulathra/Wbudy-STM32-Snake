#ifndef WBUDY_STM32_SNAKE_ST7735_H
#define WBUDY_STM32_SNAKE_ST7735_H

#include "stm32f1xx.h"

#define ST7735_RESET GPIO_PIN_13
#define ST7735_RESET_PORT GPIOB
#define ST7735_DC GPIO_PIN_14
#define ST7735_DC_PORT GPIOB
#define ST7735_CS GPIO_PIN_15
#define ST7735_CS_PORT GPIOB


//RGB to RGB565
#define ST7735_RGB565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3))

// Some register settings
#define ST7735_MADCTL_BGR 0x08
#define ST7735_MADCTL_MH 0x04

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR 0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1 0xC0
#define ST7735_PWCTR2 0xC1
#define ST7735_PWCTR3 0xC2
#define ST7735_PWCTR4 0xC3
#define ST7735_PWCTR5 0xC4
#define ST7735_VMCTR1 0xC5

#define ST7735_PWCTR6 0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1

#define ST_CMD_DELAY 0x80 // special signifier for command lists

#define ST7735_NOP 0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID 0x04
#define ST7735_RDDST 0x09

#define ST7735_SLPIN 0x10
#define ST7735_SLPOUT 0x11
#define ST7735_PTLON 0x12
#define ST7735_NORON 0x13

#define ST7735_INVOFF 0x20
#define ST7735_INVON 0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON 0x29
#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_RAMWR 0x2C
#define ST7735_RAMRD 0x2E

#define ST7735_PTLAR 0x30
#define ST7735_TEOFF 0x34
#define ST7735_TEON 0x35
#define ST7735_MADCTL 0x36
#define ST7735_COLMOD 0x3A

#define ST7735_MADCTL_MY 0x80
#define ST7735_MADCTL_MX 0x40
#define ST7735_MADCTL_MV 0x20
#define ST7735_MADCTL_ML 0x10
#define ST7735_MADCTL_RGB 0x00

#define ST7735_RDID1 0xDA
#define ST7735_RDID2 0xDB
#define ST7735_RDID3 0xDC
#define ST7735_RDID4 0xDD

// Some ready-made 16-bit ('565') color settings:
#define ST7735_BLACK 0x0000
#define ST7735_WHITE 0xFFFF
#define ST7735_RED 0xF800
#define ST7735_GREEN 0x07E0
#define ST7735_BLUE 0x001F
#define ST7735_CYAN 0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW 0xFFE0
#define ST7735_ORANGE 0xFC00

#define ST7735_SCREEN_WIDTH 128
#define ST7735_SCREEN_HEIGHT 128
#define ST7735_COLUMN_START 2
#define ST7735_ROW_START 3



//StartUp Commands:

static const uint8_t Rcmd1[] = {                       // 7735R init, part 1 (red or green tab)
        15,                             // 15 commands in list:
        ST7735_SWRESET, ST_CMD_DELAY, //  1: Software reset, 0 args, w/delay
        150,                          //     150 ms delay
        ST7735_SLPOUT, ST_CMD_DELAY, //  2: Out of sleep mode, 0 args, w/delay
        255,                          //     500 ms delay
        ST7735_FRMCTR1, 3,              //  3: Framerate ctrl - normal mode, 3 arg:
        0x01, 0x2C, 0x2D,             //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
        ST7735_FRMCTR2, 3,              //  4: Framerate ctrl - idle mode, 3 args:
        0x01, 0x2C, 0x2D,             //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
        ST7735_FRMCTR3, 6,              //  5: Framerate - partial mode, 6 args:
        0x01, 0x2C, 0x2D,             //     Dot inversion mode
        0x01, 0x2C, 0x2D,             //     Line inversion mode
        ST7735_INVCTR, 1,              //  6: Display inversion ctrl, 1 arg:
        0x07,                         //     No inversion
        ST7735_PWCTR1, 3,              //  7: Power control, 3 args, no delay:
        0xA2, 0x02,                         //     -4.6V
        0x84,                         //     AUTO mode
        ST7735_PWCTR2, 1,              //  8: Power control, 1 arg, no delay:
        0xC5,                         //     VGH25=2.4C VGSEL=-10 VGH=3 * AVDD
        ST7735_PWCTR3, 2,              //  9: Power control, 2 args, no delay:
        0x0A,                         //     Opamp current small
        0x00,                         //     Boost frequency
        ST7735_PWCTR4, 2,              // 10: Power control, 2 args, no delay:
        0x8A,                         //     BCLK/2,
        0x2A,                         //     opamp current small & medium low
        ST7735_PWCTR5, 2,              // 11: Power control, 2 args, no delay:
        0x8A, 0xEE, ST7735_VMCTR1, 1,              // 12: Power control, 1 arg, no delay:
        0x0E, ST7735_INVOFF, 0,              // 13: Don't invert display, no args
        ST7735_MADCTL, 1,              // 14: Mem access ctl (directions), 1 arg:
        0xC8,                         //     row/col addr, bottom-top refresh
        ST7735_COLMOD, 1,              // 15: set color mode, 1 arg, no delay:
        0x05};                                      // 16 bit color

static const uint8_t Rcmd2[] = {                        // 7735R init, part 2 (green 1.44 tab)
        2,                              //  2 commands in list:
        ST7735_CASET, 4,              //  1: Column addr set, 4 args, no delay:
        0x00, 0x00,                   //     XSTART = 0
        0x00, 0x7F,                   //     XEND = 127
        ST7735_RASET, 4,              //  2: Row addr set, 4 args, no delay:
        0x00, 0x00,                   //     XSTART = 0
        0x00, 0x7F};                           //     XEND = 127

static const uint8_t Rcmd3[] = {                        // 7735R init, part 3 (red or green tab)
        4,                              //  4 commands in list:
        ST7735_GMCTRP1, 16,       //  1: Gamma Adjustments (pos. polarity), 16 args + delay:
        0x02, 0x1c, 0x07, 0x12,       //     (Not entirely necessary, but provides
        0x37, 0x32, 0x29, 0x2d,       //      accurate colors)
        0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10, ST7735_GMCTRN1, 16,       //  2: Gamma Adjustments (neg. polarity), 16 args + delay:
        0x03, 0x1d, 0x07, 0x06,       //     (Not entirely necessary, but provides
        0x2E, 0x2C, 0x29, 0x2D,       //      accurate colors)
        0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10, ST7735_NORON, ST_CMD_DELAY, //  3: Normal display on, no args, w/delay
        10,                           //     10 ms delay
        ST7735_DISPON, ST_CMD_DELAY, //  4: Main screen turn on, no args w/delay
        100};                                       //     100 ms delay

void ST7735_Init();
void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);

void ST7735_FillRectangle(uint16_t x, uint16_t i, uint16_t w, uint16_t h, uint16_t color);
void ST7735_FillScreen(uint16_t color);
void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t size, const uint16_t *data);

#endif //WBUDY_STM32_SNAKE_ST7735_H
