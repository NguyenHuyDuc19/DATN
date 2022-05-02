#include "stm32f1xx_hal.h"
#include "systick_delay.h"
#include "my_gpio.h"


void DHT_Run(uint8_t buff[4]);
uint8_t DHT_Read(void);
uint8_t DHT_CheckResponse(void);

