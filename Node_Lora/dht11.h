#include "stm32f10x.h" 
#include "systick_delay.h"
#include "gpio.h"


void DHT_Run(uint8_t buff[4]);
uint8_t DHT_Read(void);
uint8_t DHT_CheckResponse(void);

