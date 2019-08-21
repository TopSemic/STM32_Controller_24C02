//=============================================================================
//  24C02 Demo code runs on AT24C02
//  www.TopSemic.com
//  Version: 2019.08.21
//=============================================================================
#include "main.h"
#include "24LC02.h"

static __IO uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;
uint8_t test_data[256];

void Delay(__IO uint32_t nTime);
void InitGPIO(void);
void RleaseI2C(void);


int main(void)
{
	static uint32_t i;
	uint8_t *pointer;
	
  // SysTick end of count event each 1ms
  RCC_GetClocksFreq(&RCC_Clocks);
	
	// RCC_Clocks.HCLK_Frequency = 8000000
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	RleaseI2C( );              // This is not necessary, but can improve robustness
	InitI2C1( );
	
	AT24C02_WriteByte(0,0x54);
	Delay(10);
	AT24C02_WriteByte(1,0x6F);
	Delay(10);
	AT24C02_WriteByte(2,0x70);
	Delay(10);
	AT24C02_WriteByte(3,0x53);
	Delay(10);
	AT24C02_WriteByte(4,0x65);
	Delay(10);
	AT24C02_WriteByte(5,0x6D);
	Delay(10);
	AT24C02_WriteByte(6,0x69);
	Delay(10);
	AT24C02_WriteByte(7,0x63);
	Delay(10);
	
  for(i = 8;i < 256;i ++)
	{
		test_data[i] = i;
	}
	
	pointer = test_data;
	for(i = 8;i < 256;i += 8)
	{
	  AT24C02_PageWrite(i,8,pointer + i);
		Delay(10);
	}
		
		
	while(1)
	{
		for(i = 0;i < 256;i ++)
		  test_data[i] = 0;
		
		RleaseI2C( );
		
		for(i = 0;i < 8;i ++)
	    test_data[i] = AT24C02_ReadByte(i);
		
		pointer = test_data;
		for(i = 8;i < 256;i += 8)
	  {
			AT24C02_SequentialRead(i, 8, pointer + i);
	  }
		
		Delay(100);
	}
	
}



// Initialize PB8 and PB9 as output
void InitGPIO(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  
  /* Configure the GPIO pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 

}


// Rlease the I2C module and device in case of SCL/SDA lock up
void RleaseI2C(void)
{
	uint8_t i;
	
	I2C_Cmd(I2C1, DISABLE);
	InitGPIO( );
	
	for(i = 0;i < 9;i ++)
	{
		GPIOB->BSRR |= GPIO_Pin_8; 
	  Delay(1);
	  GPIOB->BRR |= GPIO_Pin_8; 
	  Delay(1);
	}
		
  GPIO_DeInit(GPIOB);
  InitI2C1( );
}


/**
* @brief  Inserts a delay time.
* @param  nTime: specifies the delay time length, in 1 ms.
* @retval None
*/
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;
  
  while(TimingDelay != 0);
}

/**
* @brief  Decrements the TimingDelay variable.
* @param  None
* @retval None
*/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
* @}
*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
