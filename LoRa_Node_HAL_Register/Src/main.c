#include "main.h"
#include "usart.h"
#include "gpio.h"
#include "my_gpio.h"

uint8_t buff[6];
void SystemClock_Config(void);


int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  
	delayInit();
	GPIO_Init(GPIOA, 0, OUT_HIGH, GP_PP);
	
	/* Standby mode configuration */
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
		
	buff[4] = ADC1->DR & 0xFF;
	buff[5] = (ADC1->DR) >> 8;
	
	HAL_UART_Transmit(&huart1, buff, 6, 1000);
	while(RTC->CNTL != 0x0003)
		{
			if(1)
			{
				GPIO_TogglePin(GPIOA, 0);
			}
		}
		
	// Wait for all task will be done
	HAL_Delay(100);
	Enter_StandbyMode();
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM3 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM3) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
