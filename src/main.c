#include "stm32f1xx_hal.h"
#include "st7735.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"


int main(void)
{
	HAL_Init();

	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIOC->CRH |= (0b0010 << 19);



	while (1)
	{
		GPIOC->ODR ^= (1<<13);

		HAL_Delay(1000);
	}
}

void SysTick_Handler(void)
{
	HAL_IncTick();
}
#pragma clang diagnostic pop