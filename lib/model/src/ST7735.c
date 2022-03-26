#include "ST7735.h"
#include "stm32f1xx.h"
#include <stdlib.h>

#define TFT_CS_HIGH()    HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS, GPIO_PIN_SET)
#define TFT_CS_LOW()    HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS, GPIO_PIN_RESET)
#define TFT_DC_DATA()    HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC, GPIO_PIN_SET)
#define TFT_DC_COMMAND()    HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC, GPIO_PIN_RESET)
#define TFT_RESET_HIGH()    HAL_GPIO_WritePin(ST7735_RESET_PORT, ST7735_RESET, GPIO_PIN_SET)
#define TFT_RESET_LOW()    HAL_GPIO_WritePin(ST7735_RESET_PORT, ST7735_RESET, GPIO_PIN_RESET)

extern SPI_HandleTypeDef spi;

static void ST7735_WriteCommand(uint8_t cmd) {
    TFT_DC_COMMAND();
    HAL_SPI_Transmit(&spi, &cmd, sizeof(cmd), HAL_MAX_DELAY);
}

static void ST7735_WriteData(uint8_t *buff, size_t buff_size) {
    TFT_DC_DATA();
    HAL_SPI_Transmit_DMA(&spi, buff, buff_size);
    while (spi.State == HAL_SPI_STATE_BUSY_TX);
}

static void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    // column address set
    ST7735_WriteCommand(ST7735_CASET);
    uint8_t data[] = {0x00, x0 + ST7735_COLUMN_START, 0x00, x1 + ST7735_COLUMN_START};
    ST7735_WriteData(data, 4);

    // row address set
    ST7735_WriteCommand(ST7735_RASET);
    data[1] = y0 + ST7735_ROW_START;
    data[3] = y1 + ST7735_ROW_START;
    ST7735_WriteData(data, 4);

    // write to RAM
    ST7735_WriteCommand(ST7735_RAMWR);
}


void ST7735_ExecuteCommandArray(const uint8_t *addr) {
    uint8_t numCommands, numArgs;
    uint16_t ms;

    TFT_CS_LOW();


    numCommands = *addr++;
    while (numCommands--) {
        uint8_t cmd = *addr++;
        ST7735_WriteCommand(cmd);

        numArgs = *addr++;
        // If high bit set, delay follows args
        ms = numArgs & ST_CMD_DELAY;
        numArgs &= ~ST_CMD_DELAY;
        if (numArgs) {
            ST7735_WriteData((uint8_t *) addr, numArgs);
            addr += numArgs;
        }

        if (ms) {
            ms = *addr++;
            if (ms == 255) ms = 500;
            HAL_Delay(ms);
        }
    }

    TFT_CS_HIGH();
}

static void ST7735_Reset() {
    TFT_RESET_LOW();
    HAL_Delay(100);
    TFT_RESET_HIGH();
}

static void ST7735_GPIO_Init() {
    __HAL_RCC_SPI1_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pin = GPIO_PIN_5 | GPIO_PIN_7; // SCK, MOSI
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio);

    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pin = ST7735_CS; // CS
    HAL_GPIO_Init(ST7735_CS_PORT, &gpio);

    gpio.Pin = ST7735_RESET; // CS
    HAL_GPIO_Init(ST7735_RESET_PORT, &gpio);

    gpio.Pin = ST7735_DC; // CS
    HAL_GPIO_Init(ST7735_DC_PORT, &gpio);
}

void ST7735_Init() {
    ST7735_GPIO_Init();
    ST7735_Reset();
    TFT_CS_LOW();

    ST7735_ExecuteCommandArray(Rcmd1);
    ST7735_ExecuteCommandArray(Rcmd2);
    ST7735_ExecuteCommandArray(Rcmd3);
    TFT_CS_HIGH();
}

void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if ((x >= ST7735_SCREEN_WIDTH) || (y >= ST7735_SCREEN_HEIGHT))
        return;

    TFT_CS_LOW();

    ST7735_SetAddressWindow(x, y, x + 1, y + 1);
    uint8_t data[] = {color >> 8, color & 0xFF};
    ST7735_WriteData(data, sizeof(data));

    TFT_CS_HIGH();
}

void ST7735_FillRectangle(uint16_t x, uint16_t i, uint16_t w, uint16_t h, uint16_t color) {
    // clipping
    if ((x >= ST7735_SCREEN_WIDTH) || (i >= ST7735_SCREEN_HEIGHT)) return;
    if ((x + w - 1) >= ST7735_SCREEN_WIDTH) w = ST7735_SCREEN_WIDTH - x;
    if ((i + h - 1) >= ST7735_SCREEN_HEIGHT) h = ST7735_SCREEN_HEIGHT - i;

    TFT_CS_LOW();
    ST7735_SetAddressWindow(x, i, x + w - 1, i + h - 1);

    uint8_t data[] = {color >> 8, color & 0xFF};
    uint8_t tbuf[w * 2];
    for (i = h; i > 0; i--) {
        for (int j = w * 2; j >= 0; j -= 2) {
            tbuf[j] = data[0];
            tbuf[j + 1] = data[1];
        }
        ST7735_WriteData(tbuf, sizeof(tbuf));
    }
    TFT_CS_HIGH();
}

void ST7735_FillScreen(uint16_t color) {
    ST7735_FillRectangle(0, 0, ST7735_SCREEN_WIDTH, ST7735_SCREEN_HEIGHT, color);
}

void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t size, uint16_t *data) {
    if ((x >= ST7735_SCREEN_WIDTH) || (y >= ST7735_SCREEN_HEIGHT)) return;
    if ((x + w * size - 1) >= ST7735_SCREEN_WIDTH) return;
    if ((y + w * size - 1) >= ST7735_SCREEN_HEIGHT) return;


    TFT_CS_LOW();
    ST7735_SetAddressWindow(x, y, x + w * size - 1, y + h * size - 1);

    uint16_t lineBuffer[w * size];
    uint16_t *lineBuffer_ptr = lineBuffer;
    uint16_t *data_ptr = data;

    for (int i = h; i > 0; i--) {
        lineBuffer_ptr = lineBuffer;

        for (int j = 0; j < w; ++j) {
            for (int k = 0; k < size; ++k) {
                //*lineBuffer_ptr = ST7735_SWAP_BYTES(*data_ptr);
                *lineBuffer_ptr = (*data_ptr);
                lineBuffer_ptr++;
            }
            data_ptr++;
        }

        for (int k = 0; k < size; ++k) {
            ST7735_WriteData((uint8_t *) lineBuffer, sizeof(lineBuffer));
        }
    }
    TFT_CS_HIGH();
}

void ST7735_DrawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {

    if ((x >= ST7735_SCREEN_WIDTH) || (y >= ST7735_SCREEN_HEIGHT)) return;
    if ((x + w - 1) >= ST7735_SCREEN_WIDTH) w = ST7735_SCREEN_WIDTH - x;

    ST7735_FillRectangle(x, y, w, 1, color);

}

void ST7735_DrawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    TFT_CS_LOW();

    // Rudimentary clipping
    if ((x >= ST7735_SCREEN_WIDTH) || (y >= ST7735_SCREEN_HEIGHT)) return;
    if ((y + h - 1) >= ST7735_SCREEN_HEIGHT) h = ST7735_SCREEN_HEIGHT - y;

    ST7735_FillRectangle(x, y, 1, h, color);
}

void ST7735_DrawBuffer(uint16_t bufferIteration, uint16_t buffer[BUFFER_HEIGHT][ST7735_SCREEN_WIDTH]) {
    if (bufferIteration > BUFFER_COUNT - 1) return;

    uint16_t startY = bufferIteration * BUFFER_HEIGHT;

    TFT_CS_LOW();
    ST7735_SetAddressWindow(0, startY, ST7735_SCREEN_WIDTH - 1, startY + BUFFER_HEIGHT - 1);
    //ST7735_SetAddressWindow(0, startY, ST7735_SCREEN_WIDTH - 10, startY + BUFFER_HEIGHT - 1);

    ST7735_WriteData((uint8_t *) buffer, ST7735_SCREEN_WIDTH * BUFFER_HEIGHT * 2);
    TFT_CS_HIGH();
}


