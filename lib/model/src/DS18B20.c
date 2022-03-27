#include "DS18B20.h"

extern TIM_HandleTypeDef tim3;
static GPIO_InitTypeDef gpio;

static void SetPinInput() {
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(ONE_WIRE_DATA_PORT, &gpio);
}

static void SetPinOutput() {
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(ONE_WIRE_DATA_PORT, &gpio);
}

static void Delay(uint32_t us) {
    __HAL_TIM_SET_COUNTER(&tim3, 0);
    while ((__HAL_TIM_GET_COUNTER(&tim3)) < us);
}

void DS18B20_Init(void) {
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pin = ONE_WIRE_DATA_PIN;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ONE_WIRE_DATA_PORT, &gpio);
}

int8_t DS18B20_Start(void) {
    ONE_WIRE_DATA_HIGH;
    Delay(5);
    ONE_WIRE_DATA_LOW;
    Delay(750);
    ONE_WIRE_DATA_HIGH;
    SetPinInput();
    int t = 0;
    while (ONE_WIRE_DATA_READ) {
        t++;
        if (t > 60) return -1;
        Delay(1);
    }
    t = 480 - t;
    SetPinOutput();
    Delay(t);
    ONE_WIRE_DATA_HIGH;
    return 1;
}

void DS18B20_Write(uint8_t data) {
    SetPinOutput();
    for (int i = 0; i < 8; i++) {
        ONE_WIRE_DATA_LOW;
        Delay(10);
        if (data & 1) ONE_WIRE_DATA_HIGH;
        else
            ONE_WIRE_DATA_LOW;
        data >>= 1;
        Delay(50);
        ONE_WIRE_DATA_HIGH;
    }
}

uint8_t DS18B20_Read(void) {
    SetPinOutput();
    ONE_WIRE_DATA_HIGH;
    Delay(2);
    uint8_t data = 0;
    for (int i = 0; i < 8; i++) {
        ONE_WIRE_DATA_LOW;
        Delay(1);
        ONE_WIRE_DATA_HIGH;
        SetPinInput();
        Delay(5);
        data >>= 1;
        if (ONE_WIRE_DATA_READ) data |= 0x80;
        Delay(55);
        SetPinOutput();
        ONE_WIRE_DATA_HIGH;
    }
    return data;
}

double ReadTemp() {
    if (DS18B20_Start() == -1) return -999;
    DS18B20_Write(0xCC); // Send skip ROM command
    DS18B20_Write(0x44); // Send reading start conversion command
    if (DS18B20_Start() == -1) return -999;
    DS18B20_Write(0xCC); // Send skip ROM command
    DS18B20_Write(0xBE); // Read the register, a total of nine bytes, the first two bytes are the conversion value
    int temp = DS18B20_Read(); // Low byte
    temp |= DS18B20_Read() << 8; // High byte
    return temp;
}