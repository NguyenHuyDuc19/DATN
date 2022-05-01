#include "dht11.h"
float TEM, HUM;


uint8_t DHT_Read(void)
{
	uint8_t var;
	for(int i = 0; i < 8; i++)
	{
		while(!GPIO_ReadPin(GPIOB, 9)); //Wait for PB9 high (end of 50ms signal)
		delayUs(40);
		if(GPIO_ReadPin(GPIOB, 9))
		{
			var = (var << 1)|(0x01); //Write 1 to var
			while(GPIO_ReadPin(GPIOB, 9)); //Wait for PB9 low (end of 70ms signal)
		}
		else
		{
			var = (var << 1); //Write 0 to var
		}
	}
	return var;
}

uint8_t DHT_CheckResponse(void)
{
	while(GPIO_ReadPin(GPIOB, 9)); //Wait till the end of 10ms high signal
	delayUs(90);
	if(!GPIO_ReadPin(GPIOB, 9))
	{
		return 0; 
	}
	else
	{
		while(GPIO_ReadPin(GPIOB, 9));//Wait till the end 80ms high signal
		return 1;
	}
}

void DHT_Run(uint8_t buff[4])
{
	
	uint8_t tmp_buff[4] = {0, 0, 0, 0}, sum = 0;
	//Config GPIO
	GPIO_Init(GPIOB, 9, OUT_HIGH, GP_OD);
	
	//Transmit start signal
	GPIO_WritePin(GPIOB, 9, 0);
	delayMs(20);
	GPIO_WritePin(GPIOB, 9, 1);
	
	if(DHT_CheckResponse())
	{
		tmp_buff[0] = DHT_Read();
		tmp_buff[1] = DHT_Read();
	
		tmp_buff[2] = DHT_Read();
		tmp_buff[3] = DHT_Read();

		sum = DHT_Read();
		
		if(sum == tmp_buff[0] + tmp_buff[1] + tmp_buff[2] + tmp_buff[3])
		{
			buff[0] = tmp_buff[0];
			buff[1] = tmp_buff[1];
			buff[2] = tmp_buff[2];
			buff[3] = tmp_buff[3];
		}
	}
}
