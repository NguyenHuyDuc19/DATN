#include "systick_delay.h"

int systick;

void delayInit(void)
{
	//Configure SysTick to generate an interrupt every millisecond
	SysTick_Config(SystemCoreClock /1000000);
}

void SysTick_Handler(void)
{
	systick++;
}

void delayUs(int us)
{
	systick = 0;
	while(systick < us);
}

void delayMs(int ms)
{
	delayUs(ms * 1000);
}
