#include "OneWire.h"

extern TIM_HandleTypeDef tim3;


void WireInit() {
    GPIO_InitTypeDef gpio;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pin = ONE_WIRE_DATA_PIN;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ONE_WIRE_DATA_PORT, &gpio);
}

static void DelayUS(uint32_t us) {
    __HAL_TIM_SET_COUNTER(&tim3, 0);
    for (int i = 0; i < 500; ++i) {
        i++;
    }
    int test = __HAL_TIM_GET_COUNTER(&tim3);
    while (__HAL_TIM_GET_COUNTER(&tim3) < us) {}
}

void WireReset() {
    ONE_WIRE_DATA_RESET;
    DelayUS(70);
    ONE_WIRE_DATA_READ;
    DelayUS(410);
}

static int ReadBit(void) {
    int rc;
    ONE_WIRE_DATA_RESET;
    DelayUS(6);
    ONE_WIRE_DATA_SET;
    DelayUS(9);
    rc = ONE_WIRE_DATA_READ;
    DelayUS(55);
    return rc;
}

uint8_t WireRead(void) {
    uint8_t value = 0;
    int i;
    for (i = 0; i < 8; i++) {
        value >>= 1;
        if (ReadBit())
            value |= 0x80;
    }
    return value;
}

static void WriteBit(int value) {
    if (value) {
        ONE_WIRE_DATA_RESET;
        DelayUS(6);
        ONE_WIRE_DATA_SET;
        DelayUS(64);
    }
    else {
        ONE_WIRE_DATA_RESET;
        DelayUS(60);
        ONE_WIRE_DATA_SET;
        DelayUS(10);
    }
}

void WireWrite(uint8_t byte) {
    int i;
    for (i = 0; i < 8; i++) {
        WriteBit(byte & 0x01);
        byte >>= 1;
    }
}

static uint8_t ByteCRC(uint8_t crc, uint8_t byte) {
    int i;
    for (i = 0; i < 8; i++) {
        uint8_t b = crc ^ byte;
        crc >>= 1;
        if (b & 0x01)
            crc ^= 0x8c;
        byte >>= 1;
    }
    return crc;
}

uint8_t WireCRC(const uint8_t *data, int len) {
    int i;
    uint8_t crc = 0;

    for (i = 0; i < len; i++)
        crc = ByteCRC(crc, data[i]);

    return crc;
}
