#include "ST7735.h"
#include "stm32f1xx.h"

#define TFT_CS_HIGH()    HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS, GPIO_PIN_SET)
#define TFT_CS_LOW()    HAL_GPIO_WritePin(ST7735_CS_PORT, ST7735_CS, GPIO_PIN_RESET)
#define TFT_DC_DATA()    HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC, GPIO_PIN_SET)
#define TFT_DC_COMMAND()    HAL_GPIO_WritePin(ST7735_DC_PORT, ST7735_DC, GPIO_PIN_RESET)
#define TFT_RESET_HIGH()    HAL_GPIO_WritePin(ST7735_RESET_PORT, ST7735_RESET, GPIO_PIN_SET)
#define TFT_RESET_LOW()    HAL_GPIO_WritePin(ST7735_RESET_PORT, ST7735_RESET, GPIO_PIN_RESET)

extern SPI_HandleTypeDef spi;

static int16_t _height = ST7735_SCREEN_HEIGHT, _width = ST7735_SCREEN_WIDTH;
static uint8_t _xstart = ST7735_COLUMN_START, _ystart = ST7735_ROW_START;

static void ST7735_WriteCommand(uint8_t cmd) {
    TFT_DC_COMMAND();
    TFT_CS_LOW();
    HAL_SPI_Transmit(&spi, &cmd, sizeof(cmd), HAL_MAX_DELAY);
    TFT_CS_HIGH();
}

static void ST7735_WriteData(uint8_t *buff, size_t buff_size) {
    TFT_DC_DATA();
    TFT_CS_LOW();
    while (!(SPI1->SR & 2)) {}
    HAL_SPI_Transmit(&spi, buff, buff_size, HAL_MAX_DELAY);
    while (SPI1->SR & 0x80) {}
    TFT_CS_HIGH();
}

static void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
    // column address set
    ST7735_WriteCommand(ST7735_CASET);
    uint8_t data[] = { 0x00, x0 + _xstart, 0x00, x1 + _xstart };
    ST7735_WriteData(data, sizeof(data));

    // row address set
    ST7735_WriteCommand(ST7735_RASET);
    data[1] = y0 + _ystart;
    data[3] = y1 + _ystart;
    ST7735_WriteData(data, sizeof(data));

    // write to RAM
    ST7735_WriteCommand(ST7735_RAMWR);
}


void ST7735_ExecuteCommandArray(const uint8_t *addr) {
    uint8_t numCommands, numArgs;
    uint16_t ms;

    numCommands = *addr++;
    while (numCommands--)
    {
        uint8_t cmd = *addr++;
        ST7735_WriteCommand(cmd);

        numArgs = *addr++;
        // If high bit set, delay follows args
        ms = numArgs & ST_CMD_DELAY;
        numArgs &= ~ST_CMD_DELAY;
        if (numArgs)
        {
            ST7735_WriteData((uint8_t*)addr, numArgs);
            addr += numArgs;
        }

        if (ms)
        {
            ms = *addr++;
            if (ms == 255) ms = 500;
            HAL_Delay(ms);
        }
    }
}

static void ST7735_Reset() {
    TFT_RESET_LOW();
    HAL_Delay(20);
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
    TFT_CS_LOW();
    ST7735_Reset();
    ST7735_ExecuteCommandArray(Rcmd1);
    ST7735_ExecuteCommandArray(Rcmd2);
    ST7735_ExecuteCommandArray(Rcmd3);
    TFT_CS_HIGH();
}

void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    if ((x >= _width) || (y >= _height))
        return;

    TFT_CS_LOW();

    ST7735_SetAddressWindow(x, y, x+1, y+1);
    uint8_t data[] = { color >> 8, color & 0xFF };
    ST7735_WriteData(data, sizeof(data));

    TFT_CS_HIGH();
}

void ST7735_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    // clipping
    if ((x >= _width) || (y >= _height)) return;
    if ((x + w - 1) >= _width) w = _width - x;
    if ((y + h - 1) >= _height) h = _height - y;

    TFT_CS_LOW();
    ST7735_SetAddressWindow(x, y, x+w-1, y+h-1);

    uint8_t data[] = { color >> 8, color & 0xFF };
    TFT_DC_DATA();
    for (y = h; y > 0; y--)
    {
        for (x = w; x > 0; x--)
        {
            uint8_t colorFistByte = color >> 8;
            uint8_t colorSecondByte = color;
            ST7735_WriteData(&colorFistByte, 1);
            ST7735_WriteData(&colorSecondByte, 1);
        }
    }
    TFT_CS_HIGH();
}

void ST7735_FillScreen(uint16_t color)
{
    ST7735_FillRectangle(0, 0, _width, _height, color);
}

void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data) {
    int16_t skipC = 0;
    int16_t originalWidth = w;
    int i = w*(h - 1);

    if ((x >= _width) || ((y - h + 1) >= _height) || ((x + w) <= 0) || (y < 0)){
        return;
    }
    if ((w > _width) || (h > _height)){

        return;
    }
    if ((x + w - 1) >= _width){
        skipC = (x + w) - _width;
        w = _width - x;
    }
    if ((y - h + 1) < 0){
        i = i - (h - y - 1)*originalWidth;
        h = y + 1;
    }
    if (x < 0){
        w = w + x;
        skipC = -1*x;
        i = i - x;
        x = 0;
    }
    if (y >= _height){
        h = h - (y - _height + 1);
        y = _height - 1;
    }

    TFT_CS_LOW();

    ST7735_SetAddressWindow(x, y-h+1, x+w-1, y);

    for(y=0; y<h; y=y+1){
        for(x=0; x<w; x=x+1){
            uint8_t colorFistByte = data[i] >> 8;
            uint8_t colorSecondByte = data[i];

            ST7735_WriteData(&colorFistByte, 1);
            ST7735_WriteData(&colorSecondByte, 1);
            i = i + 1;
        }
        i = i + skipC;
        i = i - 2*originalWidth;
    }
    TFT_CS_HIGH();  //Unselect
}

