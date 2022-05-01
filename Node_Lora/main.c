#include "main.h"

uint8_t buff[4], tmp_buff[4];
float tem = 0, hum = 0;
int val =0;
uint8_t check;


int main(void)
{
		delayInit();
		
		/* Enable PWR and BKP clock */
		RCC-> APB1ENR |= 0x18000000U;
	
		/* Allow access to BKP Domain */
		PWR->CR |= 0x0100U;				

		PWR->CR |= PWR_CR_CWUF;
		PWR->CR |= PWR_CR_CSBF;
		GPIO_Configuration();
		/* Configure RTC clock source and prescaler */
		RTC_Configuration();
	
		/* Wait till RTC Second event occurs */
		RTC->CRL &= ~(0x0001U);
 
		while((RTC->CRL & 0x0001U) == 0){}

    /* Set the RTC Alarm after x seconds ( = x*1s) */
		RTC->CRL |= 0x0010U;										// Enter the Configuration Mode
		RTC->ALRH = 0x0000U;										// Set PRLH value
		RTC->ALRL = RTC->CNTL + 0x0005U;				// Set PRLL value = x
		RTC->CRL &= ~(0x0010U);								  // Exit the Configuration Mode	
		while (((RTC->CRL >> 5) & 0x01) != 1){};

		ADC_Configuration();
		DHT_Run(buff);
		hum = (float)buff[0] +((float)buff[1]/10.0);
		tem = (float)buff[2] + (float)buff[3]/10.0;

		while(RTC->CNTL != 0x0003){
			GPIOA->ODR ^= 1;
		}
			
		Enter_StandbyMode();
}
		
