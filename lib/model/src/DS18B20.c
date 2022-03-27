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

static void delay(uint32_t us) {
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

uint8_t DS18B20_Start(void) {
    uint8_t Response = 0;
    ONE_WIRE_DATA_LOW;  // pull the pin low
    delay(480);   // delay according to datasheet

    gpio.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(ONE_WIRE_DATA_PORT, &gpio);
    delay(80);    // delay according to datasheet

    if (!(ONE_WIRE_DATA_READ)) Response = 1;    // if the pin is low i.e the presence pulse is detected
    else Response = -1;

    delay(400); // 480 us delay totally.

    return Response;
}

void DS18B20_Write(uint8_t data) {
    SetPinOutput();  // set as output

    for (int i = 0; i < 8; i++) {

        if ((data & (1 << i)) != 0)  // if the bit is high
        {
            // write 1

            SetPinOutput();  // set as output
            ONE_WIRE_DATA_LOW;  // pull the pin LOW
            delay(1);  // wait for 1 us

            SetPinInput();  // set as input
            delay(50);  // wait for 60 us
        }

        else  // if the bit is low
        {
            // write 0

            SetPinOutput();
            ONE_WIRE_DATA_LOW;  // pull the pin LOW
            delay(50);  // wait for 60 us

            SetPinInput();
        }
    }
}

uint8_t DS18B20_Read(void) {
    uint8_t value = 0;
    SetPinInput();

    for (int i = 0; i < 8; i++) {
        SetPinOutput();   // set as output

        ONE_WIRE_DATA_LOW;  // pull the data pin LOW
        delay(2);  // wait for 2 us

        SetPinInput();  // set as input
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1))  // if the pin is HIGH
        {
            value |= 1 << i;  // read = 1
        }
        delay(60);  // wait for 60 us
    }
    return value;
}