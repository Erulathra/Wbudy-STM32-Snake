#include "stm32f1xx_hal.h"
#include "st7735.h"
#include "gpio.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"


int main(void) {
	HAL_Init();

	__HAL_RCC_GPIOC_CLK_ENABLE(); // Uruchamiam zegar na Porcie GPIOC
	GPIOC->CRH |= (0b0010 << 19); // Ustawiam PUSH PULL i 2MHz na pinie 13
	SET_BIT(GPIOC->CRH, GPIO_OUTPUT_ALTERNATE_PUSH_PULL | GPIO_MODE_2MHZ);


	while (1) {
		GPIOC->ODR ^= (1 << 13);

		HAL_Delay(500);
	}
}

void SysTick_Handler(void) {
	HAL_IncTick();
}

#pragma clang diagnostic pop