#include "spi.h"
#include "stm32f1xx.h"

void spi2_init(void) {
	__HAL_RCC_SPI2_CLK_ENABLE();

}
